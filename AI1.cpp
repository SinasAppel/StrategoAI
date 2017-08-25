#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include "generating.h"

// gives the base game the starting position.
Start_pos AI1_start_pos_gen()
{
	srand(time(0));
	Start_pos uit;
	int getallen[40] = {11, 12, 12, 12, 12, 12, 12, 1, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6, 7, 7, 7, 8, 8, 9, 10};
	Piece stukken[40];
	// make all the pieces in a array
	for (int T1 = 0; T1 < 40; T1++)
	{
		if (getallen[T1] != 11 && getallen[T1] != 12)
		{
			stukken[T1].value = getallen[T1];
			stukken[T1].name = 'N';
		}
		else if (getallen[T1] == 11)
		{
			stukken[T1].value = 0;
			stukken[T1].name = 'F';
		}
		else 
		{
			stukken[T1].value = 0;
			stukken[T1].name = 'B';
		}
	}
	//transver array to start_pos
	for (int T2 = 0; T2 < 10; T2++)
	{
		uit.row0[T2] = stukken[T2];
		uit.row1[T2] = stukken[10 + T2];
		uit.row2[T2] = stukken[20 + T2];
		uit.row3[T2] = stukken[30 + T2];
	}

	return uit;
}
