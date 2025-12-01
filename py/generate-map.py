

import matplotlib.pyplot as pp
import numpy as np
import sys
import cv2

def generate_map(file, output_file):
    img = pp.imread(file)
    res = img.copy()

    of = open("./output/coords.txt", encoding='utf-8', mode='r').readlines()
    points = []
    
    if (len(of) == 0):
        print("not coords")
        exit(0)
    
    for s in of:
        s = s.replace('\n', '').split(' ')
        points.append(
            [int(s[0]), int(s[1])]
        )
    points = np.array(points)
    
    for i in range(len(points) - 1):
        pt1 = tuple(points[i])
        pt2 = tuple(points[i + 1]) 
        
        cv2.line(res, pt1, pt2, (255, 0, 0), 2)
    
    for point in points:
        x, y = point
        cv2.circle(res, (x, y), 4, (0, 0, 255), -1)
    
    pp.imshow(res)
    pp.show()
    
    pp.imsave("./output/path.png", res)
    
def main():
    if len(sys.argv) != 2:
        print("""
Usage: python main,py <input_map_file>
Example: python generate-map.py ./input/karta-01.bmp
        """)
        sys.exit(0)
    else:
        create_map(sys.argv[1])
        sys.exit(0)

if __name__ == "__main__":
    main()
