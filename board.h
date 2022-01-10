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

	Point_2D get_effective_move(const bool _isWhite);

	int _enemy_kills_after_move(const Point_2D& _point) const;

	int _calc_ratio(const Point_2D& _point) const;

	void do_move(const Point_2D& _move_point);

	void notation(const Point_2D& _point, ostream& out) const;

	bool end_game() const;

	bool white_is_win() const;

	Board& operator=(const Board& _board);

	friend ostream& operator<<(ostream& out, const Board& board);

	friend ofstream& operator <<(ofstream& out, const Board& board);

	~Board();
};