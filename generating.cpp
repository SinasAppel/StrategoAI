#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include "generating.h"
#include "AI1.h"

// Standard constructor for a Piece, creates an empty piece.
Piece::Piece() {
	value = -1;
	name = 'E';
	owner = 0;
	visible = false;
}

// Standard constructor for a Move, creates an empty Move.
Move::Move() {
	x = -1;
	y = -1;
	cardinal = '0';
	no_moves = false;
}
// Standard constructor for a Turn, creates an empty Turn.
Turn::Turn() {
	count = 0;
	you_moved = Move();
	opponent_moved = Move();
	you_killed[0] = Piece();
	you_killed[1] = Piece();
	opponent_killed[0] = Piece();
	opponent_killed[1] = Piece();
	you_revealed = Piece();
	opponent_revealed = Piece();
}
// special constructor to call each new turn
Turn::Turn(Turn players_turn, Turn opponents_turn) {
	count = opponents_turn.count + 1;
	you_moved = players_turn.you_moved;
	opponent_moved = opponents_turn.you_moved;
	you_killed[0] = players_turn.you_killed[0];
	you_killed[1] = players_turn.you_killed[1];
	opponent_killed[0] = opponents_turn.you_killed[0];
	opponent_killed[1] = opponents_turn.you_killed[1];
	you_revealed = players_turn.you_revealed;
	opponent_revealed = opponents_turn.you_revealed;;
}

/*
 * Special constructor for Piece.
 * Takes a char n, that denotes the name of the Piece.
 * For example '5' corresponds to the Five (AKA Lieutenant) Piece.
 */
Piece::Piece(char n, int o) {
	name = n;
	owner = o;
	visible = false;
	if (n == 'B' || n == 'F') { // Bomb or flag, both have the same value
		value = 0;
	} else if (n == 'T') { // Ten (AKA Marshal)
		value = 10;
	} else { // Name equals value for all other pieces
		value = name - '0'; // Handy trick to convert chars to ints
	}	
}

/*
 * Special constructor for Piece
 * Takes an int v, that denotes the value of the Piece
 * In this case, 0 is the flag and 11 is the bomb
 */
Piece::Piece(int v, int o) {
	v == 11 ? value = 0 : value = v;
	owner = o;
	visible = false;
	if (v == 11) {
		name = 'B';
	} else if (v == 0) {
		name = 'F';
	} else if (v == 10) {
		name = 'T';
	} else {
		name = value + '0';
	}
}

// Default constructor for a Grass tile
Grass::Grass() {
	land = 'G';
	Piece p;
	piece = p;
}

// Default constructor for a Water tile
Water::Water() {
	land = 'W';
	Piece p;
	piece = p;
}

// Default constructor of a default AI
AI::AI(int p) {
	playerNumber = p;
}

//Creates the initial empty board
void createBoard(Tile field[10][10])
{
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if ((i == 4 || i == 5) && (j == 2 || j == 3 || j == 6 || j == 7)) {
				field[i][j] = cleanWaterTile();
			}
			else {
				field[i][j] = cleanGrassTile();
			}
		}
	}
}
/*
 * fillBoard handles the starting positions of the AI('s).
 * Manipulates the field of the base game.
 */
void fillBoard(Tile field[10][10], Start_pos startPos1, Start_pos startPos2)
{
	for (int i = 0; i < 10; i++) {
		int j = 10 - i - 1;
		field[0][i].piece = startPos1.row0[i];
		field[1][i].piece = startPos1.row1[i];
		field[2][i].piece = startPos1.row2[i];
		field[3][i].piece = startPos1.row3[i];
		field[6][i].piece = startPos2.row3[j];
		field[7][i].piece = startPos2.row2[j];
		field[8][i].piece = startPos2.row1[j];
		field[9][i].piece = startPos2.row0[j];
	}
}

/*
 * Creates an empty grass tile
 */
Tile cleanGrassTile()
{
	Grass g;
	return g;
}

/*
 * Creates an empty water tile
 */
Tile cleanWaterTile()
{
	Water w;
	return w;
}