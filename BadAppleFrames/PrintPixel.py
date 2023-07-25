from PIL import Image
import os

AllFrameData = []
NumOfBits = 16

FrameToSkip = 3

def dump_2d_array_to_file(array, filename):
    with open(filename, "w") as file:
        rows = len(array)
        cols = len(array[0])

        file.write("{\n")

        for i in range(rows):
            file.write("    {")
            for j in range(cols):
                file.write(str(array[i][j]))
                if j != cols - 1:
                    file.write(", ")
            file.write("}")
            if i != rows - 1:
                file.write(",")
            file.write("\n")

        file.write("};\n")

def convert_rgb_to_bw(image_path):
    try:
        image = Image.open(image_path)
        width, height = image.size

        CurrentFrameData = []
        CurrentNumber = 0

        for y in range(height):
            for x in range(width):
                pixel_rgb = image.getpixel((x, y))
                is_white = all(value > 127 for value in pixel_rgb)
                if(is_white):
                    CurrentNumber = CurrentNumber | (1 << ((NumOfBits - 1) - (x % NumOfBits)))
                if((x % NumOfBits) == (NumOfBits - 1)):
                    CurrentFrameData.append(CurrentNumber)
                    CurrentNumber = 0
        
        AllFrameData.append(CurrentFrameData)

    except Exception as e:
        print(f"Error: {e}")

def process_images_in_folder(folder_path):
    CurrentFrame = 0
    if not os.path.exists(folder_path):
        print(f"Error: Folder '{folder_path}' does not exist.")
        return

    for filename in os.listdir(folder_path):
        image_path = os.path.join(folder_path, filename)
        if os.path.isfile(image_path):
            if(CurrentFrame % FrameToSkip != 0):
                print(f"Processing image: {filename}")
                convert_rgb_to_bw(image_path)
            CurrentFrame += 1

if __name__ == "__main__":
    input_folder = "Output"
    process_images_in_folder(input_folder)
    output_file = "bitmaps.txt"
    dump_2d_array_to_file(AllFrameData, output_file)
    print(len(AllFrameData), len(AllFrameData[0]))