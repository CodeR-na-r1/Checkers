#include "Checher.h"

Checher::Checher() : Figure() {};

Checher::Checher(const bool _isWhite) : Figure()
{
	this->white = _isWhite;
}

bool Checher::isWhite()
{
	return this->white;
}

Checher::~Checher()
{

}