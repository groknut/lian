
import numpy as np

def load_points(coords):
    with open(coords, 'r') as f:
    	points = np.array([list(map(int, line.split())) for line in f if line.strip()])
    return points


