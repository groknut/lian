
import configparser
import matplotlib.pyplot as pp
import numpy as np
from pathlib import Path
import sys
import subprocess as sp
import os

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

        self.fig, self.ax = [None] * 2

        self.bindings = {
                "save": ['c', 'C'],
                'reset': ['r', 'R']
            }
            
        self.point_colors = {
            'start': 'green',
            'goal': 'red' 
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
        """Обновляет любые значения в указанной секции"""
               
        if section not in self.config:
            self.config[section] = {}
        
        for key, value in updates.items():
            self.config[section][key] = str(value)
        
        with open(self.config_file, 'w') as f:
            self.config.write(f, space_around_delimiters=False)

    def compile_cpp(self):
        exe_name = "main.exe" if self.os_name == "nt" else "main"
        if not os.path.exists(exe_name):
            sp.run("g++ main.cpp src/*.cpp -o main", shell=True)

    def start_cpp(self):
        if self.os_name == "nt":
            sp.run(f"main.exe {self.config_file}", shell=True)
        elif self.os_name == "posix":
            sp.run(f"main {self.config_file}", shell=True)
        else:
            print("Your os not supported")

    def vizualize(self):
        res = self.img.copy()

        with open(self.coords, 'r', encoding='utf-8') as f:
            lines = f.readlines()
              
        if not lines:
            print("not coords")
            exit(0)

        points = []
        
        for l in lines:
            l = l.strip()
            if l:
                x, y = map(int, l.split())
                points.append([x, y])

        points = np.array(points)

        self.fig, self.ax = pp.subplots()
        self.ax.imshow(res)       

        for i in range(len(points) - 1):
            x1, y1 = tuple(points[i])
            x2, y2 = tuple(points[i + 1])            

            self.ax.plot([x1, x2], [y1, y2], color='red', linewidth=2)

        for point in points:
            x, y = point
            circle = pp.Circle((x, y), 4, color='blue', fill=True)
            self.ax.add_patch(circle)

        pp.savefig(self.output_file)
        pp.show()
            
    def run(self):
        """Запуск всего пайплайна"""
        print("🚀 Запуск пайплайна...")
        
        # 1. Создание карты
        print("\n1. Создание карты...")
        if self.create_map():
            print("✅ Карта создана")
        else:
            print("⚠ Не удалось создать карту")
        
        # 2. Запуск GUI
        print("\n2. Запуск GUI...")
        self.gui()
        
        # 3. Проверяем сохранены ли точки
        if len(self.points) == 2:
            print("\n🎯 Точки выбраны, готово к запуску алгоритма")
        else:
            print("\n⚠ Точки не выбраны")
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
    
