#pragma once

#include "Point.h"
#include <vector>

using namespace std;

struct Point_2D
{
	Point to, from;	// будущая и исходная координаты соответственно
	vector<Point> kills;	// Координаты срубленных шашек
	int enemy_kills_after_move;	// Количество срубленных шашек при ходе конкретной фигурой из-зи этой фигуры
	int ratio;	// Коэффициент успешности хода

	Point_2D();
	Point_2D(Point _to, Point _from);
	Point_2D(Point _to, Point _from, vector<Point>& _kills);

	bool operator>(const Point_2D& _other);
};