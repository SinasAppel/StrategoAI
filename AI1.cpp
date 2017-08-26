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

int AI1::evaluate_tile(Tile target, int piece_falue)
{
	if (target.piece.value == -1){ return 1; }
	if (target.piece.owner == playerNumber){ return -900; }
	if (target.piece.owner != playerNumber){ return 10; }
}

Move AI1::move(Tile field[10][10], Move opponent_move)
{
	Move output; 

	//update de hasmoved map
	int opp_des_x, opp_des_y;
	if (opponent_move.cardinal == 'N'){ opp_des_x = opponent_move.x; opp_des_y = opponent_move.y-1; }
	else if (opponent_move.cardinal == 'E'){ opp_des_x = opponent_move.x+1; opp_des_y = opponent_move.y; }
	else if (opponent_move.cardinal == 'S'){ opp_des_x = opponent_move.x; opp_des_y = opponent_move.y+1; }
	else if (opponent_move.cardinal == 'W'){ opp_des_x = opponent_move.x-1; opp_des_y = opponent_move.y; }
	hasmoved[opp_des_x][opp_des_y] = 1;


	int max = -10000, rating;
	char best;
	int best_tile[2];
	for (int T1 = 0; T1 < 10; T1++){
		for (int T2 = 0; T2 < 10; T2++){
			if (field[T1][T2].piece.owner == playerNumber && field[T1][T2].piece.value != 0)//evaluate the moves of the piece it the Ai ownse it and it is not a flag or a bom
			{
				Tile target;
				if (T1 != 0){
					target = field[T1 - 1][T2];// get the target field to evaluate
					rating = evaluate_tile(target, field[T1][T2].piece.value);// get the avaluation
					if (rating > max){ max = rating;  best = 'N'; best_tile[0] = T1 - 1; best_tile[1] = T2; }// if it is max move there
				}

				if (T1 != 10){
					target = field[T1 + 1][T2];
					rating = evaluate_tile(target, field[T1][T2].piece.value);
					if (rating > max){ max = rating;  best = 'S'; best_tile[0] = T1 + 1; best_tile[1] = T2; }
				}

				if (T2 != 0){
					target = field[T1][T2 - 1];
					rating = evaluate_tile(target, field[T1][T2].piece.value);
					if (rating > max){ max = rating;  best = 'W'; best_tile[0] = T1; best_tile[1] = T2 - 1; }
				}

				if (T1 != 10){
					target = field[T1][T2 + 1];
					rating = evaluate_tile(target, field[T1][T2].piece.value);
					if (rating > max){ max = rating;  best = 'E'; best_tile[0] = T1; best_tile[1] = T2 + 1; }
				}
			}
		}
	}
	output.cardinal = best;
	output.x = best_tile[1];
	output.y = best_tile[0];
	return output;
}
