#pragma once

struct Point
{
	int x, y;

	Point();
	Point(int _x, int _y);

	bool operator==(const Point& other) const;
	bool operator>(const Point& other) const;
	bool operator<(const Point& other) const;
};