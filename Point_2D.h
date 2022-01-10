#pragma once

#include "Point.h"
#include <vector>

using namespace std;

struct Point_2D
{
	Point to, from;
	vector<Point> kills;
	int enemy_kills_after_move;
	int ratio;	// коэффициент

	Point_2D();
	Point_2D(Point _to, Point _from);
	Point_2D(Point _to, Point _from, vector<Point>& _kills);

	bool operator>(const Point_2D& _other);
};