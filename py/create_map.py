import matplotlib.pyplot as pp
import numpy as np
import sys

def create_map(input_file, output_file):
    img = pp.imread(input_file)
    print("Image shape:", img.shape)

    gray = img.mean(axis = 2)
    binary = gray < 0.05

    pp.imshow(binary)
    pp.show()

    f = open(output_file, encoding='utf-8', mode='w')
    f.write(f"{binary.shape[1]} {binary.shape[0]}\n")
    for i in range(binary.shape[0]):
        for j in range(binary.shape[1]):
            f.write(str(int(binary[i][j])))
        f.write('\n')

def main():
    if len(sys.argv) != 3:
        print("""
Usage: python main,py <input_map_file> <output_file>
Example: python create-map.py ./input/karta-01.bmp ./output/map.txt
        """)
        sys.exit(0)
    else:
        create_map(sys.argv[1], sys.argv[2])
        sys.exit(0)

if __name__ == "__main__":
    main()
