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
	this->_end_game = false;

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
				if (temp[0] != 'M')
				{
					this->board[temp[1] - '0' - 1][temp[0] - '0' - 17] = new Checher(isWhite);
					if (isWhite) ++this->count_white;
					++this->count;
				}
				else
				{
					this->board[temp[2] - '0' - 1][temp[1] - '0' - 17] = new King(isWhite);
					if (isWhite) ++this->count_white;
					++this->count;
				}
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

	this->board[_move_point.to.y][_move_point.to.x] = this->board[_move_point.from.y][_move_point.from.x];
	this->board[_move_point.from.y][_move_point.from.x]->~Figure();
	this->board[_move_point.from.y][_move_point.from.x] = nullptr;

	for (int i = 0; i < _move_point.kills.size(); i++)
	{
		if (this->board[_move_point.kills[i].y][_move_point.kills[i].x] == nullptr) throw "Error points of kills in parameter  (from 'do_move')";
		this->board[_move_point.kills[i].y][_move_point.kills[i].x]->~Figure();
		this->board[_move_point.kills[i].y][_move_point.kills[i].x] = nullptr;
	}

	this->count -= _move_point.kills.size();
	if (!this->board[_move_point.to.y][_move_point.to.x]->isWhite()) this->count_white -= _move_point.kills.size();

	if (this->count_white == 0 || this->count - this->count_white == 0)
		this->_end_game = true;

	if ((_move_point.to.y == 7 && this->board[_move_point.to.y][_move_point.to.x]->isWhite()) || (_move_point.to.y == 0 && !this->board[_move_point.to.y][_move_point.to.x]->isWhite()))
	{
		bool color_figure = this->board[_move_point.to.y][_move_point.to.x]->isWhite();
		this->board[_move_point.to.y][_move_point.to.x]->~Figure();
		this->board[_move_point.to.y][_move_point.to.x] = new King(color_figure);
	}

	return;
}

void Board::notation(const Point_2D& _point) const
{
	if (this->board[_point.from.y][_point.from.x] == nullptr)
	{
		throw "Error point in 'notation'!";
	}

	bool color_move_figure = this->board[_point.from.y][_point.from.x]->isWhite();

	if (color_move_figure)
	{
		cout << "White: ";
	}
	else
	{
		cout << "Black: ";
	}

	cout << char(_point.from.x + 65) << _point.from.y + 1;
	cout << " -> ";
	cout << char(_point.to.x + 65) << _point.to.y + 1;
	cout << " || Kills === " << _point.kills.size();

	if (_point.kills.size())
	{
		cout << " (" << char(_point.kills[0].x + 65) << _point.kills[0].y + 1;
		for (int i = 1; i < _point.kills.size(); ++i)
		{
			cout << ", " << char(_point.kills[i].x + 65) << _point.kills[i].y + 1;
		}
		cout << ")";
	}
	
	cout << endl;

	return;
}

bool Board::end_game() const
{
	return this->_end_game;
}

bool Board::white_is_win() const
{
	return this->count_white ? true : false;
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
				out << "X ";
			else
			{
				cout << _board.board[i][j]->output() << " ";

			}
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
				if (board.board[i][j]->isKing())
				{
					out << 'M';
				}
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
				out << endl;
				if (board.board[i][j]->isKing())
				{
					out << 'M';
				}
				out << char(j + 65) << i + 1;
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