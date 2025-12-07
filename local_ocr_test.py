# try:
#     import cv2
#     import numpy as np
# except ImportError:
#     print("Error: 'cv2' (opencv-python) or 'numpy' is not installed. Install with: python -m pip install opencv-python numpy")
#     raise SystemExit(1)

# import pytesseract
# from fuzzywuzzy import fuzz
# import serial
# import time

# # --- Configuration ---
# pytesseract.pytesseract.tesseract_cmd = r'C:\Program Files\Tesseract-OCR\tesseract.exe' 
# REFERENCE_TEXT = "MIX 9:5:2"
# CAMERA_INDEX = 0 
# # --- Configuration ---
# # IMPORTANT: Replace 'COM_PORT_NAME' with the actual port name
# # Windows: 'COM3', 'COM4', etc.
# # Linux: '/dev/ttyACM0' (common for Pico/RP2040)
# # macOS: '/dev/tty.usbmodemXXXX'
# SERIAL_PORT = 'COM6' # Adjust this for your OS
# BAUD_RATE = 115200 # Must match stdio_init_all's default baud (often ignored for USB CDC but good practice)
# # ---------------------

# # try:
# #     # 1. Initialize Serial Port over USB
# #     ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
# #     time.sleep(2) # Give time for the connection to establish
# #     print(f"Connected to {SERIAL_PORT}")
# # except serial.SerialException as e:
# #     print(f"Error opening serial port: {e}")
# #     # You may need to install the driver or check the port name
# #     exit()

# # --- Detection Mode Configuration ---
# # You can switch between "UV_INK" and "HIGHLIGHTER" to test.
# DETECTION_MODE = "HIGHLIGHTER" 
# # NOTE: For UV_INK to work, you MUST illuminate the scene with a UV lamp.
# # NOTE: For HIGHLIGHTER, use normal room lighting.

# # 1. UV Ink (Fluorescence) Filter (Appears bright blue/white under UV light)
# # H: 80-140 (Blue/Purple), S: 50-255 (High Saturation), V: 150-255 (Very Bright)
# UV_LOWER_COLOR = np.array([80, 50, 150])
# UV_UPPER_COLOR = np.array([140, 255, 255])

# # 2. Green Highlighter Filter (Bright Green Fluorescence)
# # H: 30-80 (Green/Yellow-Green), S: 80-255 (High Saturation), V: 150-255 (Very Bright)
# HIGHLIGHTER_LOWER_COLOR = np.array([30, 80, 150])
# HIGHLIGHTER_UPPER_COLOR = np.array([80, 255, 255])
# # ---------------------

# def get_color_bounds(mode):
#     """Returns the appropriate HSV bounds based on the detection mode."""
#     if mode == "UV_INK":
#         return UV_LOWER_COLOR, UV_UPPER_COLOR
#     elif mode == "HIGHLIGHTER":
#         return HIGHLIGHTER_LOWER_COLOR, HIGHLIGHTER_UPPER_COLOR
#     else:
#         raise ValueError("Invalid DETECTION_MODE specified.")

# def filter_color_range(frame, lower_bound, upper_bound):
#     """Isolates a specific color range (fluorescent object) using HSV."""
#     hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
#     mask = cv2.inRange(hsv_frame, lower_bound, upper_bound)
    
#     # Morphological operations to clean up small specks and fill gaps
#     kernel = np.ones((3, 3), np.uint8)
#     mask = cv2.dilate(mask, kernel, iterations=1)
#     mask = cv2.erode(mask, kernel, iterations=1)
    
#     return mask

# def process_frame(frame, mode):
#     """Processes the image using color segmentation for the selected mode."""
#     lower, upper = get_color_bounds(mode)
    
#     # Isolate the fluorescent color
#     color_mask = filter_color_range(frame, lower, upper)
    
#     # Apply a blur for noise reduction before OCR
#     processed_image = cv2.GaussianBlur(color_mask, (5, 5), 0)
    
#     # Optional: Invert the mask if your OCR needs black text on white background
#     # processed_image = cv2.bitwise_not(processed_image)
    
#     return processed_image

# def validate_text(extracted_text, reference_text):
#     """Validates the extracted text using fuzzy matching."""
#     clean_text = "".join(c for c in extracted_text if c.isalnum() or c in [':', ' ']).strip()
#     similarity = fuzz.ratio(clean_text.lower(), reference_text.lower())
    
#     if similarity > 75: # 75% match threshold
#         return True, clean_text, similarity
#     else:
#         return False, clean_text, similarity

# # --- Main Test Loop ---
# # cap = cv2.VideoCapture(CAMERA_INDEX)
# # if not cap.isOpened():
# #     print(f"Error: Cannot open camera at index {CAMERA_INDEX}.")
# #     exit()

# # print(f"Starting OCR test in **{DETECTION_MODE}** mode. Press 'q' to quit.")
# # last_check_time = time.time()

# # display_text = "Waiting for OCR..."
# # display_color = (255, 255, 255) 

# # while True:
# #     ret, frame = cap.read()
# #     if not ret:
# #         print("Error: Failed to capture frame.")
# #         break

# #     if time.time() - last_check_time > 1.0:
        
# #         # 1. Process the frame for OCR using the selected mode
# #         processed_image = process_frame(frame, DETECTION_MODE)
        
# #         # 2. Run Tesseract OCR on the processed image
# #         config = "--psm 6" # Assumes a single block of text
# #         extracted_text = pytesseract.image_to_string(processed_image, config=config)
        
# #         # 3. Validate the text
# #         is_match, clean_text, similarity = validate_text(extracted_text, REFERENCE_TEXT)
        
# #         # 4. Update the display text
# #         display_text = f"MODE: {DETECTION_MODE} | OCR: '{clean_text}' (Sim: {similarity}%)"
# #         if is_match:
# #             display_color = (0, 255, 0) # Green
# #         else:
# #             display_color = (0, 0, 255) # Red
            
# #         last_check_time = time.time()

# #     # Draw the text on the live video frame
# #     cv2.putText(frame, display_text, 
# #                 (10, 30), # Position (top-left)
# #                 cv2.FONT_HERSHEY_SIMPLEX, 
# #                 0.7, display_color, 2)

# #     # Show the live feed and the processed image for debugging
# #     cv2.imshow("Live Feed", frame)
# #     # The processed image shows what Tesseract actually 'sees'
# #     cv2.imshow(f"Processed Image ({DETECTION_MODE})", processed_image) 

# #     if cv2.waitKey(1) & 0xFF == ord('q'):
# #         break

# # # Clean up
# # cap.release()
# # cv2.destroyAllWindows()
# # print("Test finished.")

# # --- New Serial Communication Setup ---
# try:
#     # Initialize the serial connection over USB-C
#     ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
#     time.sleep(2) # Wait for the connection to establish
#     print(f"Connected to {SERIAL_PORT}. Waiting for 'START_OCR' command.")
# except serial.SerialException as e:
#     print(f"Error opening serial port {SERIAL_PORT}: {e}")
#     print("Ensure the Arduino Nano RP2040 is plugged in and the port name is correct.")
#     raise SystemExit(1)

# # --- Camera Setup (Still needed for the OCR function) ---
# cap = cv2.VideoCapture(CAMERA_INDEX)
# if not cap.isOpened():
#     print(f"Error: Cannot open camera at index {CAMERA_INDEX}.")
#     raise SystemExit(1)

# print("Starting OCR test. Press Ctrl+C to quit.")

# # --- Serial Monitor Loop ---
# while True:
#     try:
#         # Read a line from the serial port (blocks for up to 'timeout' seconds)
#         if ser.in_waiting > 0:
#             line = ser.readline().decode('utf-8').strip()
            
#             if line == "START_OCR":
#                 print("\n[RP2040] Command received: START_OCR. Running OCR...")
                
#                 # 1. Capture and Process the frame
#                 ret, frame = cap.read()
#                 if not ret:
#                     print("Error: Failed to capture frame.")
#                     # Send a generic error response back
#                     response = "OCR_FAIL\n"
#                     ser.write(response.encode('utf-8'))
#                     continue

#                 # Use the appropriate processing function (UV_INK or HIGHLIGHTER)
#                 # Ensure DETECTION_MODE is still set in your configuration section!
#                 processed_image = process_frame(frame, DETECTION_MODE) 
                
#                 # 2. Run Tesseract OCR
#                 config = "--psm 6"
#                 extracted_text = pytesseract.image_to_string(processed_image, config=config).strip()
                
#                 # 3. Validate the text (Optional, but good for confidence check)
#                 is_match, clean_text, similarity = validate_text(extracted_text, REFERENCE_TEXT)

#                 # 4. Prepare and Send Response
#                 # Send the clean text followed by a newline (\n)
#                 final_response = f"{clean_text}\n" 
                
#                 ser.write(final_response.encode('utf-8'))
#                 print(f"[RP2040] Response sent: '{clean_text}' (Sim: {similarity}%)")

#                 # Optional: Show the processed image briefly for debugging
#                 cv2.imshow("Processed Image for OCR", processed_image)
#                 cv2.waitKey(1000) # Show for 1 second
#                 cv2.destroyAllWindows()
            
#             # Print any other serial messages from the RP2040
#             elif line:
#                 print(f"[RP2040]: {line}")

#     except KeyboardInterrupt:
#         break
#     except serial.SerialException:
#         print("\n[ERROR] Serial connection lost.")
#         break
    
#     # Small delay to prevent high CPU usage
#     time.sleep(0.05)

# # --- Clean up ---
# print("Test finished. Cleaning up.")
# cap.release()
# cv2.destroyAllWindows()
# ser.close()
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
# IMPORTANT: Replace 'COM_PORT_NAME' with the actual port name
SERIAL_PORT = 'COM6' # Adjust this for your OS
BAUD_RATE = 115200 
# --- Configuration ---

# --- Detection Mode Configuration ---
DETECTION_MODE = "HIGHLIGHTER" 

# 1. UV Ink (Fluorescence) Filter (Appears bright blue/white under UV light)
UV_LOWER_COLOR = np.array([80, 50, 150])
UV_UPPER_COLOR = np.array([140, 255, 255])

# 2. Green Highlighter Filter (Bright Green Fluorescence)
HIGHLIGHTER_LOWER_COLOR = np.array([30, 80, 150])
HIGHLIGHTER_UPPER_COLOR = np.array([80, 255, 255])
# ---------------------

def get_color_bounds(mode):
    """Returns the appropriate HSV bounds based on the detection mode."""
    if mode == "UV_INK":
        return UV_LOWER_COLOR, UV_UPPER_COLOR
    elif mode == "HIGHLIGHTER":
        return HIGHLIGHTER_LOWER_COLOR, HIGHLIGHTER_UPPER_COLOR
    else:
        raise ValueError("Invalid DETECTION_MODE specified.")

def filter_color_range(frame, lower_bound, upper_bound):
    """Isolates a specific color range (fluorescent object) using HSV."""
    hsv_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv_frame, lower_bound, upper_bound)
    
    # Morphological operations to clean up small specks and fill gaps
    kernel = np.ones((3, 3), np.uint8)
    mask = cv2.dilate(mask, kernel, iterations=1)
    mask = cv2.erode(mask, kernel, iterations=1)
    
    return mask

def process_frame(frame, mode):
    """Processes the image using color segmentation for the selected mode."""
    lower, upper = get_color_bounds(mode)
    
    # Isolate the fluorescent color
    color_mask = filter_color_range(frame, lower, upper)
    
    # Apply a blur for noise reduction before OCR
    processed_image = cv2.GaussianBlur(color_mask, (5, 5), 0)
    
    return processed_image

def validate_text(extracted_text, reference_text):
    """Validates the extracted text using fuzzy matching."""
    # Clean text to keep only alphanumeric characters and common separators
    clean_text = "".join(c for c in extracted_text if c.isalnum() or c in [':', ' ']).strip()
    similarity = fuzz.ratio(clean_text.lower(), reference_text.lower())
    
    if similarity > 75: # 75% match threshold
        return True, clean_text, similarity
    else:
        return False, clean_text, similarity

# --- Initial Setup ---
try:
    # 1. Initialize Serial Port over USB
    ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=0.05) # Lower timeout for responsiveness
    time.sleep(2) # Give time for the connection to establish
    print(f"Connected to {SERIAL_PORT}")
except serial.SerialException as e:
    print(f"Error opening serial port {SERIAL_PORT}: {e}")
    print("Ensure the device is plugged in and the port name is correct.")
    sys.exit(1)

# --- Camera Setup ---
cap = cv2.VideoCapture(CAMERA_INDEX)
if not cap.isOpened():
    print(f"Error: Cannot open camera at index {CAMERA_INDEX}.")
    ser.close()
    sys.exit(1)

print(f"Starting OCR monitor in **{DETECTION_MODE}** mode. Press 'q' in the video window or Ctrl+C to quit.")

# Variables for the live video feedback
last_check_time = time.time()
display_text = "Waiting for OCR command..."
display_color = (255, 255, 255) # White
processed_image = None # Initialize outside the loop

# --- Main Test Loop (Live Video and Serial Monitor) ---
while True:
    try:
        ret, frame = cap.read()
        if not ret:
            print("Error: Failed to capture frame.")
            break

        # --- Serial Communication Check ---
        # Check for commands from the RP2040
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            
            if line == "START_OCR":
                print("\n[RP2040] Command received: START_OCR. Running OCR...")
                
                # ... (Capture frame and process_frame calls remain the same) ...
                ret_ocr, frame_ocr = cap.read()
                if not ret_ocr:
                    print("Error: Failed to capture frame for OCR.")
                    response = "OCR_FAIL\n"
                    ser.write(response.encode('utf-8'))
                    continue

                processed_image = process_frame(frame_ocr, DETECTION_MODE) 
                
                # 2. Run Tesseract OCR
                config = "--psm 6"
                extracted_text = pytesseract.image_to_string(processed_image, config=config).strip()
                
                # 3. Validate the text
                is_match, clean_text, similarity = validate_text(extracted_text, REFERENCE_TEXT)

                # 4. Prepare and Send Response ONLY IF IT'S A MATCH
                if is_match:
                    # Send the clean text (e.g., "MIX 9:5:2")
                    final_response = f"{REFERENCE_TEXT}\n" 
                    ser.write(final_response.encode('utf-8'))
                    
                    # Update display feedback
                    display_color = (0, 255, 0) # Green
                    display_text = f"SUCCESS! Sent: '{REFERENCE_TEXT}' (OCR: '{clean_text}', Sim: {similarity}%)"
                    print(f"[RP2040] Response sent: '{clean_text}' (Sim: {similarity}%)")
                else:
                    # Do NOT send anything back to the serial port.
                    # The RP2040 will wait for its timeout.
                    # Update display feedback for failure
                    display_color = (0, 0, 255) # Red
                    display_text = f"FAILURE! OCR: '{clean_text}' (Sim: {similarity}%) - Not sending."
                    print(f"[RP2040] **Validation Failed**! Text: '{clean_text}' (Sim: {similarity}%) - Response NOT sent.")
                
                # Optional: Show the processed image when OCR runs
                cv2.imshow(f"Processed Image ({DETECTION_MODE})", processed_image)
            
            # Print any other serial messages from the RP2040
            elif line:
                print(f"[RP2040]: {line}")

        # --- Live Video Display ---
        # Draw the status text on the live video frame
        cv2.putText(frame, display_text, 
                    (10, 30), # Position (top-left)
                    cv2.FONT_HERSHEY_SIMPLEX, 
                    0.7, display_color, 2)

        # Show the live feed
        cv2.imshow("Live Feed | Press 'q' to quit", frame)

        # Check for a keyboard press ('q' or ESC) to quit
        key = cv2.waitKey(1) & 0xFF
        if key == ord('q') or key == 27: # 27 is ESC key
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