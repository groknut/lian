

#ifndef _MAP_H_
#define _MAP_H_

#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Cell
{
	int x = 0, y = 0;
	bool value = false;

// g = расстояние от старта до предыдущей клетки + расстояние от предыдущей клетки
// эврестическая оценка
// g + h
// угол от предыдущей клетки к текущей
	float g = 0.0f,
		h = 0.0f,
		f = 0.0f;

	Cell* parent = nullptr;
	
	float angle = 0.0f;

	float distanceTo(const Cell& other) const;
	bool operator == (const Cell& other) const;
	friend ostream& operator << (ostream& out, const Cell& cell);

	Cell() = default;
	Cell(const int& ax, const int& ay) : x(ax), y(ay) {}
};

class Map
{
private:
	size_t width = 0, height = 0;
	std::vector<Cell> cells;
public:

	Map(const std::string& filename);
	const Cell& getCell(const int& y, const int& x) const;
	bool isFree(const int& y, const int& x) const;
	friend ostream& operator << (ostream& out, const Map& map);
};

class ReadFileError {};

#endif
