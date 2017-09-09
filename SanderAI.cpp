#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include "generating.h"
#include "SanderAI.h"
using namespace std;

// Standard constructor for AI1
SanderAI::SanderAI(int p) {
	playerNumber = p;
}

// gives a random unice piece from an array
Piece SanderAI::rand_from_array(int in[], int size)
{
	int V = -1, R;
	while (V == -1)
	{
		R = rand() % size;
		V = in[R];
	}
	in[R] = -1;
	Piece p = Piece(V, playerNumber);
	return p;
}

//updates the private frontline arrays
void SanderAI::update_frontline(Tile field[10][10])
{
	int T3;
	for (int T1 = 9; T1 > 0; T1--){
		T3 = 9 - T1;
		for (int T2 = 0; T2 < 10; T2++){
			if (field[T1][T2].piece.owner == playerNumber){
				Frontline_My[T2] = field[T1][T2].piece;
			}
			
			if (field[T3][T2].piece.owner == (playerNumber == 1 ? 2 : 1)){
				Frontline_Opponent[T2] = field[T3][T2].piece;
			}
		}
	}
}

// gives the base game the starting position.
Start_pos SanderAI::startPos()
{
	srand(time(0));
	Start_pos output;
	int row01[6] = { 0, 4, 4, 4, 5, 5 }, row02[4] = { 1, 8, 9, 10 };
	int row1[4] = { 3, 3, 3, 4 };
	int row2[4] = { 3, 3, 5, 5 };
	int row3[8] = { 6, 6, 6, 6, 7, 7, 7, 8 };

	for (int T1 = 0; T1 < 10; T1++)
	{
		//making row0
		if (T1 < 3 || T1 > 6){
			output.row0[T1] = rand_from_array(row01, 6);
		}
		if (T1 >= 3 && T1 <= 6){
			output.row0[T1] = rand_from_array(row02, 4);
		}
		//making row1
		if (T1 < 3 || T1 > 6){ output.row1[T1] = Piece(11, playerNumber); }
		if (T1 >= 3 && T1 <= 6){
			output.row1[T1] = rand_from_array(row1, 4);
		}
		//making row2
		if (T1 < 2 || T1 > 7 || T1 == 4 || T1 == 5){output.row2[T1] = Piece(2, playerNumber);}
		if (output.row2[T1].value != 2){
			output.row2[T1] = rand_from_array(row2, 4);
		}
		//making row3
		if (T1 == 3 || T1 == 6){ output.row3[T1] = Piece(2, playerNumber); }
		if (output.row3[T1].value != 2){
			output.row3[T1] = rand_from_array(row3, 8);
		}

	}
	return output;
}

Move SanderAI::move(Tile field[10][10], Move opponent_move)
{
	Move output;

	update_frontline(field);

	//update de hasmoved map
	if (opponent_move.x != -1){
		int opp_des_x, opp_des_y;
		if (opponent_move.cardinal == 'N'){ opp_des_x = opponent_move.x; opp_des_y = opponent_move.y - 1; }
		else if (opponent_move.cardinal == 'E'){ opp_des_x = opponent_move.x + 1; opp_des_y = opponent_move.y; }
		else if (opponent_move.cardinal == 'S'){ opp_des_x = opponent_move.x; opp_des_y = opponent_move.y + 1; }
		else if (opponent_move.cardinal == 'W'){ opp_des_x = opponent_move.x - 1; opp_des_y = opponent_move.y; }
		hasmoved[opp_des_x][opp_des_y] = 1;
	}

	//steps
	//breatch through the small pieces
	//reviele the big pieces
	//make you pieces star and plus
	// attack all that moves

	output.no_moves = true;
	//printf("AI%i: %i, %i, %c\n", playerNumber, output.x, output.y, output.cardinal);
	return output;
}
