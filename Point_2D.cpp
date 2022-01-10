#pragma once

#include "Point_2D.h"

Point_2D::Point_2D()
{
	this->enemy_kills_after_move = 0;
	this->ratio = 0;
}

Point_2D::Point_2D(Point _to, Point _from) : Point_2D()
{
	this->to = _to;
	this->from = _from;
}

Point_2D::Point_2D(Point _to, Point _from, vector<Point>& _kills) : Point_2D(_to, _from)
{
	for (int i = 0; i < _kills.size(); ++i)
	{
		this->kills.push_back(_kills[i]);
	}

	return;
}

bool Point_2D::operator>(const Point_2D& _other)
{
	return this->kills.size() > _other.kills.size();
}