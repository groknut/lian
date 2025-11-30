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

	float g = numeric_limits<float>::max(),
		h = 0.0f,
		f = 0.0f;

	Cell* parent = nullptr;
	
	float angle = 0.0f;

	float distanceTo(const Cell& other) const;
	bool operator == (const Cell& other) const;
	friend ostream& operator << (ostream& out, const Cell& cell);

	Cell() = default;
	Cell(const int& ax, const int& ay) : x(ax), y(ay) {}
	Cell(const std::pair<int, int>& p) : y(p.second), x(p.first) {}
};

class Map
{
private:
	size_t width = 0, height = 0;
	std::vector<Cell> cells;
public:

	Map(const std::string& filename);
	const Cell& getCell(const int& x, const int& y) const;
	const Cell& getCell(const int& i) const;
	bool isFree(const int& x, const int& y) const;
	bool isFree(const Cell* cell) const;
	friend ostream& operator << (ostream& out, const Map& map);
	std::pair<int, int> shape() const { return std::make_pair(width, height); }
};

class ReadFileError {};

#endif
