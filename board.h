#pragma once
#include "Figure.h"

#include <fstream>
#include <string>

using namespace std;

class Board
{
public:
	Figure*** board;
	
	Board();
	Board(const char* namefile);

	friend ostream& operator<<(ostream& out, const Board& board);

	~Board();
};