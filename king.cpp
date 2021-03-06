#pragma once

#include "King.h"

King::King() : Figure() {};

King::King(const bool _isWhite) : Figure()
{
	this->white = _isWhite;

	return;
}

const bool King::isWhite() const
{
	return this->white;
}

const bool King::isKing() const
{
	return true;
}

void King::_get_more_kills(Figure*** _board, const Point& _point, vector<Point>& _kills, Point_2D& best_kills)
{
	// Проверка границ, наличия фигуры, цвета фигуры, то, что еще не сбивали ее в этом ходе, будущие границы (куда встает фигура) и отсутсвие фигуры на клетке куда ходим
	// Вверх влево
	bool finded_enemy(false);
	// Как у шашки, но проверяем диагонали
	for (int i(_point.y+1), j(_point.x-1); i < 8 && j >= 0; i++, j--)
	{
		if (finded_enemy)
		{
			vector<Point> copy = _kills;
			copy.push_back(Point(j+1, i-1));	// Координаты врага, обнаруженного на предыдущей итерации (подгоняем координаты)
			_get_more_kills(_board, Point(j, i), copy, best_kills);
			break;
		}

		if (_board[i][j] != nullptr)
		{
			if (_board[i][j]->isWhite() == this->white)
				break;

			if (_board[i][j]->isWhite() != this->white && find(_kills.begin(), _kills.end(), Point(j, i)) == _kills.end() && i + 1 < 8 && j - 1 >= 0 && _board[i + 1][j - 1] == nullptr)
			{
				finded_enemy = true;	// Если нашли врага, то поднимаем флаг и переходим на следующую итерацию, откуда идем в рекурсию
				continue;
			}
		}

	}

	finded_enemy = false;
	for (int i(_point.y + 1), j(_point.x + 1); i < 8 && j < 8; i++, j++)
	{
		if (finded_enemy)
		{
			vector<Point> copy = _kills;
			copy.push_back(Point(j - 1, i - 1));
			_get_more_kills(_board, Point(j, i), copy, best_kills);
			break;
		}

		if (_board[i][j] != nullptr)
		{
			if (_board[i][j]->isWhite() == this->white)
				break;

			if (_board[i][j]->isWhite() != this->white && find(_kills.begin(), _kills.end(), Point(j, i)) == _kills.end() && i + 1 < 8 && j + 1 < 8 && _board[i + 1][j + 1] == nullptr)
			{
				finded_enemy = true;
				continue;
			}
		}

	}

	finded_enemy = false;
	for (int i(_point.y - 1), j(_point.x - 1); i >= 0 && j >= 0; i--, j--)
	{
		if (finded_enemy)
		{
			vector<Point> copy = _kills;
			copy.push_back(Point(j + 1, i + 1));
			_get_more_kills(_board, Point(j, i), copy, best_kills);
			break;
		}

		if (_board[i][j] != nullptr)
		{
			if (_board[i][j]->isWhite() == this->white)
				break;

			if (_board[i][j]->isWhite() != this->white && find(_kills.begin(), _kills.end(), Point(j, i)) == _kills.end() && i - 1 >= 0 && j - 1 >= 0 && _board[i - 1][j - 1] == nullptr)
			{
				finded_enemy = true;
				continue;
			}
		}

	}

	finded_enemy = false;
	for (int i(_point.y - 1), j(_point.x + 1); i >= 0 && j < 8; i--, j++)
	{
		if (finded_enemy)
		{
			vector<Point> copy = _kills;
			copy.push_back(Point(j - 1, i + 1));
			_get_more_kills(_board, Point(j, i), copy, best_kills);
			break;
		}

		if (_board[i][j] != nullptr)
		{
			if (_board[i][j]->isWhite() == this->white)
				break;

			if (_board[i][j]->isWhite() != this->white && find(_kills.begin(), _kills.end(), Point(j, i)) == _kills.end() && i - 1 >= 0 && j + 1 < 8 && _board[i - 1][j + 1] == nullptr)
			{
				finded_enemy = true;
				continue;
			}
		}

	}

	if (_kills.size() > best_kills.kills.size())
	{
		best_kills.kills = _kills;
		best_kills.to = _point;
	}

	return;
}

void King::get_possible_moveS(Figure*** _board, const Point& _point, vector<Point_2D>& _res)
{
	vector<Point> kills;
	Point_2D _best_kill;
	_get_more_kills(_board, _point, kills, _best_kill);	// Проверяем и получаем ход с убийствами, если есть убийства
		
	if (_best_kill.kills.size())
	{
		_res.push_back( Point_2D(_best_kill.to, _point, _best_kill.kills) );
		return;
	}

	// Иначе стандартные ходы для фигуры соответсвенного цвета
	for (int i(_point.y + 1), j(_point.x - 1); i < 8 && j >= 0; i++, j--)
	{
		if (_board[i][j] != nullptr)
			break;

		_res.push_back(Point_2D(Point(j, i), _point));
	}

	for (int i(_point.y + 1), j(_point.x + 1); i < 8 && j < 8; i++, j++)
	{
		if (_board[i][j] != nullptr)
			break;

		_res.push_back(Point_2D(Point(j, i), _point));
	}

	for (int i(_point.y - 1), j(_point.x - 1); i >= 0 && j >= 0; i--, j--)
	{
		if (_board[i][j] != nullptr)
			break;

		_res.push_back(Point_2D(Point(j, i), _point));
	}

	for (int i(_point.y - 1), j(_point.x + 1); i >= 0 && j < 8; i--, j++)
	{
		if (_board[i][j] != nullptr)
			break;

		_res.push_back(Point_2D(Point(j, i), _point));
	}

	return;
}

const char* King::output()
{
	return this->isWhite() ? "W" : "B";
}

King::~King()
{

}