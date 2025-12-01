#include "map.h"

float Cell::distanceTo(const Cell& other) const
{
	return sqrt(
		(other.x - x) * (other.x - x) + (other.y - y) * (other.y - y)	
	);
}

ostream& operator<<(ostream& out, const CellType& ctype)
{
	switch (ctype)
	{
	case CellType::FREE:
		out << "0";
		break;
	case CellType::OBSTACLE:
		out << "1";
		break;
	case CellType::PATH:
		out << "2";
		break;
	case CellType::START:
		out << "A";
		break;
	case CellType::GOAL:
		out << "B";
		break;
	default:
		out << "?";
	}
	return out;
}

void Map::updateCell(const int& x, const int& y, const CellType& ctype)
{
	cells[y * width + x].value = ctype;	
}

void Map::updateCell(const Cell& cell, const CellType& ctype)
{
	updateCell(cell.x, cell.y, ctype);
}

void Map::update(const vector<Cell>& path)
{
	if (path.size() <= 2) return;

	for (size_t i = 1; i < path.size() - 1; i++)
	{
		updateCell(path[i], CellType::PATH);
	}
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

		if (val == '0')
			cell.value = CellType::FREE;
		else if (val == '1')
			cell.value = CellType::OBSTACLE;
		else
			cell.value = CellType::FREE;

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
	if ((size_t)cell->x < 0 || (size_t)cell->x >= width || (size_t)cell->y < 0 || (size_t)cell->y >= height)
		return false;

	CellType ctype = getCell(cell->x, cell->y).value;

	if (ctype == CellType::FREE || ctype == CellType::START || ctype == CellType::GOAL)
		return true;

	return false;	
}

bool Map::isFree(const int& x, const int& y) const
{
	if ((size_t)x < 0 || (size_t)x >= width || (size_t)y < 0 || (size_t)y >= height)
		return false;

	CellType ctype = getCell(x, y).value;

	if (ctype == CellType::FREE || ctype == CellType::START || ctype == CellType::GOAL)
		return true;

	return false;
}

ostream& operator << (ostream& out, const Map& map) 
{
	out << "Map:" << std::endl;
	out << "Width: " << map.width << " Height: " << map.height << std::endl;

	for (size_t y = 0; y < map.height; y++)
	{
		for (size_t x = 0; x < map.width; x++)
			out << map.cells[y * map.width + x].value;
		out << std::endl;
	}
	out << std::endl;
	
    
	return out;

}
