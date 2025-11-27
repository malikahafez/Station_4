try:
    import cv2
except ImportError:
    print("Error: 'cv2' (opencv-python) is not installed. Install it with: python -m pip install opencv-python")
    raise SystemExit(1)

import pytesseract
from fuzzywuzzy import fuzz
import time

# --- Configuration ---
pytesseract.pytesseract.tesseract_cmd = r'C:\Program Files\Tesseract-OCR\tesseract.exe'  # Add this line
REFERENCE_TEXT = "MIX 9:5:2"
CAMERA_INDEX = 0 # 0 is usually your laptop's built-in webcam
# ---------------------

def process_frame(frame):
    """Pre-processes the image to make it easier for OCR to read."""
    # Convert to grayscale
    gray_frame = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    
    # Apply a Gaussian blur to reduce noise
    blurred_frame = cv2.GaussianBlur(gray_frame, (5, 5), 0)
    
    # Use adaptive thresholding to get clear black-on-white text
    thresh = cv2.adaptiveThreshold(blurred_frame, 255,
                                   cv2.ADAPTIVE_THRESH_GAUSSIAN_C,
                                   cv2.THRESH_BINARY, 11, 2)
    return thresh

def validate_text(extracted_text, reference_text):
    """
    Checks if the extracted text is a close match to the reference text.
    Uses fuzzy string matching to account for OCR errors (e.g., S vs 5, O vs 0).
    """
    # Clean up the text
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

print("Starting OCR test. Press 'q' to quit.")
last_check_time = time.time()

# Initialize display variables before the loop
display_text = "Waiting for OCR..."
display_color = (255, 255, 255)  # White

while True:
    ret, frame = cap.read()
    if not ret:
        print("Error: Failed to capture frame.")
        break

    # We only run the OCR once per second to avoid lagging the video feed
    if time.time() - last_check_time > 1.0:
        
        # 1. Process the frame for OCR
        processed_image = process_frame(frame)
        
        # 2. Run Tesseract OCR on the processed image
        # --psm 6 assumes a single uniform block of text
        config = "--psm 6"
        extracted_text = pytesseract.image_to_string(processed_image, config=config)
        
        # 3. Validate the text
        is_match, clean_text, similarity = validate_text(extracted_text, REFERENCE_TEXT)
        
        # 4. Update the display text
        display_text = f"OCR: '{clean_text}' (Sim: {similarity}%)"
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

    # Show the live feed
    cv2.imshow("Laptop OCR Test", frame)

    # Quit when 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Clean up
cap.release()
cv2.destroyAllWindows()
print("Test finished.")


