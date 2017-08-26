#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include "generating.h"
#include "AI1.h"
using namespace std;

/**
 * Gives the base game the starting position
 * @return Start_pos 
 */
Start_pos AI1::startPos() 
{
	srand(time(0));
	Start_pos output;
	int piece_values[40] = {11, 12, 12, 12, 12, 12, 12, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 8, 8, 9, 10};
	Piece list_of_pieces[40];
	string b;

	for (int T1 = 0; T1 < 40; T1++)
	{
		int R1 = rand() % 40, R2 = rand() % 40;
		int temp = piece_values[R1];
		piece_values[R1] = piece_values[R2];
		piece_values[R2] = temp;
	}

	for (int T2 = 0; T2 < 40; T2++)
	{
		if (piece_values[T2] != 11 && piece_values[T2] != 12)
		{
			list_of_pieces[T2].value = piece_values[T2];
			if (piece_values[T2] == 10)
			{
				list_of_pieces[T2].name = 'T';
			}
			else
			{
				b = to_string(piece_values[T2]);
				list_of_pieces[T2].name = b[0];
			}
		}
		else if (piece_values[T2] == 11)
		{
			list_of_pieces[T2].value = 0;
			list_of_pieces[T2].name = 'F';
		}
		else 
		{
			list_of_pieces[T2].value = 0;
			list_of_pieces[T2].name = 'B';
		}
	}

	for (int T3 = 0; T3 < 10; T3++)
	{
		output.row0[T3] = list_of_pieces[T3];
		output.row1[T3] = list_of_pieces[10 + T3];
		output.row2[T3] = list_of_pieces[20 + T3];
		output.row3[T3] = list_of_pieces[30 + T3];
	}

	return output;
}

Move AI1::move() 
{
	Move move;
	return move;
}
