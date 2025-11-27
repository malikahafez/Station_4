import cv2
import pytesseract
import serial
import time
from model import validate_text


# This is the USB port your RP2040 is plugged into.
SERIAL_PORT = '/dev/ttyACM0' 
BAUD_RATE = 115200
CAMERA_INDEX = 0 # 0 is usually the default webcam or PiCamera
# ---------------------

def setup_serial():
    
    while True:
        try:
            ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)
            print(f"Connected to RP2040 on {SERIAL_PORT}")
            return ser
        except serial.SerialException:
            print(f"RP2040 not found on {SERIAL_PORT}. Retrying in 2 seconds...")
            time.sleep(2)

def process_frame(frame):
    
    # Convert to grayscale
    gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # Apply a Gaussian blur to reduce noise
    blurred_frame = cv2.GaussianBlur(gray_frame, (5, 5), 0)
    
    # Use adaptive thresholding to get clear black-on-white text
    # This is critical for varied lighting like a UV light
    thresh = cv2.adaptiveThreshold(blurred_frame, 255,
                                   cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
                                   cv2.THRESH_BINARY, 11, 2)
    return thresh

def perform_ocr():
  
    print("OCR_SCAN: Capturing image...")
    # Open the camera
    cap = cv2.VideoCapture(CAMERA_INDEX)
    if not cap.isOpened():
        print("Error: Cannot open camera.")
        return None

    # Let the camera auto-adjust for a moment
    time.sleep(1) 
    
    ret, frame = cap.read()
    cap.release() # Release camera immediately
    
    if not ret:
        print("Error: Failed to capture frame.")
        return None

    # Process the image for better OCR
    processed_image = process_frame(frame)
    
    # Use pytesseract to find text in the image
    print("OCR_SCAN: Performing OCR...")
    extracted_text = pytesseract.image_to_string(processed_image)
    
    # Validate the text
    clean_text = "".join(c for c in extracted_text if c.isalnum() or c in [':', ' ']).strip()
    
    if clean_text:
        print(f"OCR_SCAN: Extracted text: '{clean_text}'")
        return clean_text
    else:
        print("OCR_SCAN: No text found.")
        return None
def main_loop(ser):
    
    print("OCR Server is running. Waiting for 'START_OCR' command...")
    while True:
        if ser.in_waiting > 0:
            try:
                message = ser.readline().decode('utf-8').strip()
                
                if message == "START_OCR":
                    print("MESSAGE_IN: 'START_OCR' received.")
                    
                    # Run the OCR process
                    clue = perform_ocr()
                    
                    if clue:
                      
                        ser.write(f"CLUE:{clue}\n".encode('utf-8'))
                        print(f"MESSAGE_OUT: CLUE:{clue}")
                    else:
                        # Tell the RP2040 the scan failed
                        ser.write(b"ERROR:OCR_FAILED\n")
                        print("MESSAGE_OUT: ERROR:OCR_FAILED")
                        
            except Exception as e:
                print(f"Error in main loop: {e}")
                
        time.sleep(0.1) # Don't fry the CPU

if __name__ == "__main__":
    ser = setup_serial()
    if ser:
        main_loop(ser)