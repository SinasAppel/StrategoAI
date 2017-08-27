#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include "generating.h"
#include "JurAI.h"
using namespace std;

/**
 * Start_pos startPos() returns the starting position when called.
 */
Start_pos JurAI::startPos()
{
	srand(time(0));
	// Create eventual output variable
	Start_pos output;
	
	// Initial array storing values of pieces
	char piece_names[40] = {'F', 'B', 'B', 'B', 'B', 'B', 'B', '1', 
		'2', '2', '2', '2', '2', '2', '2', '2', '3', '3', '3', '3', 
		'3', '4', '4', '4', '4', '5', '5', '5', '5', '6', '6', '6', 
		'6', '7', '7', '7', '8', '8', '9', 'T'};
	
	// Piece array that will contain 
	// the pieces generated using piece_names array
	Piece list_of_pieces[40];

	// Generate Pieces
	for (int i = 0; i < 40; i++) {
		Piece piece(piece_names[i]);
		list_of_pieces[i] = piece;
	}
	
	// Fit the array of pieces into the Start_pos struct used by the base game
	for (int T3 = 0; T3 < 10; T3++) {
		output.row0[T3] = list_of_pieces[T3];
		output.row1[T3] = list_of_pieces[10 + T3];
		output.row2[T3] = list_of_pieces[20 + T3];
		output.row3[T3] = list_of_pieces[30 + T3];
	}

	return output;
}

int JurAI::evaluate_tile(Tile target, int piece_falue)
{
	if (target.piece.owner == playerNumber){ return -900; }
	if (target.land == 'W'){ return -900; }
	if (target.piece.owner != playerNumber){ return 10; }
	if (target.piece.value == -1){ return 1; }
	else { return 0; }
}

Move JurAI::move(Tile field[10][10], Move opponent_move)
{
	Move output;

	//update de hasmoved map
	if (opponent_move.x != -1){
		int opp_des_x, opp_des_y;
		if (opponent_move.cardinal == 'N'){ opp_des_x = opponent_move.x; opp_des_y = opponent_move.y - 1; }
		else if (opponent_move.cardinal == 'E'){ opp_des_x = opponent_move.x + 1; opp_des_y = opponent_move.y; }
		else if (opponent_move.cardinal == 'S'){ opp_des_x = opponent_move.x; opp_des_y = opponent_move.y + 1; }
		else if (opponent_move.cardinal == 'W'){ opp_des_x = opponent_move.x - 1; opp_des_y = opponent_move.y; }
		hasmoved[opp_des_x][opp_des_y] = 1;
	}

	int max = -10000, rating;
	char best = 'N';
	int best_tile[2] = {-1, -1};
	for (int T1 = 0; T1 < 10; T1++){
		for (int T2 = 0; T2 < 10; T2++){
			if (field[T1][T2].piece.owner == playerNumber && field[T1][T2].piece.value != 0)//evaluate the moves of the piece it the Ai ownse it and it is not a flag or a bom
			{
				Tile target;
				if (T1 != 0){
					target = field[T1 - 1][T2];// get the target field to evaluate
					rating = evaluate_tile(target, field[T1][T2].piece.value);// get the avaluation
					if (rating > max){ max = rating;  best = 'N'; best_tile[0] = T1; best_tile[1] = T2; }// if it is max move there
				}

				if (T1 != 10){
					target = field[T1 + 1][T2];
					rating = evaluate_tile(target, field[T1][T2].piece.value);
					if (rating > max){ max = rating;  best = 'S'; best_tile[0] = T1; best_tile[1] = T2; }
				}

				if (T2 != 0){
					target = field[T1][T2 - 1];
					rating = evaluate_tile(target, field[T1][T2].piece.value);
					if (rating > max){ max = rating;  best = 'W'; best_tile[0] = T1; best_tile[1] = T2; }
				}

				if (T1 != 10){
					target = field[T1][T2 + 1];
					rating = evaluate_tile(target, field[T1][T2].piece.value);
					if (rating > max){ max = rating;  best = 'E'; best_tile[0] = T1; best_tile[1] = T2; }
				}
			}
		}
	}
	output.cardinal = best;
	output.x = best_tile[1];
	output.y = best_tile[0];
	printf("AI%i: %i, %i, %c\n", playerNumber, output.x, output.y, output.cardinal);
	return output;
}

