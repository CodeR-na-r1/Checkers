#pragma once

#include "Figure.h"
#include "Point_2D.h"
#include "Checher.h"
#include "king.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Board
{
public:
	Figure*** board;
	long count, count_white;
	bool _end_game;
	
	Board();
	Board(const char* namefile);
	Board(const Board& _board);

	// Получить координаты относительно лучшего хода для фигур конкретного цвета
	Point_2D get_effective_move(const bool _isWhite);

	// Подсчет сколько фигур может убить враг, если фигура совершит определенный ход
	int _enemy_kills_after_move(const Point_2D& _point) const;

	// Подсчет коэффицента для определенного хода
	int _calc_ratio(const Point_2D& _point) const;

	// Сделать ход по координатам
	void do_move(const Point_2D& _move_point);

	// Выввод нотаций
	void notation(const Point_2D& _point, ostream& out) const;

	// Условие окончания игры
	bool end_game() const;

	// Условие выйгрыша белых
	bool white_is_win() const;

	Board& operator=(const Board& _board);

	friend ostream& operator<<(ostream& out, const Board& board);

	friend ofstream& operator <<(ofstream& out, const Board& board);

	~Board();
};