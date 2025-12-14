#include <iostream>
#include "./cfig/cfig.h"
#include "./head/map.h"
#include "./head/lian.h"
using namespace std;

int main()
{

	Cfig config("config.ini", Cfig::EQUAL, Cfig::HASH);


	std::string map_filename = config("input", "map");
    std::cout << "Map file: " << map_filename << std::endl;
    
    Map map(map_filename);
    std::cout << "Map loaded successfully: " << map.shape().first << " x " << map.shape().second << std::endl;

	auto [w, h] = map.shape();

	if (w * h < 1000)
		cout << map << endl;

    // Запускаем алгоритм LIAN
    LIAN lian(config, map);
    lian.run();

    // Получаем и выводим результат
    auto path = lian.getPath();

    if (path.empty())
        std::cout << "No path found!" << std::endl;
    else 
    {
	    std::cout << "Path found with " << path.size() << " points:" << std::endl;
	    for (const auto& cell : path) 
	        std::cout << "(" << cell.x << ", " << cell.y << ", " << cell.angle << ")" << std::endl;

	    map.update(path);

		lian.save();
	    
	    if (w * h < 1000)
    		cout << map << endl;
    }
    
    return 0;
}
