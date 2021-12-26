#pragma once

#include "Board.h"
#include "Checher.h"

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

ostream& operator<<(ostream& out, const Board& _board)
{
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

Board::~Board()
{
	for (int i = 0; i < 8; ++i)
	{
		delete this->board[i];
	}

	delete board;

	return;
}