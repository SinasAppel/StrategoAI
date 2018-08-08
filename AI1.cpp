#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include "generating.h"
#include "AI1.h"
#include "definitions.cpp"
using namespace std;

// Standard constructor for AI1
AI1::AI1(int p) : AI(p) {
}

// gives the base game the starting position.
Start_pos AI1::startPos() {
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
		list_of_pieces[T2].owner = playerNumber;
		list_of_pieces[T2].visible = false;
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
	if (target.piece.owner == playerNumber){ return -900; }
	if (target.land == 'W'){ return -900; }
	if (target.piece.value > piece_falue){ return -900; }
	if (target.piece.value == -1){ return 2; }
	if (target.piece.value == -2)
	{
		return 3;
	}
	else 
	{
		if (target.piece.value < piece_falue){ int V1 = target.piece.value, V2 = piece_falue; int V3 = V2 - V1; return V3; }
		if (target.piece.value == piece_falue){ return 1; }
	}
	 return 0; 
}

Move AI1::move(Tile field[10][10], Move opponent_move, Turn turn)
{
	Move output;

	//update de hasmoved map
	if (opponent_move.x != -1){
		int opp_des_x, opp_des_y;
		if (opponent_move.cardinal == NORTH){ opp_des_x = opponent_move.x; opp_des_y = opponent_move.y - 1; }
		else if (opponent_move.cardinal == EAST){ opp_des_x = opponent_move.x + 1; opp_des_y = opponent_move.y; }
		else if (opponent_move.cardinal == SOUTH){ opp_des_x = opponent_move.x; opp_des_y = opponent_move.y + 1; }
		else if (opponent_move.cardinal == WEST){ opp_des_x = opponent_move.x - 1; opp_des_y = opponent_move.y; }
		hasmoved[opp_des_x][opp_des_y] = 1;
	}

	int rating[100] = {}, M = 0, value, max = -10000, value2 = -10;
	Move moveCat[100];
	for (int T1 = 0; T1 < 10; T1++){
		for (int T2 = 0; T2 < 10; T2++){
			if (field[T1][T2].piece.owner == playerNumber && field[T1][T2].piece.value != 0)//evaluate the moves of the piece it the Ai ownse it and it is not a flag or a bom
			{
				Tile target;
				if (T1 != 0){
					target = field[T1 - 1][T2];// get the target field to evaluate
					value = evaluate_tile(target, field[T1][T2].piece.value);// get the avaluation
					if (value >= max && value > -10 && value < 100){ max = value;  rating[M] = value; moveCat[M].x = T2; moveCat[M].y = T1; moveCat[M].cardinal = NORTH; M++; }
				}

				if (T1 != 10){
					target = field[T1 + 1][T2];
					value = evaluate_tile(target, field[T1][T2].piece.value);
					if (value >= max && value > -10 && value < 100){ max = value; rating[M] = value; moveCat[M].x = T2; moveCat[M].y = T1; moveCat[M].cardinal = SOUTH;  M++; }
				}

				if (T2 != 0){
					target = field[T1][T2 - 1];
					value = evaluate_tile(target, field[T1][T2].piece.value);
					if (value >= max && value > -10 && value < 100){ max = value; rating[M] = value; moveCat[M].x = T2; moveCat[M].y = T1; moveCat[M].cardinal = WEST;  M++; }
				}

				if (T1 != 10){
					target = field[T1][T2 + 1];
					value = evaluate_tile(target, field[T1][T2].piece.value);
					if (value >= max && value > -10 && value < 100){ max = value; rating[M] = value; moveCat[M].x = T2; moveCat[M].y = T1; moveCat[M].cardinal = EAST;  M++; }
				}
			}
		}
	}
	
	int R1, D = 0;
	if (M == 0){ output.noMoves = true; return output; }// nomoves so forfit
	for (int T3 = 0; T3 < M; T3++)
	{
		if (rating[T3] != max){ D++; }
	}

	R1 = (rand() % (M - D)) + D;
	value2 = rating[R1];
	moveCat[R1].noMoves = false;
	output = moveCat[R1];

	//printf("AI%i: %i, %i, %c\n", playerNumber, output.x, output.y, output.cardinal);
	return output;
}
