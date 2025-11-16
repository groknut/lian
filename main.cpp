

#include <iostream>
#include "ic.h"
#include "map.h"

int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Укажите файл с картой" << std::endl;
		return 0;
	}

	Ic ic(argv[1]);

	Map map(ic.getVal("lian", "map"));

	std::cout << map.getCell(450, 400);
	std::cout << map.getCell(130, 50);
	
	return 0;
	
}
