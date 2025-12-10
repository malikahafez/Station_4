import cv2
import numpy as np
import pytesseract
from fuzzywuzzy import fuzz
import serial
import time
import sys

# --- Configuration ---
pytesseract.pytesseract.tesseract_cmd = r'C:\Program Files\Tesseract-OCR\tesseract.exe' 
REFERENCE_TEXT = "MIX 9:5:2"
CAMERA_INDEX = 0 
SERIAL_PORT = 'COM6'
BAUD_RATE = 115200 
DETECTION_MODE = "HIGHLIGHTER" 

UV_LOWER_COLOR = np.array([80, 50, 150])
UV_UPPER_COLOR = np.array([140, 255, 255])

HIGHLIGHTER_LOWER_COLOR = np.array([30, 80, 150])
HIGHLIGHTER_UPPER_COLOR = np.array([80, 255, 255])

def get_color_bounds(mode):
    if mode == "UV_INK":
        return UV_LOWER_COLOR, UV_UPPER_COLOR
    elif mode == "HIGHLIGHTER":
        return HIGHLIGHTER_LOWER_COLOR, HIGHLIGHTER_UPPER_COLOR
    else:
        raise ValueError("Invalid DETECTION_MODE specified.")

def filter_color_range(frame, lower_bound, upper_bound):
    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv_frame, lower_bound, upper_bound)
    
    kernel = np.ones((3, 3), np.uint8)
    mask = cv2.dilate(mask, kernel, iterations=1)
    mask = cv2.erode(mask, kernel, iterations=1)
    
    return mask

def process_frame(frame, mode):
    lower, upper = get_color_bounds(mode)
    color_mask = filter_color_range(frame, lower, upper)
    processed_image = cv2.GaussianBlur(color_mask, (5, 5), 0)
    return processed_image

def validate_text(extracted_text, reference_text):
    clean_text = "".join(c for c in extracted_text if c.isalnum() or c in [':', ' ']).strip()
    similarity = fuzz.ratio(clean_text.lower(), reference_text.lower())
    
    if similarity > 75:
        return True, clean_text, similarity
    else:
        return False, clean_text, similarity

# --- Initial Setup ---
try:
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.05)
    time.sleep(2)
    print(f"Connected to {SERIAL_PORT}")
except serial.SerialException as e:
    print(f"Error opening serial port {SERIAL_PORT}: {e}")
    sys.exit(1)

cap = cv2.VideoCapture(CAMERA_INDEX)
if not cap.isOpened():
    print(f"Error: Cannot open camera at index {CAMERA_INDEX}.")
    ser.close()
    sys.exit(1)

print(f"Starting OCR monitor in **{DETECTION_MODE}** mode. Press 'q' to quit.")

display_text = "Waiting for OCR command..."
display_color = (255, 255, 255)
processed_image = None
ocr_active = False
last_ocr_text = ""
last_similarity = 0

# --- Main Test Loop ---
while True:
    try:
        ret, frame = cap.read()
        if not ret:
            print("Error: Failed to capture frame.")
            break

        # --- Serial Communication Check ---
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            
            if line == "START_OCR":
                print("\n[RP2040] Command received: START_OCR. Continuously capturing and comparing...")
                ocr_active = True
                response_sent = False

        # --- Continuous OCR Loop ---
        if ocr_active:
            # Process the current frame
            processed_image = process_frame(frame, DETECTION_MODE)
            
            # Run Tesseract OCR
            config = "--psm 6"
            extracted_text = pytesseract.image_to_string(processed_image, config=config).strip()
            
            # Validate the text
            is_match, clean_text, similarity = validate_text(extracted_text, REFERENCE_TEXT)
            
            last_ocr_text = clean_text
            last_similarity = similarity

            if is_match and not response_sent:
                # Match found! Send response
                final_response = f"{REFERENCE_TEXT}\n"
                ser.write(final_response.encode('utf-8'))
                
                display_color = (0, 255, 0)  # Green
                display_text = f"MATCH FOUND! Sent: '{REFERENCE_TEXT}' (Sim: {similarity}%)"
                print(f"[RP2040] **MATCH FOUND!** Response sent: '{clean_text}' (Sim: {similarity}%)")
                
                response_sent = True
                ocr_active = False
                cv2.imshow(f"Processed Image ({DETECTION_MODE})", processed_image)
            else:
                # No match yet, keep trying
                display_color = (0, 165, 255)  # Orange (searching)
                display_text = f"Searching... OCR: '{clean_text}' (Sim: {similarity}%)"
                
                cv2.imshow(f"Processed Image ({DETECTION_MODE})", processed_image)

        # --- Live Video Display ---
        cv2.putText(frame, display_text, 
                    (10, 30),
                    cv2.FONT_HERSHEY_SIMPLEX, 
                    0.7, display_color, 2)

        cv2.imshow("Live Feed | Press 'q' to quit", frame)

        key = cv2.waitKey(1) & 0xFF
        if key == ord('q') or key == 27:
            break

    except KeyboardInterrupt:
        break
    except serial.SerialException:
        print("\n[ERROR] Serial connection lost.")
        break

# --- Clean up ---
print("Test finished. Cleaning up.")
cap.release()
cv2.destroyAllWindows()
ser.close()