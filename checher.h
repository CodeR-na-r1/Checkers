#pragma once
#include "Figure.h"

class Checher : public Figure
{
public:
	Checher();

	Checher(const bool _isWhite);

	virtual bool isWhite() override;
	
	~Checher();
};