#include "Checher.h"
#include "board.h"

Checher::Checher() : Figure() {};

Checher::Checher(const bool _isWhite) : Figure()
{
	this->white = _isWhite;
}

bool Checher::isWhite() const
{
	return this->white;
}

void Checher::_get_more_kills(Figure*** _board, const bool enemy_color_isWhite, const Point& _point, vector<Point_2D>& _res, vector<Point_2D>& best_kills)
{
	
}

void Checher::get_possible_moveS(Figure*** _board, const bool enemy_color_isWhite, const Point& _point, vector<Point_2D>& _res)
{
	//if (_board[_point.y][_point.x] == nullptr)
		//throw "There is no figure: error in _point (parametr) (exception in Checher::get_possible_moveS)";

	vector<Point_2D> max_kills;
	_get_more_kills(_board, enemy_color_isWhite, _point, _res, max_kills);

	//if (max_kills.size())
		//return 
	if (_board[_point.y][_point.x]->isWhite() == true)
	{
		if (_point.y + 1 < 8 && _point.x + 1 < 8)
		{
			if (_board[_point.y + 1][_point.x + 1] == nullptr)
				_res.push_back(Point_2D(Point(_point.x + 1, _point.y + 1), Point(_point.x, _point.y)));
			//else
				//this->_get_more_kills(_board, )
		}
			

		if (_point.y + 1 < 8 && _point.x - 1 < 8 && _board[_point.y + 1][_point.x - 1] == nullptr)
			_res.push_back(Point_2D(Point(_point.x - 1, _point.y + 1), Point(_point.x, _point.y)));
	}
	else
	{

	}

	return;
}

Checher::~Checher()
{

}