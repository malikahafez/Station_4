try:
    import cv2
    import numpy as np
except ImportError:
    print("Error: 'cv2' (opencv-python) or 'numpy' is not installed. Install with: python -m pip install opencv-python numpy")
    raise SystemExit(1)

import pytesseract
from fuzzywuzzy import fuzz
import time

# --- Configuration ---
pytesseract.pytesseract.tesseract_cmd = r'C:\Program Files\Tesseract-OCR\tesseract.exe' 
REFERENCE_TEXT = "MIX 9:5:2"
CAMERA_INDEX = 0 

# --- Detection Mode Configuration ---
# You can switch between "UV_INK" and "HIGHLIGHTER" to test.
DETECTION_MODE = "HIGHLIGHTER" 
# NOTE: For UV_INK to work, you MUST illuminate the scene with a UV lamp.
# NOTE: For HIGHLIGHTER, use normal room lighting.

# 1. UV Ink (Fluorescence) Filter (Appears bright blue/white under UV light)
# H: 80-140 (Blue/Purple), S: 50-255 (High Saturation), V: 150-255 (Very Bright)
UV_LOWER_COLOR = np.array([80, 50, 150])
UV_UPPER_COLOR = np.array([140, 255, 255])

# 2. Green Highlighter Filter (Bright Green Fluorescence)
# H: 30-80 (Green/Yellow-Green), S: 80-255 (High Saturation), V: 150-255 (Very Bright)
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
    
    # Optional: Invert the mask if your OCR needs black text on white background
    # processed_image = cv2.bitwise_not(processed_image)
    
    return processed_image

def validate_text(extracted_text, reference_text):
    """Validates the extracted text using fuzzy matching."""
    clean_text = "".join(c for c in extracted_text if c.isalnum() or c in [':', ' ']).strip()
    similarity = fuzz.ratio(clean_text.lower(), reference_text.lower())
    
    if similarity > 75: # 75% match threshold
        return True, clean_text, similarity
    else:
        return False, clean_text, similarity

# --- Main Test Loop ---
cap = cv2.VideoCapture(CAMERA_INDEX)
if not cap.isOpened():
    print(f"Error: Cannot open camera at index {CAMERA_INDEX}.")
    exit()

print(f"Starting OCR test in **{DETECTION_MODE}** mode. Press 'q' to quit.")
last_check_time = time.time()

display_text = "Waiting for OCR..."
display_color = (255, 255, 255) 

while True:
    ret, frame = cap.read()
    if not ret:
        print("Error: Failed to capture frame.")
        break

    if time.time() - last_check_time > 1.0:
        
        # 1. Process the frame for OCR using the selected mode
        processed_image = process_frame(frame, DETECTION_MODE)
        
        # 2. Run Tesseract OCR on the processed image
        config = "--psm 6" # Assumes a single block of text
        extracted_text = pytesseract.image_to_string(processed_image, config=config)
        
        # 3. Validate the text
        is_match, clean_text, similarity = validate_text(extracted_text, REFERENCE_TEXT)
        
        # 4. Update the display text
        display_text = f"MODE: {DETECTION_MODE} | OCR: '{clean_text}' (Sim: {similarity}%)"
        if is_match:
            display_color = (0, 255, 0) # Green
        else:
            display_color = (0, 0, 255) # Red
            
        last_check_time = time.time()

    # Draw the text on the live video frame
    cv2.putText(frame, display_text, 
                (10, 30), # Position (top-left)
                cv2.FONT_HERSHEY_SIMPLEX, 
                0.7, display_color, 2)

    # Show the live feed and the processed image for debugging
    cv2.imshow("Live Feed", frame)
    # The processed image shows what Tesseract actually 'sees'
    cv2.imshow(f"Processed Image ({DETECTION_MODE})", processed_image) 

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Clean up
cap.release()
cv2.destroyAllWindows()
print("Test finished.")