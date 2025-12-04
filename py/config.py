
import numpy as np
import functools
import time

def timeit(func):
	@functools.wraps(func)
	def wrapper(*args, **kwargs):
		start = time.perf_counter()
		result = func(*args, **kwargs)
		print(f"{func.__name__}: {time.perf_counter() - start:.4f}с")
		return result
	return wrapper

def load_points(coords):
    with open(coords, 'r') as f:
    	points = np.array([list(map(int, line.split())) for line in f if line.strip()])
    return points


