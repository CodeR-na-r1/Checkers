#pragma once

#include <iostream>
#include <fstream>

#include "Board.h"

int main()
{
	Board* board = new Board("data_in.txt");
	
	cout << *board << endl << endl;
	
	bool who_move = true;	// цвет ходящей фигуры
	int draw = 0;	// Счетчик условия ничьи
	
	ofstream out;
	out.open("notation_output.txt");	// файл с нотациями

	while (true)
	{
		Point_2D move = board->get_effective_move(who_move);	// Получение лучшего хода для фигур конкретного цвета
		board->notation(move, cout);	// Выввод нотаций на экран
		board->notation(move, out);	// Выввод нотаций в файл
		board->do_move(move);	// Делаем ход

		if (board->end_game())	// Условие окончания партии
			break;

		who_move = !who_move;	// Смена цвета ходящей фигуры
		cout << *board << endl << endl;
		if (++draw > 31)	// Условие ничьи
			break;
	}

	out.close();

	cout << *board << endl << endl;

	if (draw > 31)	// Выввод результата окончания партии
		cout << "no one won! (draw)\n";
	else
		board->white_is_win() ? cout << "White is win!\n" : cout << "Black is win!\n";

	out.open("output.txt");
	out << *board;
	out.close();

	return 0;
}