#ifndef _LIAN_H_
#define _LIAN_H_

#include "map.h"
#include "ic.h"
#include <queue>
#include <unordered_set>
#include <cmath>
#include <vector>
using namespace std;

struct CompareCell {
    bool operator()(Cell* a, Cell* b) const;
};

class LIAN {
private:

    const Ic& ic;
    const Map& map;
    
    std::priority_queue<Cell*, std::vector<Cell*>, CompareCell> openSet;
    std::unordered_set<Cell*> closedSet;
    std::vector<Cell> path;

    // получить соседей
    std::vector<Cell*> getNeighs(Cell* current, Cell* goal, float& theta);

	// прямая видимость между точками
    bool lineOfSight(const Cell* from, const Cell* to) const;

    // расчитать угол 
    float calculateAngle(const Cell* parent, const Cell* current, const Cell* successor) const;

	// востанавливаем путь
    std::vector<Cell> reconstructPath(Cell* goalCell) const;

    // getCircleSuccesors
    std::vector<Cell*> getCirclePoints(Cell* center, float& radius);

	// инициализация
	bool init(float& theta, float& max_angle, Cell*& start, Cell*& goal);

	// проверка, что достигли конечной точки
	bool checkGoal(Cell* current, Cell* goal, float& theta);

	// поиск соседей
	void processNeighs(Cell* current, Cell* goal, float& theta, float& max_angle);

	// expand функция
	void expand(Cell* current, Cell* neigh, Cell* goal, float& max_angle);

public:
    LIAN(const Ic& aic, const Map& amap) : ic(aic), map(amap) {}
    void run();
    std::vector<Cell> returnPath() const { return path; }
};

#endif
