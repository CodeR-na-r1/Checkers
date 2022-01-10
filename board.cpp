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

Board::Board(const Board& _board) : Board()
{
	this->count = _board.count;
	this->count_white = _board.count_white;
	this->_end_game = _board._end_game;

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (_board.board[i][j] == nullptr)
				this->board[i][j] = nullptr;
			else if (_board.board[i][j]->isKing())
				this->board[i][j] = new King(_board.board[i][j]->isWhite());
			else
				this->board[i][j] = new Checher(_board.board[i][j]->isWhite());
		}
	}

	return;
}

Point_2D Board::get_effective_move(const bool _isWhite)
{
	vector<Point_2D> temp_moves;
	vector<Point_2D> all_moves;
	bool fook = false;
	for (int i = 7; i >= 0; --i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (this->board[i][j] == nullptr || this->board[i][j]->isWhite() != _isWhite) continue;	// Если нет фигуры или цвет не тот, то пропускаем ее
			temp_moves.clear();
			this->board[i][j]->get_possible_moveS(this->board, Point(j, i), temp_moves);	// Получаем возможные ходы шашки
			for (int z = 0; z < temp_moves.size(); z++)
			{
				if (!fook && temp_moves[z].kills.size())	// Если она рубит других, то ходы без атаки на врага учитывать нельзя (фук)
				{
					fook = true;
					all_moves.clear();
				}

				if (fook && temp_moves[z].kills.size() == 0)
				{
					continue;
				}
				// Обработка ходов
				temp_moves[z].enemy_kills_after_move = _enemy_kills_after_move(temp_moves[z]);
				temp_moves[z].ratio = _calc_ratio(temp_moves[z]);	// Рассчет коэффицента
				all_moves.push_back(temp_moves[z]);
			}
		}
	}

	Point_2D res;
	res.ratio = -999;
	for (int i = 0; i < all_moves.size(); ++i)
	{
		if (all_moves[i].ratio > res.ratio)	// Отбираем ход с наибольшим коэффицентом
			res = all_moves[i];
	}

	if (res.ratio > 0 || res.ratio < 0)	// Если К > 0, то убиваем больше чем теряем, поэтому ходим. Если К < 0, то даже при лучшем ходе есть потери,поэтому ходим
		return res;

	// Иначе смотрим будущие ходы и ищем с наиболльшим коэффицентом

	int max_count(4), max_ratio(-999);	// Переменные для контроля глубины анализа (защита от бесконечного цикла) и нахождения макс. коэффицента соответственно
	vector<pair<Point_2D, Point_2D>> all_future_moves;	// Рассчеты те же, но вектор сложнее (нужно хранить самый первый ход, чтоб после прогноза знать как ходить + учет исходных координат фигуры
	vector<pair<Point_2D, Point_2D>> temp_all_future_moves;
	for (int i = 0; i < all_moves.size(); i++)
	{
		all_future_moves.push_back(pair<Point_2D, Point_2D>(all_moves[i], all_moves[i]));		// Первая пара - самый первый ход, а вторая пара - будущие ходы
	}

	while (max_count)
	{
		max_ratio = -999;
		for (auto it = all_future_moves.begin(); it < all_future_moves.end(); it++)
		{
			if (it->second.ratio != 0)	// Удаляем отрицательные коэффиценты (потому что при них шашка гибнет после первого же хода), положительных коэффицентов здесь нет, потому что они учитываются в условии выхода выше
				it = all_future_moves.erase(it);
		}
		temp_all_future_moves.clear();

		for (int i = 0; i < all_future_moves.size(); ++i)
		{
			this->board[all_future_moves[i].first.from.y][all_future_moves[i].first.from.x]->get_possible_moveS(this->board, Point(all_future_moves[i].second.to.x, all_future_moves[i].second.to.y), temp_moves);
			for (int z = 0; z < temp_moves.size(); z++)
			{
				temp_moves[z].enemy_kills_after_move = _enemy_kills_after_move(Point_2D(temp_moves[z].to, all_future_moves[i].first.from));
				temp_moves[z].ratio = _calc_ratio(temp_moves[z]);
				temp_all_future_moves.push_back(pair<Point_2D, Point_2D>(all_future_moves[i].first, temp_moves[z]));
			}
			temp_moves.clear();
		}

		for (int z = 0; z < temp_all_future_moves.size(); z++)
		{
			if (temp_all_future_moves[z].second.ratio > max_ratio)
			{
				max_ratio = temp_all_future_moves[z].second.ratio;
				res = temp_all_future_moves[z].first;
			}
		}
		if (max_ratio != 0)
			break;

		all_future_moves = temp_all_future_moves;
		--max_count;
	}

	return res;
}

int Board::_enemy_kills_after_move(const Point_2D& _point) const
{
	Board board = *this;	// копия доски

	bool color = board.board[_point.from.y][_point.from.x]->isWhite();	// цвет ходящей фигуры

	// делаем ход
	board.board[_point.from.y][_point.from.x]->~Figure();
	board.board[_point.from.y][_point.from.x] = nullptr;

	board.board[_point.to.y][_point.to.x] = new Checher(color);

	vector<Point_2D> temp_moves;
	Point_2D best_enemy_move;
	bool enemy_color = !color;
	for (int i = 7; i >= 0; --i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (board.board[i][j] == nullptr || board.board[i][j]->isWhite() != enemy_color) continue;
			temp_moves.clear();
			board.board[i][j]->get_possible_moveS(board.board, Point(j, i), temp_moves);
			for (int z = 0; z < temp_moves.size(); z++)
			{
				if (temp_moves[z] > best_enemy_move)
					best_enemy_move = temp_moves[z];
			}
		}
	}

	// Если вражеские фигуры убивают фигуру из-за которой был совершен ход, ворачиваем число гибнущих из-за этого фигур, иначе 0
	if (find(best_enemy_move.kills.begin(), best_enemy_move.kills.end(), Point(_point.to.x, _point.to.y)) == best_enemy_move.kills.end())
		return 0;
	else
		return best_enemy_move.kills.size();
}

int Board::_calc_ratio(const Point_2D& _point) const
{
	if (_point.enemy_kills_after_move)	// Если теряем больше, чем уничтожаем, то коэффициент < 0
	{
		return _point.kills.size() - _point.enemy_kills_after_move;
	}
	else	// Если смертей в результате хода нет - повышенный коэффициент
	{
		return _point.kills.size() * 2;
	}
}

void Board::do_move(const Point_2D& _move_point)
{
	if (_move_point.to > Point(7,7) || _move_point.from > Point(7, 7) || _move_point.to < Point(0, 0) || _move_point.from < Point(0, 0))
		throw "Error points in parameter  (from 'do_move')";

	// Передвигаем ходящую фигуру
	this->board[_move_point.to.y][_move_point.to.x] = this->board[_move_point.from.y][_move_point.from.x];
	this->board[_move_point.from.y][_move_point.from.x]->~Figure();
	this->board[_move_point.from.y][_move_point.from.x] = nullptr;

	// Затираем пострадавших
	for (int i = 0; i < _move_point.kills.size(); i++)
	{
		if (this->board[_move_point.kills[i].y][_move_point.kills[i].x] == nullptr) throw "Error points of kills in parameter  (from 'do_move')";
		this->board[_move_point.kills[i].y][_move_point.kills[i].x]->~Figure();
		this->board[_move_point.kills[i].y][_move_point.kills[i].x] = nullptr;
	}

	// Обновление количества фигур на доске
	this->count -= _move_point.kills.size();
	if (!this->board[_move_point.to.y][_move_point.to.x]->isWhite()) this->count_white -= _move_point.kills.size();

	// Условие окончания игры
	if (this->count_white == 0 || this->count - this->count_white == 0)
		this->_end_game = true;

	// Условие на то, что шашка стала королевой (дамка)
	if ((_move_point.to.y == 7 && this->board[_move_point.to.y][_move_point.to.x]->isWhite()) || (_move_point.to.y == 0 && !this->board[_move_point.to.y][_move_point.to.x]->isWhite()))
	{
		bool color_figure = this->board[_move_point.to.y][_move_point.to.x]->isWhite();
		this->board[_move_point.to.y][_move_point.to.x]->~Figure();
		this->board[_move_point.to.y][_move_point.to.x] = new King(color_figure);
	}

	return;
}

void Board::notation(const Point_2D& _point, ostream& _out) const
{
	if (this->board[_point.from.y][_point.from.x] == nullptr)
	{
		throw "Error point in 'notation'!";
	}

	bool color_move_figure = this->board[_point.from.y][_point.from.x]->isWhite();

	if (color_move_figure)
	{
		_out << "White: ";
	}
	else
	{
		_out << "Black: ";
	}

	_out << char(_point.from.x + 65) << _point.from.y + 1;
	_out << " -> ";
	_out << char(_point.to.x + 65) << _point.to.y + 1;
	_out << " || Kills === " << _point.kills.size();

	if (_point.kills.size())
	{
		_out << " (" << char(_point.kills[0].x + 65) << _point.kills[0].y + 1;
		for (int i = 1; i < _point.kills.size(); ++i)
		{
			_out << ", " << char(_point.kills[i].x + 65) << _point.kills[i].y + 1;
		}
		_out << ")";
	}
	
	_out << endl;

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

Board& Board::operator=(const Board& _board)
{
	this->count = _board.count;
	this->count_white = _board.count_white;
	this->_end_game = _board._end_game;

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			if (_board.board[i][j] == nullptr)
				this->board[i][j] = nullptr;
			else if (_board.board[i][j]->isKing())
				this->board[i][j] = new King(_board.board[i][j]->isWhite());
			else
				this->board[i][j] = new Checher(_board.board[i][j]->isWhite());
		}
	}

	return *this;
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
					out << 'M';	// Метка на королеву
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