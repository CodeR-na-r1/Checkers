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
	// Проверка цвета фигуры
	virtual const bool isWhite() const = 0;
	// Проверка на дамку
	virtual const bool isKing() const = 0;
	// Рекурсивная функция, находящая наиболее эффективное убийство для фигуры
	virtual void _get_more_kills(Figure*** _board, const Point& _point, vector<Point>& _kills, Point_2D& best_kills) = 0;
	// Функция, находящая возможные ходы для фигуры (или один, но с убийствами)
	virtual	void get_possible_moveS(Figure*** _board, const Point& _point, vector<Point_2D>& _res) = 0;
	// Функция для выввода фигуры на экран
	virtual const char* output() = 0;
};