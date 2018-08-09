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

Move AI1::move(Tile field[10][10], Turn turn)
{
	Move output;

	//update de hasmoved map
	if (turn.opponentMoved.x != -1) {
		int opp_des_x, opp_des_y;
		if (turn.opponentMoved.cardinal == 'N') { opp_des_x = turn.opponentMoved.x; opp_des_y = turn.opponentMoved.y - 1; }
		else if (turn.opponentMoved.cardinal == 'E') { opp_des_x = turn.opponentMoved.x + 1; opp_des_y = turn.opponentMoved.y; }
		else if (turn.opponentMoved.cardinal == 'S') { opp_des_x = turn.opponentMoved.x; opp_des_y = turn.opponentMoved.y + 1; }
		else if (turn.opponentMoved.cardinal == 'W') { opp_des_x = turn.opponentMoved.x - 1; opp_des_y = turn.opponentMoved.y; }
		hasmoved[opp_des_x][opp_des_y] = 1;
	}

	int rating[100] = {}, M = 0, value, max = -10000, value2 = -10;
	Move moveCat[100];
	for (int Ty = 0; Ty < 10; Ty++){
		for (int Tx = 0; Tx < 10; Tx++){
			if (field[Ty][Tx].piece.owner == playerNumber && field[Ty][Tx].piece.value != 0)//evaluate the moves of the piece it the Ai ownse it and it is not a flag or a bom
			{
				Tile target;
				if (Ty != 0){
					target = field[Ty - 1][Tx];// get the target field to evaluate
					value = evaluate_tile(target, field[Ty][Tx].piece.value);// get the avaluation
					if (value >= max && value > -10 && value < 100){ 
						max = value;  rating[M] = value; moveCat[M].x = Tx; moveCat[M].y = Ty; moveCat[M].cardinal = NORTH; M++; }
				}

				if (Ty != 9){
					target = field[Ty + 1][Tx];
					value = evaluate_tile(target, field[Ty][Tx].piece.value);
					if (value >= max && value > -10 && value < 100){ 
						max = value; rating[M] = value; moveCat[M].x = Tx; moveCat[M].y = Ty; moveCat[M].cardinal = SOUTH;  M++; }
				}

				if (Tx != 0){
					target = field[Ty][Tx - 1];
					value = evaluate_tile(target, field[Ty][Tx].piece.value);
					if (value >= max && value > -10 && value < 100){ 
						max = value; rating[M] = value; moveCat[M].x = Tx; moveCat[M].y = Ty; moveCat[M].cardinal = WEST;  M++; }
				}

				if (Tx != 9){
					target = field[Ty][Tx + 1];
					value = evaluate_tile(target, field[Ty][Tx].piece.value);
					if (value >= max && value > -10 && value < 100){ 
						max = value; rating[M] = value; moveCat[M].x = Tx; moveCat[M].y = Ty; moveCat[M].cardinal = EAST;  M++; }
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
