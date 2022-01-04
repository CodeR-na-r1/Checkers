#pragma once

#include "Board.h"

Board::Board()
{
	this->board = new Figure**[8];
	for (int i = 0; i < 8; ++i)
	{
		board[i] = new Figure*[8];
		for (int j = 0; j < 8; ++j)
		{
			this->board[i][j] = nullptr;
		}
	}

	this->count = 0;
	this->count_white = 0;

	return;
}

Board::Board(const char* namefile) : Board()
{
	ifstream in(namefile);
	bool isWhite = false;
	string temp;
	
	while (getline(in, temp))
	{
		if (temp.find("Black", 0) != std::string::npos)
			isWhite = false;
		else if (temp.find("White", 0) != std::string::npos)
			isWhite = true;
		else
		{
			try
			{
				this->board[temp[1] - '0' - 1][temp[0] - '0' - 17] = new Checher(isWhite);
				if (isWhite) ++this->count_white;
				++this->count;
			}
			catch(...)
			{
				throw "Encorrect data in input file!";
			}
		}

	}
	
	in.close();

	return;
}

Point_2D Board::get_effective_move(const bool _isWhite)
{
	vector<Point_2D> temp_moves;
	vector<Point_2D> all_moves;
	for (int i = 7; i >= 0; --i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (this->board[i][j] == nullptr || this->board[i][j]->isWhite() != _isWhite) continue;
			temp_moves.clear();
			this->board[i][j]->get_possible_moveS(this->board, Point(j, i), temp_moves);
			for (int z = 0; z < temp_moves.size(); z++)
			{
				all_moves.push_back(temp_moves[z]);
			}
		}
	}

	Point_2D res;
	for (int i = 0; i < all_moves.size(); ++i)
	{
		if (all_moves[i] > res)
			res = all_moves[i];
	}

	if (!res.kills.size())
	{
		int max_count(4), max_kill(0);
		vector<pair<Point_2D, Point_2D>> all_future_moves;
		vector<pair<Point_2D, Point_2D>> temp_all_future_moves;
		for (int i = 0; i < all_moves.size(); i++)
		{
			all_future_moves.push_back( pair<Point_2D, Point_2D>(all_moves[i], all_moves[i]) );		// Самый первый ход, а вторая пара - будущие ходы
		}

		while (max_count)
		{
			temp_all_future_moves.clear();

			for (int i = 0; i < all_future_moves.size(); ++i)
			{
				this->board[all_future_moves[i].first.from.y][all_future_moves[i].first.from.x]->get_possible_moveS(this->board, Point(all_future_moves[i].second.to.x, all_future_moves[i].second.to.y), temp_moves);
				for (int z = 0; z < temp_moves.size(); z++)
				{
					temp_all_future_moves.push_back( pair<Point_2D, Point_2D>(all_future_moves[i].first, temp_moves[z]) );
				}
				temp_moves.clear();
			}

			for (int z = 0; z < temp_all_future_moves.size(); z++)
			{
				if (temp_all_future_moves[z].second.kills.size() > max_kill)
				{
					max_kill = temp_all_future_moves[z].second.kills.size();
					res = temp_all_future_moves[z].first;
				}
			}
			if (max_kill)
				break;

			all_future_moves = temp_all_future_moves;
			--max_count;
		}
	}

	return res;
}

void Board::do_move(const Point_2D& _move_point)
{
	if (_move_point.to > Point(7,7) || _move_point.from > Point(7, 7) || _move_point.to < Point(0, 0) || _move_point.from < Point(0, 0))
		throw "Error points in parameter  (from 'do_move')";

	bool color_moving_figure = this->board[_move_point.from.y][_move_point.from.x]->isWhite();

	this->board[_move_point.to.y][_move_point.to.x] = new Checher(color_moving_figure);
	this->board[_move_point.from.y][_move_point.from.x]->~Figure();
	this->board[_move_point.from.y][_move_point.from.x] = nullptr;

	for (int i = 0; i < _move_point.kills.size(); i++)
	{
		if (this->board[_move_point.kills[i].y][_move_point.kills[i].x] == nullptr) throw "Error points of kills in parameter  (from 'do_move')";
		this->board[_move_point.kills[i].y][_move_point.kills[i].x]->~Figure();
		this->board[_move_point.kills[i].y][_move_point.kills[i].x] = nullptr;
	}

	this->count -= _move_point.kills.size();
	if (!color_moving_figure) this->count_white -= _move_point.kills.size();

	return;
}

ostream& operator<<(ostream& out, const Board& _board)
{
	out << "Board: white figures == " << _board.count_white << "; black figures == " << _board.count - _board.count_white << endl;

	for (int i = 7; i >= 0; --i)
	{
		out << i + 1 << ' ';
		for (int j = 0; j < 8; ++j)
		{
			if (_board.board[i][j] == nullptr)
				out<<"X ";
			else if (_board.board[i][j]->isWhite())
				out<<"W ";
			else if (_board.board[i][j]->isWhite() == false)
				out<<"B ";
			else
				throw "Error output figures on board on console";
		}
		out << endl;
	}

	out << ' ';
	for (int i = 0; i < 8; ++i)
	{
		out << ' ' << char(i + 65);
	}
	
	return out;
}

ofstream& operator <<(ofstream& out, const Board& board)
{
	out << string("White: ") << board.count_white << endl;
	for (int i = 0; i < 8; i++)			// y
	{
		for (int j = 0; j < 8; j++)		// x
		{
			if (board.board[i][j] != nullptr && board.board[i][j]->isWhite())
			{
				out << char(j + 65) << i + 1 << endl;
			}
		}
	}

	out << string("Black: ") << board.count - board.count_white;
	for (int i = 0; i < 8; i++)			// y
	{
		for (int j = 0; j < 8; j++)		// x
		{
			if (board.board[i][j] != nullptr && !board.board[i][j]->isWhite())
			{
				out << endl << char(j + 65) << i + 1;
			}
		}
	}

	return out;
}

Board::~Board()
{
	for (int i = 0; i < 8; ++i)
	{
		delete[] this->board[i];
	}

	delete[] board;

	return;
}