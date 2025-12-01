#include <iostream>
#include "ic.h"
#include "map.h"
#include "lian.h"
using namespace std;
int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "not file" << std::endl;
        return 0;
    }

    Ic ic(argv[1]);

	std::string map_filename = ic.getVal("lian", "map");
    std::cout << "Map file: " << map_filename << std::endl;
    
    Map map(map_filename);
    std::cout << "Map loaded successfully: " << map.shape().first << " x " << map.shape().second << std::endl;

	auto [w, h] = map.shape();

	if (w * h < 1000)
		cout << map << endl;

    // Запускаем алгоритм LIAN
    LIAN lian(ic, map);
    lian.run();

    // Получаем и выводим результат
    auto path = lian.returnPath();

    if (path.empty())
        std::cout << "No path found!" << std::endl;
    else 
    {
	    std::cout << "Path found with " << path.size() << " points:" << std::endl;
	    for (const auto& cell : path) 
	        std::cout << "(" << cell.x << ", " << cell.y << ")" << std::endl;

	    map.update(path);
	    if (w * h < 1000)
    		cout << map << endl;
    }
    
    return 0;
}
