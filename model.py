# This file contains your text validation logic
from fuzzywuzzy import fuzz


REFERENCE_TEXT = "MIX 9:5:2"

def validate_text(extracted_text):
   
   
    clean_text = "".join(c for c in extracted_text if c.isalnum() or c in [':', ' '])
    
    similarity = fuzz.ratio(clean_text.lower(), REFERENCE_TEXT.lower())
    
    print(f"OCR Result: '{clean_text}', Similarity: {similarity}%")
    
    if similarity > 75: # Set a 75% threshold for a match
        return True
    else:
        return False