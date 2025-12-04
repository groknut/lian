
import configparser
import matplotlib.pyplot as pp
import numpy as np
from pathlib import Path
import sys
import subprocess as sp
import os
import functools
import time
from matplotlib.animation import FuncAnimation

def timeit(func):
	@functools.wraps(func)
	def wrapper(*args, **kwargs):
		start = time.perf_counter()
		result = func(*args, **kwargs)
		print(f"{func.__name__}: {time.perf_counter() - start:.4f}с")
		return result
	return wrapper

class PathFinderApp:
	def __init__(self, config_file="config.ic"):
		self.config_file = Path(config_file)
		self.config = configparser.ConfigParser(
			delimiters=[':'],
			comment_prefixes=[';'],  # строка начинается с ;
			inline_comment_prefixes=[';']  # комментарий в конце строки
		)

		self.load_config()

		self.points = []

		self.img = None
		self.image_path, self.map_path, self.output_file, self.coords = [None] * 4
		self.cpp_app = None

		self.fig, self.ax = [None] * 2

		self.bindings = {
				"save": ['c', 'C'],
				'reset': ['r', 'R']
			}
			
		self.point_colors = {
			'start': 'green',
			'goal': 'red' 
		}

		self.animation = {
			"enabled": False,
			"save_to_file": False
		}

		self.os_name = os.name

		self.init_from_config()

	def load_config(self):
		if self.config_file.exists():
			self.config.read(self.config_file)
			return {
				sect: dict(self.config.items(sect)) for sect in self.config.sections()
			}
		return {}

	def init_from_config(self):
		self.image_path = self.config['input'].get('image', './input/karta-01.bmp')
		self.map_path = self.config['input'].get('map', './input/map.txt')
		if not self.image_path or not Path(self.image_path).exists():
			print("Image not found")
			return False
		self.img = pp.imread(self.image_path)
		self.coords = self.config['output'].get('coords', './output/coords.txt')
		self.output_file = self.config['output'].get('output_file', './output/path.png')

		self.animation["enabled"] = self.config['animation'].get("enabled", "false") == "true"
		self.animation["save_to_file"] = self.config['animation'].get("save_to_file", "false") == "true"

		self.cpp_app = self.config['app'].get("name", "main")
		
		
	@timeit
	def create_map(self):
   
		gray = self.img.mean(axis = 2)
		binary = gray < 0.05

		if self.map_path:
			f = open(self.map_path, encoding='utf-8', mode='w')
			f.write(f"{binary.shape[1]} {binary.shape[0]}\n")
			for i in range(binary.shape[0]):
				for j in range(binary.shape[1]):
					f.write(str(int(binary[i][j])))
				f.write('\n')
			print(f"Load map in: {self.map_path}")
			return True
		return False
		
	@timeit
	def gui(self):
		if not self.image_path or not Path(self.image_path).exists():
			print("Image not found")
			return

		self.fig, self.ax = pp.subplots()
		self.ax.imshow(self.img)

		self.fig.canvas.mpl_connect('button_press_event', self.on_click)
		self.fig.canvas.mpl_connect('key_press_event', self.on_key)
		
		self.ax.set_xlabel("Simple GUI for MAP", fontsize=10, labelpad=10)
		
		pp.tight_layout()
		pp.show()

	def on_click(self, event):
		if event.inaxes != self.ax:
			return

		x, y = int(event.xdata), int(event.ydata)

		if len(self.points) >= 2:
			self.points.clear()
			self.ax.clear()
			self.ax.imshow(self.img)
			self.fig.canvas.draw()

		self.points.append((x, y))
		color = self.point_colors['start'] if len(self.points) == 1 else self.point_colors['goal']
		
		self.ax.plot(x, y, 'o', color=color, markersize=8)

		self.fig.canvas.draw()

	def on_key(self, event):
	
		if event.key in self.bindings['reset']:
			self.points.clear()
			self.ax.clear()
			self.ax.imshow(self.img)
			self.fig.canvas.draw()

		if event.key in self.bindings['save'] and len(self.points) == 2:
				self.update_config(
					'lian', {
						'start': self.points[0],
						'goal': self.points[1]
					}
				)
				self.ax.set_xlabel("Save points", fontsize=10, labelpad=10)
				pp.close()
		

	def update_config(self, section, updates):
			   
		if section not in self.config:
			self.config[section] = {}
		
		for key, value in updates.items():
			self.config[section][key] = str(value)
		
		with open(self.config_file, 'w') as f:
			self.config.write(f, space_around_delimiters=False)
			
	@timeit
	def compile_cpp(self):
		exe_path = str(Path(self.cpp_app).with_suffix('.exe'))  if os.name == "nt" else str(Path(self.cpp_app))
		if not os.path.exists(exe_path):
			sp.run(f"g++ main.cpp src/*.cpp -o {self.cpp_app}", shell=True)
			
	@timeit
	def start_cpp(self):
		exe_path = str(Path(self.cpp_app).with_suffix('.exe'))  if os.name == "nt" else str(Path(self.cpp_app))
		sp.run(
			f"{exe_path} {self.config_file}", shell=True
		)

	def _load_points(self):
		with open(self.coords, 'r') as f:
			points = np.array([list(map(int, line.split())) for line in f if line.strip()])
		return points

	def _animate_path(self, points):
		line, = self.ax.plot([], [], 'r-', lw=2)
		dot, = self.ax.plot([], [], 'blue', ms=8)

		for x, y in points:
			self.ax.add_patch(pp.Circle((x, y), 4, color='blue', fill=True))
		
		def anim(frame):
			if frame < len(points):
				line.set_data(points[:frame+1, 0], points[:frame+1, 1])
				dot, = self.ax.plot([], [], 'blue', ms=8)
			return line
		dot.set_data([], [])
		ani = FuncAnimation(self.fig, anim, frames=len(points),
		init_func=lambda: (line.set_data([], []), dot.set_data([], [])),
		interval=200, repeat=False)

		if self.animation["save_to_file"]:
			gif_path = str(Path(self.output_file).with_suffix('.gif'))
			ani.save(gif_path, writer='pillow', fps=10, dpi=100)

		return ani

	def _draw_final_path(self, points):
		for i in range(len(points)-1):
			self.ax.plot([points[i,0], points[i+1,0]], [points[i,1], points[i+1,1]], 'r-', lw=2)

		for x, y in points:
			self.ax.add_patch(pp.Circle((x, y), 4, color='blue', fill=True))

	@timeit
	def vizualize(self):
		points = self._load_points()
		
		self.fig, self.ax = pp.subplots()
		self.ax.imshow(self.img)

		if self.animation["enabled"]:
			ani = self._animate_path(points)
			pp.show()

			self.ax.clear()
			self.ax.imshow(self.img)
			self._draw_final_path(points)

		else:
			self._draw_final_path(points)

		self.fig.savefig(self.output_file, bbox_inches='tight')
		print(f"✅ Saved: {self.output_file}")

		if not self.animation["enabled"]:
			pp.show()
			
	def run(self):
		print("🚀 Start pipeline...")
		
		# 1. Создание карты
		print("\n1. Create map...")
		if self.create_map():
			print("✅ Map was create")
		else:
			print("⚠ Map wasn't create")
		
		# 2. Запуск GUI
		print("\n2. Start GUI...")
		self.gui()
		
		# 3. Проверяем сохранены ли точки
		if len(self.points) == 2:
			print("\n🎯 Start algorithm")
		else:
			print("\n⚠ No points selected")
			exit(0)

		# 4. Компиляция при необходимости
		self.compile_cpp()

		# 5. запуск алгоритм
		self.start_cpp()

		# визуализация результатов 
		self.vizualize()

if len(sys.argv) == 2:
	app = PathFinderApp(sys.argv[1])
	app.run()
else:
	app = PathFinderApp()
	app.run()
	
