#ifndef _LIAN_H_
#define _LIAN_H_

#include "map.h"
#include "ic.h"
#include <queue>
#include <unordered_set>
#include <vector>
#include <functional>

struct CompareCell {
    bool operator()(Cell* a, Cell* b) const;
};

class LIAN {
private:
    const Ic& ic;
    const Map& map;
    
    // Структуры данных
    std::priority_queue<Cell*, std::vector<Cell*>, CompareCell> openSet;
    std::unordered_set<Cell*> closedSet;
    std::vector<Cell> path;
    
    // Методы
    std::vector<Cell*> getNeighs(Cell* current, Cell* goal, float& theta);

    bool lineOfSight(const Cell* from, const Cell* to) const;
    float calculateAngle(const Cell* parent, const Cell* current, const Cell* successor) const;

    std::vector<Cell> reconstructPath(Cell* goalCell) const;
    std::vector<Cell*> getCirclePoints(Cell* center, float& radius);

	bool init(float& theta, float& max_angle, Cell*& start, Cell*& goal);
	bool checkGoal(Cell* current, Cell* goal, float& theta);

	void processNeighs(Cell* current, Cell* goal, float& theta, float& max_angle);
	void expand(Cell* current, Cell* neigh, Cell* goal, float& max_angle);

public:
    LIAN(const Ic& aic, const Map& amap) : ic(aic), map(amap) {}
    void run();
    std::vector<Cell> returnPath() const;
};

#endif
