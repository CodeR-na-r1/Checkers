#pragma once

#include "Point.h"

Point::Point()
{
	this->x = 0;
	this->y = 0;
}

Point::Point(int _x, int _y)
{
	this->x = _x;
	this->y = _y;
}

bool Point::operator==(const Point& other) const
{
	return this->x == other.x && this->y == other.y;
}