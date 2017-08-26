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
	int piece_values[40] = {11, 12, 12, 12, 12, 12, 12, 
		1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4,
		4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 8, 8, 9, 10};
	Piece list_of_pieces[40];
	string b;
	for (int T1 = 0; T1 < 40; T1++)
	{
		int R1 = rand() % 40, R2 = rand() % 40;
		int temp = getallen[R1];
		getallen[R1] = getallen[R2];
		getallen[R2] = temp;
	}
	
	// make all the pieces in a array
	for (int T2 = 0; T2 < 40; T2++)
	{
		if (getallen[T2] != 11 && getallen[T2] != 12)
		{
			stukken[T2].value = getallen[T2];
			if (getallen[T2] == 10)
			{
				stukken[T2].name = 'T';
			}
			else
			{
				b = to_string(getallen[T2]);
				stukken[T2].name = b[0];
			}
		}
		else if (getallen[T2] == 11)
		{
			stukken[T2].value = 0;
			stukken[T2].name = 'F';
		}
		else 
		{
			stukken[T2].value = 0;
			stukken[T2].name = 'B';
		}
	}
	
	//transver array to start_pos
	for (int T3 = 0; T3 < 10; T3++)
	{
		uit.row0[T3] = stukken[T3];
		uit.row1[T3] = stukken[10 + T3];
		uit.row2[T3] = stukken[20 + T3];
		uit.row3[T3] = stukken[30 + T3];
	}

	return output;
}

Move AI1::move() 
{
	Move move;
	return move;
}
