import matplotlib.pyplot as pp
import sys
from matplotlib.animation import FuncAnimation

from config import load_points

def animate_path(image, coords):

    img = pp.imread(image)
    points = load_points(coords)

    fig, ax = pp.subplots()
    ax.imshow(img)

    line, = ax.plot([], [], 'r-', lw=2)
    dot, = ax.plot([], [], 'blue', ms=6)

    for x, y in points:
        ax.add_patch(
            pp.Circle((x, y), 4, color="blue", fill=True)
        )

    def anim(frame):
        if frame < len(points):
            line.set_data(points[:frame+1, 0], points[:frame+1, 1])
            dot, = ax.plot([], [], 'blue', ms=8)
        return line

    dot.set_data([], [])
    ani = FuncAnimation(
        fig, 
        anim, 
        frames=len(points),
        init_func=lambda: (line.set_data([], []), dot.set_data([], [])),
        interval=200
    )

    return ani


def main():
    if len(sys.argv) != 3:
        print(f"""
Usage: python {sys.argv[0]} <input_map_file> <file_coords>
Example: python {sys.argv[0]} ./input/karta-01.bmp ./output/coords.txt
        """)
        sys.exit(0)
    else:
        ani = animate_path(sys.argv[1], sys.argv[2])
        pp.show()
        sys.exit(0)

if __name__ == "__main__":
    main()
