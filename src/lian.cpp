
#include "../head/lian.h"

bool CompareCell::operator()(Cell* a, Cell* b) const {
    return a->f > b->f;
}

vector<Cell*> LIAN::getCirclePoints(Cell* center, float& radius) 
{
	int r = (int)radius;
    vector<Cell*> points;
    auto [width, height] = map.shape();
    
    int xc = center->x, yc = center->y;
    
    int x = 0, y = r;
    int d = 3 - 2 * r;
    
    while (x <= y) 
    {

        int coords[8][2] = {
            {xc + x, yc + y}, {xc - x, yc + y},
            {xc + x, yc - y}, {xc - x, yc - y},
            {xc + y, yc + x}, {xc - y, yc + x},
            {xc + y, yc - x}, {xc - y, yc - x}
        };
        
        for (int i = 0; i < 8; i++) 
        {
            int px = coords[i][0];
            int py = coords[i][1];
            
            if (px >= 0 && px < width && py >= 0 && py < height) 
            {
                Cell* cell = const_cast<Cell*>(&map.getCell(px, py));
                points.push_back(cell);
            }
        }
        
        x++;
        if (d < 0)
            d = d + 4 * x + 6;
        else 
        {
            y--;
            d = d + 4 * (x - y) + 10;
        }
    }
    
    return points;
}

vector<Cell*> LIAN::getNeighs(Cell* current, Cell* goal, float& theta) 
{
    vector<Cell*> neighs;
    auto [width, height] = map.shape();

    float dist = current->distanceTo(*goal);

    if (dist <= theta)
    	neighs.push_back(goal);

    vector<Cell*> circle_points = getCirclePoints(current, theta);

    for (Cell* neigh : circle_points)
    {
    	if (!map.isFree(neigh))
    		continue;

		if (*neigh == *goal)
			continue;

		if (!lineOfSight(current, neigh))
			continue;

		neighs.push_back(neigh);
    }

	return neighs;

}

bool LIAN::lineOfSight(const Cell* from, const Cell* to) const 
{
    int x0 = from->x, y0 = from->y;
    int x1 = to->x, y1 = to->y;
    
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;
    
    int x = x0;
    int y = y0;
    
    while (true) 
    {
        if (x == x1 && y == y1) 
        	break;
        
        if (!map.isFree(x, y)) 
        	return false;
        
        int e2 = 2 * err;

        if (e2 > -dy) 
        {
            err -= dy;
            x += sx;
        }
        
        if (e2 < dx) 
        {
            err += dx;
            y += sy;
        }
    }
    
    return true;
}

float LIAN::calculateAngle(const Cell* parent, const Cell* current, const Cell* successor) const 
{
    float 
    	dx1 = current->x - parent->x, 
    	dy1 = current->y - parent->y,
    	dx2 = successor->x - current->x,
    	dy2 = successor->y - current->y;
    
    float dot = dx1 * dx2 + dy1 * dy2;
    float m1 = sqrt(dx1 * dx1 + dy1 * dy1);
    float m2 = sqrt(dx2 * dx2 + dy2 * dy2);
    
    if (m1 == 0 || m2 == 0 ) 
    	return 0.0f;
    
    float cos_angle = dot / (m1 * m2);
    cos_angle = max(-1.0f, min(1.0f, cos_angle));
    
    return acos(cos_angle);
}

vector<Cell> LIAN::reconstructPath(Cell* goalCell) const 
{
    vector<Cell> path;
    Cell* current = goalCell;
    
    while (current != nullptr) {
        path.insert(path.begin(), *current);
        current = current->parent;
    }
    
    return path;
}

bool LIAN::init(float& theta, float& max_angle, Cell*& start, Cell*& goal)
{
	theta = ic.as_double("lian", "theta") > 0 ? ic.as_double("lian", "theta") : 10.0f;
	max_angle = ic.as_double("lian", "angle") * M_PI / 180.0f;

	Cell start_cell = Cell(ic.as_int_pair("lian", "start"));
	Cell goal_cell = Cell(ic.as_int_pair("lian", "goal"));

	cout << "Start: (" << start_cell.y << ", " << start_cell.x << ")" << endl;
	cout << "Goal: (" << goal_cell.y << ", " << goal_cell.x << ")" << endl;
	cout << "Theta: " << theta << ", Max angle: " << max_angle * 180.0f / M_PI << " deg" << endl;

	if (!map.isFind(start_cell))
	{
		cout << "Start point not found in map" << endl;
		return false;
	}

	if (!map.isFind(goal_cell))
	{
		cout << "Goal point not found in map" << endl;
		return false;
	}

	if (!map.isFree(start_cell.x, start_cell.y))
	{
        cout << "ERROR: Start position is blocked!" << endl;
        return false;
    }
   
    if (!map.isFree(goal_cell.x, goal_cell.y)) {
        cout << "ERROR: Goal position is blocked!" << endl;
        return false;
    }

	start = const_cast<Cell*>(&map.getCell(start_cell.x, start_cell.y));
    goal = const_cast<Cell*>(&map.getCell(goal_cell.x, goal_cell.y));

    start->g = 0.0f;

    return true;
	
}

bool LIAN::checkGoal(Cell* current, Cell* goal, float& theta)
{

	float dist = current->distanceTo(*goal);

	if (dist <= theta && lineOfSight(current, goal))
	{
		goal->parent = current;
		path = reconstructPath(goal);
		return true;
	}

	if (*current == *goal)
	{
		path = reconstructPath(goal);
		return true;
	}

	return false;

}

void LIAN::processNeighs(Cell* current, Cell* goal, float& theta, float& max_angle)
{
	vector<Cell*> neighs = getNeighs(current, goal, theta);

    for (Cell* neigh : neighs)
    {
    	if (closedSet.find(neigh) != closedSet.end()) 
    		continue;
    	expand(current, neigh, goal, max_angle);
    }
    
}

void LIAN::expand(Cell* current, Cell* neigh, Cell* goal, float& max_angle)
{
	float angle = 0.0f;

	if (current->parent != nullptr)
	{
		angle = calculateAngle(current->parent, current, neigh);
		if (angle > max_angle)
		{
			return;
		}
	}

	for (Cell* closed_cell : closedSet) 
	{
	    if (*closed_cell == *neigh) 
	    {
	        if (closed_cell->parent && closed_cell->parent->x == current->x && closed_cell->parent->y == current->y) {
	            return;
	        }
	    }
	}

	float update_g = current->g + current->distanceTo(*neigh);

	if (update_g < neigh->g)
	{
		neigh->parent = current;
		neigh->g = update_g;
		neigh->h = neigh->distanceTo(*goal);
		neigh->f = neigh->g + neigh->h;
		openSet.push(neigh);
		neigh->angle = angle * 180.0f / M_PI;
	}
}

void LIAN::run() {

    auto [width, height] = map.shape();

	float theta, max_angle;
	Cell *start, *goal;

	if (!init(theta, max_angle, start, goal))
		return;
    
    start->h = start->distanceTo(*goal);
    start->f = start->g + start->h;

    start->value = CellType::START;
	goal->value = CellType::GOAL;    
    
    openSet.push(start);

	while (!openSet.empty())
	{
		Cell* current = openSet.top();
		openSet.pop();

		if (closedSet.find(current) != closedSet.end())
			continue;

		closedSet.insert(current);

		if (checkGoal(current, goal, theta))
		{
			cout << "Goal reached!" << endl;
			return;
		}

		processNeighs(current, goal, theta, max_angle);
	}
}

void LIAN::save()
{
	std::ofstream out(ic.getVal("output", "coords"));

	if (!out.is_open())
	{
		cout << "File not found";
		return;
	}

	if (path.empty())
	{
		cout << "Path not found";
		return;
	}

	for (auto& cell: path)
	{
		out << cell.x << ' ' << cell.y << ' ' << cell.angle << endl;
	}

	out.close();
}
