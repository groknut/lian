#include "map.h"

float Cell::distanceTo(const Cell& other) const
{
	return sqrt(
		(other.x - x) * (other.x - x) + (other.y - y) * (other.y - y)	
	);
}

ostream& operator << (ostream& out, const Cell& cell)
{
	out << "(" << cell.y << ", " << cell.x << ", " << cell.value << ")" << std::endl;
	return out;
}

bool Cell::operator == (const Cell& other) const
{
	return x == other.x && y == other.y;
}

Map::Map(const std::string& filename)
{
	std::ifstream in(filename);
	if (!in.is_open())
		throw ReadFileError();

	in >> width >> height;

	cells.resize(width * height);

	char val;
	size_t x = 0, y = 0;

	while (in >> val)
	{
		Cell& cell = cells[y * width + x];
		cell.x = (short)x;
		cell.y = (short)y;
		cell.value = (val != '0');

		x++;
		if (x >= width)
		{
			x = 0;
			y++;
		}
		
	}
}

const Cell& Map::getCell(const int& x, const int& y) const
{
	return cells[((size_t)y * width + (size_t)x)];
}

const Cell& Map::getCell(const int& i) const
{
	return cells[i];
}

bool Map::isFree(const Cell* cell) const
{
	return ( (size_t)cell->x < 0 || (size_t)cell->x >= width || (size_t)cell->y < 0 || (size_t)cell->y >= height ) ? false : !getCell(cell->x, cell->y).value;
}

bool Map::isFree(const int& x, const int& y) const
{
	return ( (size_t)x < 0 || (size_t)x >= width || (size_t)y < 0 || (size_t)y >= height ) ? false : !getCell(x, y).value;
}

ostream& operator << (ostream& out, const Map& map) 
{
	out << "Map:" << std::endl;
	out << "Width: " << map.width << " Height: " << map.height << std::endl;

	for (size_t y = 0; y < map.height; y++)
	{
		for (size_t x = 0; x < map.width; x++)
			out << (map.isFree(x, y) ? '0' : '1');
		out << std::endl;
	}
	out << std::endl;
	
    
	return out;

}
