#pragma once
#include "Figure.h"

class Checher : public Figure
{
public:
	Checher();

	Checher(const bool _isWhite);

	virtual bool isWhite() const override;

	virtual void _get_more_kills(Figure*** _board, const Point& _point, vector<Point_2D>& _res) override;

	virtual void get_possible_moveS(Figure*** _board, const Point& _point, vector<Point_2D>& _res) override;
	
	~Checher();
};