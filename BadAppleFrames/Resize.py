import os
from PIL import Image

def resize_images(input_folder, output_folder, target_width = 64, target_height = 48):
    if not os.path.exists(output_folder):
        os.makedirs(output_folder)

    for filename in os.listdir(input_folder):
        input_path = os.path.join(input_folder, filename)
        if os.path.isfile(input_path):
            try:
                image = Image.open(input_path)
                resized_image = image.resize((target_width, target_height), Image.ANTIALIAS)
                output_path = os.path.join(output_folder, filename)
                resized_image.save(output_path)
                print(f"Resized and saved {filename}")
            except Exception as e:
                print(f"Error processing {filename}: {e}")

if __name__ == "__main__":
    input_folder = "Input"
    output_folder = "Output"
    resize_images(input_folder, output_folder)