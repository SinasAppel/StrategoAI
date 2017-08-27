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
	return 0;
}

Move JurAI::move(Tile field[10][10], Move opponent_move)
{
	Move output;
	return output;
}

