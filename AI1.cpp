#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include "generating.h"
#include "AI1.h"

/**
 * Gives the base game the starting position
 * @return Start_pos 
 */
Start_pos AI1::startPos() 
{
	srand(time(0));
	Start_pos output;
	int piece_values[40] = {11, 12, 12, 12, 12, 12, 12, 
		1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4,
		4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 8, 8, 9, 10};
	Piece list_of_pieces[40];
	// make all the pieces in a array
	for (int T1 = 0; T1 < 40; T1++)
	{
		if (piece_values[T1] != 11 && piece_values[T1] != 12)
		{
			list_of_pieces[T1].value = piece_values[T1];
			list_of_pieces[T1].name = 'N';
		}
		else if (piece_values[T1] == 11)
		{
			list_of_pieces[T1].value = 0;
			list_of_pieces[T1].name = 'F';
		}
		else 
		{
			list_of_pieces[T1].value = 0;
			list_of_pieces[T1].name = 'B';
		}
	}
	//transver array to start_pos
	for (int T2 = 0; T2 < 10; T2++)
	{
		output.row0[T2] = list_of_pieces[T2];
		output.row1[T2] = list_of_pieces[10 + T2];
		output.row2[T2] = list_of_pieces[20 + T2];
		output.row3[T2] = list_of_pieces[30 + T2];
	}

	return output;
}

Move AI1::move() 
{
	Move move;
	return move;
}
