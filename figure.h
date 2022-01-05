#pragma once

#include <vector>

#include "Point_2D.h"
#include "Point.h"
#include <vector>

using namespace std;

class Figure
{
protected:
	bool white;

public:
	virtual bool isWhite() const = 0;
	virtual void _get_more_kills(Figure*** _board, const Point& _point, vector<Point>& _kills, Point_2D& best_kills) = 0;
	virtual	void get_possible_moveS(Figure*** _board, const Point& _point, vector<Point_2D>& _res) = 0;
	virtual const char* output() = 0;
};