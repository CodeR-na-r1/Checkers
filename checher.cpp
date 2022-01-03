#pragma once

#include "Checher.h"

Checher::Checher() : Figure() {};

Checher::Checher(const bool _isWhite) : Figure()
{
	this->white = _isWhite;
}

bool Checher::isWhite() const
{
	return this->white;
}

void Checher::_get_more_kills(Figure*** _board, const Point& _point, vector<Point>& _kills, Point_2D& best_kills)
{
	if (_point.x - 1 >= 0 && _point.y + 1 < 8 && _board[_point.y + 1][_point.x - 1] != nullptr && _board[_point.y + 1][_point.x - 1]->isWhite() != this->white && find(_kills.begin(), _kills.end(), Point(_point.x - 1, _point.y + 1)) == _kills.end() && _point.x - 2 >= 0 && _point.y + 2 < 8 && _board[_point.y + 2][_point.x - 2] == nullptr)
	{
		vector<Point> copy = _kills;
		copy.push_back( Point(_point.x - 1, _point.y + 1) );
		_get_more_kills(_board, Point(_point.x - 2, _point.y + 2), copy, best_kills);
	}

	if (_point.x + 1 < 8 && _point.y + 1 < 8 && _board[_point.y + 1][_point.x + 1] != nullptr && _board[_point.y + 1][_point.x + 1]->isWhite() != this->white && find(_kills.begin(), _kills.end(), Point(_point.x + 1, _point.y + 1)) == _kills.end() && _point.x + 2 < 8 && _point.y + 2 < 8 && _board[_point.y + 2][_point.x + 2] == nullptr)
	{
		vector<Point> copy = _kills;
		copy.push_back(Point(_point.x + 1, _point.y + 1));
		_get_more_kills(_board, Point(_point.x + 2, _point.y + 2), copy, best_kills);
	}

	if (_point.x - 1 >= 0 && _point.y - 1 >= 0 && _board[_point.y - 1][_point.x - 1] != nullptr && _board[_point.y - 1][_point.x - 1]->isWhite() != this->white && find(_kills.begin(), _kills.end(), Point(_point.x - 1, _point.y - 1)) == _kills.end() && _point.x - 2 >= 0 && _point.y - 2 >= 0 && _board[_point.y - 2][_point.x - 2] == nullptr)
	{
		vector<Point> copy = _kills;
		copy.push_back(Point(_point.x - 1, _point.y - 1));
		_get_more_kills(_board, Point(_point.x - 2, _point.y - 2), copy, best_kills);
	}

	if (_point.x + 1 < 8 && _point.y - 1 >= 0 && _board[_point.y - 1][_point.x + 1] != nullptr && _board[_point.y - 1][_point.x + 1]->isWhite() != this->white && find(_kills.begin(), _kills.end(), Point(_point.x + 1, _point.y - 1)) == _kills.end() && _point.x + 2 < 8 && _point.y - 2 >= 0 && _board[_point.y - 2][_point.x + 2] == nullptr)
	{
		vector<Point> copy = _kills;
		copy.push_back(Point(_point.x + 1, _point.y - 1));
		_get_more_kills(_board, Point(_point.x + 2, _point.y - 2), copy, best_kills);
	}

	if (_kills.size() > best_kills.kills.size())
	{
		best_kills.to = _point;
		best_kills.kills = _kills;
	}

	return;
}

void Checher::get_possible_moveS(Figure*** _board, const Point& _point, vector<Point_2D>& _res)
{
	vector<Point> temp_kills;
	Point_2D res;

	_get_more_kills(_board, _point, temp_kills, res);

	if (res.kills.size())
	{
		res.from = _point;
		_res.push_back(res);
	}
	else if (this->white)
	{
		if (_point.x - 1 >= 0 && _point.y + 1 < 8 && _board[_point.y + 1][_point.x - 1] == nullptr)
			_res.push_back( Point_2D( Point(_point.x - 1, _point.y + 1), _point ) );

		if (_point.x + 1 < 8 && _point.y + 1 < 8 && _board[_point.y + 1][_point.x + 1] == nullptr)
			_res.push_back(Point_2D(Point(_point.x + 1, _point.y + 1), _point));
	}
	else if (!this->white)
	{
		if (_point.x - 1 >= 0 && _point.y - 1 >= 0 && _board[_point.y - 1][_point.x - 1] == nullptr)
			_res.push_back(Point_2D(Point(_point.x - 1, _point.y - 1), _point));

		if (_point.x + 1 < 8 && _point.y - 1 >= 0 && _board[_point.y - 1][_point.x + 1] == nullptr)
			_res.push_back(Point_2D(Point(_point.x + 1, _point.y - 1), _point));
	}
	else
	{
		throw "Error in calculating the move";
	}

	return;
}

Checher::~Checher()
{

}