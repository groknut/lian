
import matplotlib.pyplot as pp
import numpy as np
import sys

def load_points(coords):
    with open(coords, 'r') as f:
    	points = np.array([list(map(int, line.split())) for line in f if line.strip()])
    return points

def generate_map(file, coords ,output_file):

    img = pp.imread(file)

    fig, ax = pp.subplots()
    ax.imshow(img)
    
    points = load_points(coords)

    if len(points) == 0:
        print("no coords")
        exit(0)
    
    for i in range(len(points)-1):
    	ax.plot([points[i,0], points[i+1,0]], [points[i,1], points[i+1,1]], 'r-', lw=2)

    for x, y in points:
    	ax.add_patch(pp.Circle((x, y), 4, color='blue', fill=True))

    pp.show()
    fig.savefig(output_file, bbox_inches='tight')
    print(f"✅ Saved: {output_file}")

    
def main():
    if len(sys.argv) != 4:
        print("""
Usage: python generate_map.py <input_map_file> <file_coords>  <output_map_file>
Example: python generate_map.py ./input/karta-01.bmp ./output/coords.txt ./output/file.png
        """)
        sys.exit(0)
    else:
        generate_map(sys.argv[1], sys.argv[2], sys.argv[3])
        sys.exit(0)

if __name__ == "__main__":
    main()
