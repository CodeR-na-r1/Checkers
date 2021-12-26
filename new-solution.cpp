#include <iostream>

#include "Board.h"
#include "Checher.h"
#include "King.h"
#include "figure.h"

int main()
{
	Board* board = new Board("data_in.txt");
	
	cout << *board;

	return 0;
}