import sys

from dataclasses import dataclass, field
from pathlib import Path
from PIL import Image
import numpy as np

IMAGE = Path(__file__).parent / "Font.png"
BLANK = " "
FILL = "█"

@dataclass
class Glyph:
    data: list[int] = field(default_factory=list)  # 8x8

    def __str__(self):
        result = f"Glyph: '{self.char}' (width: {self.width}, height: {self.height}, y_offset: {self.y_offset})\n"
        for row in self.data:
            row_str = ""
            for pixel in row:
                row_str += FILL if pixel else BLANK
            result += row_str + "\n"
        return result


def extract_glyphs(filename: str) -> list[list[int]]:
    """Extract individual characters from the font image."""
    try:
        # Load the image
        img = Image.open(filename)

        # Convert to grayscale and then binary
        img_gray = img.convert("L")
        img_array = np.array(img_gray)

        # Convert to binary (0 for black, 1 for white)
        # Assuming white pixels are the font and black is background
        binary_array = (img_array > 128).astype(int)

        # Try to find character boundaries by looking for consistent patterns
        height, width = binary_array.shape

        # Let's try to extract 8x8 characters assuming they're arranged in a grid
        # We'll look for the first character (should be space or A)
        chars = []

        # Try to extract in 8x8 blocks
        char_width = 8
        char_height = 8

        # Calculate how many characters fit
        chars_per_row = width // char_width
        char_rows = height // char_height

        print(f"\nTrying to extract {chars_per_row}x{char_rows} characters")

        for row in range(char_rows):
            for col in range(chars_per_row):
                # Extract 8x8 block
                start_y = row * char_height
                end_y = start_y + char_height
                start_x = col * char_width
                end_x = start_x + char_width

                if end_y <= height and end_x <= width:
                    char_data = 1 - binary_array[start_y:end_y, start_x:end_x]
                    chars.append(char_data)

        return chars

    except Exception as e:
        print(f"Error extracting characters: {e}")
        return []


def save_characters_to_image(
    chars: list[Glyph],
    output_path: str,
    char_width: int = 8,
    char_height: int = 8,
    chars_per_row: int = 16,
):
    """Save the extracted characters to a new image."""
    if not chars:
        print("No characters to save.")
        return

    # Calculate the number of rows needed
    total_chars = len(chars)
    rows = (total_chars + chars_per_row - 1) // chars_per_row

    # Create a blank image
    output_width = chars_per_row * char_width
    output_height = rows * char_height
    output_image = Image.new("L", (output_width, output_height), color=255)

    # Paste each character into the output image
    for i, char in enumerate(chars):
        row = i // chars_per_row
        col = i % chars_per_row
        start_x = col * char_width
        start_y = row * char_height

        char_image = Image.fromarray((char.data * 255).astype("uint8"), mode="L")
        output_image.paste(char_image, (start_x, start_y))

    # Save the output image
    output_image.save(output_path)
    print(f"Saved characters to {output_path}")


if __name__ == "__main__":
    glyph_byte_size = sys.getsizeof(Glyph)
    glyphs = extract_glyphs(IMAGE)

    # Save the extracted characters to a new image
    # output_path = Path(__file__).parent / "image_out.png"
    # save_characters_to_image(glyphs, output_path, chars_per_row=8)

    output_data_path = Path(__file__).parent / "font.dat"
    with open (output_data_path, "wb") as f:
        for glyph in glyphs:
            for row in glyph:
                byte = 0
                for col_index, col in enumerate(row):
                    byte = (byte << 1) | int(col)
                    if (col_index + 1) % 8 == 0:  # Write the byte after every 8 bits
                        f.write(byte.to_bytes(1))
                        byte = 0
    
