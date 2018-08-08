#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include "generating.h"
#include "JurAI.h"
#include "definitions.cpp"
using namespace std;

/*
 * Default JurAI constructor
 */
JurAI::JurAI(int p) : AI(p){
}

/*
 * Start_pos startPos() returns the starting position when called.
 * Place certain pieces on strategic places.
 * 
 * Currently implements the following rules:
 * Flag on the backmost row, bombs around it.
 * Scouts on non water columns
 */
Start_pos JurAI::startPos() {
	srand(time(0));
	// Create eventual output variable
	Start_pos output;
	
	// Array that stores the pieces left to place
	// 0 = flag, 11 = bomb
	int piecesLeft[12] = {1, 1, 8, 5, 4, 4, 4, 3, 2, 1, 1, 6};
	
	/*

	 */
	 // Place flag
	 int flagPos = rand() % 10;
	 if (piecesLeft[0]) {
		 Piece flag('F', playerNumber);
		 output.row0[flagPos] = flag;
		 piecesLeft[0]--;
	 }
	 // Place bombs around flag
	 if (flagPos > 0 && piecesLeft[11]) {
		 Piece bomb('B', playerNumber);
		 output.row0[flagPos-1] = bomb;
		 piecesLeft[11]--;
	 }
	 if (flagPos < 9 && piecesLeft[11]) {
		 Piece bomb('B', playerNumber);
		 output.row0[flagPos+1] = bomb;
		 piecesLeft[11]--;
	 }
	 if (piecesLeft[11]) {
		 Piece bomb('B', playerNumber);
		 output.row1[flagPos] = bomb;
		 piecesLeft[11]--;
		 
	 }
	 
	 // Place Scouts on non water columns
	 for (int i = 0; i < 8; i++) {
		 // Get column
		 int twoColumn = rand () % 6;
		 if (twoColumn >= 4) {
			 twoColumn += 4;
		 } else if (twoColumn >= 2) {
			 twoColumn += 2;
		 }
		 
		 // Get row
		 int twoRow = rand() % 4;
		 Piece two('2', playerNumber);
		 // Find out if there is a piece already placed on random position
		 // If empty, place piece in there, if not try again
		 if (twoRow == 0) {
			 if (output.row0[twoColumn].value == -1) {
				 if (piecesLeft[2]) {
					 output.row0[twoColumn] = two;
					 piecesLeft[2]--;
				 } else {
					 printf("Error, JurAI tried to place 2 while not have a 2 left\n");
				 }
			 } else {
				 i--;
			 }
		 } else if (twoRow == 1) {
			 if (output.row1[twoColumn].value == -1) {
				 if (piecesLeft[2]) {
					 output.row1[twoColumn] = two;
					 piecesLeft[2]--;
				 } else {
					 printf("Error, JurAI tried to place 2 while not have a 2 left\n");
				 }
			 } else {
				 i--;
			 }
		 } else if (twoRow == 2) {
			 if (output.row2[twoColumn].value == -1) {
				 if (piecesLeft[2]) {
					 output.row2[twoColumn] = two;
					 piecesLeft[2]--;
				 } else {
					 printf("Error, JurAI tried to place 2 while not have a 2 left\n");
				 }
			 } else {
				 i--;
			 }
		 } else {
			 if (output.row3[twoColumn].value == -1) {
				 if (piecesLeft[2]) {
					 output.row3[twoColumn] = two;
					 piecesLeft[2]--;
				 } else {
					 printf("Error, JurAI tried to place 2 while not have a 2 left\n");
				 }
			 } else {
				 i--;
			 }
		 }
	 }
	 
	 // Fill rest of the board randomly
	 
	 // Row 0
	 for (int i=0; i < 10; i++) {
		 if (output.row0[i].value == -1) { // place is empty
			 int r = rand() % 12;
			 if (piecesLeft[r]) {
				 // Still pieces left of chosen piece, place
				 Piece randomPiece(r, playerNumber);
				 output.row0[i] = randomPiece;
				 piecesLeft[r]--;
			 } else {
				 // No piece left of chosen piece, try to fill this space again
				 i--;
			 }
		 }
	 }
	 // Row 1
	 for (int i=0; i < 10; i++) {
		 if (output.row1[i].value == -1) { // place is empty
			 int r = rand() % 12;
			 if (piecesLeft[r]) {
				 // Still pieces left of chosen piece, place
				 Piece randomPiece(r, playerNumber);
				 output.row1[i] = randomPiece;
				 piecesLeft[r]--;
			 } else {
				 // No piece left of chosen piece, try to fill this space again
				 i--;
			 }
		 }
	 }
	 // Row 2
	 for (int i=0; i < 10; i++) {
		 if (output.row2[i].value == -1) { // place is empty
			 int r = rand() % 12;
			 if (piecesLeft[r]) {
				 // Still pieces left of chosen piece, place
				 Piece randomPiece(r, playerNumber);
				 output.row2[i] = randomPiece;
				 piecesLeft[r]--;
			 } else {
				 // No piece left of chosen piece, try to fill this space again
				 i--;
			 }
		 }
	 }
	 // Row 3
	 for (int i=0; i < 10; i++) {
		 if (output.row3[i].value == -1) { // place is empty
			 int r = rand() % 12;
			 if (piecesLeft[r]) {
				 // Still pieces left of chosen piece, place
				 Piece randomPiece(r, playerNumber);
				 output.row3[i] = randomPiece;
				 piecesLeft[r]--;
			 } else {
				 // No piece left of chosen piece, try to fill this space again
				 i--;
			 }
		 }
	 }
	 
	 return output;
}

int JurAI::evaluate_tile(Tile target, int piece_falue) {
	return 0;
}

// Simple function that update the hasMoved array (used in JurAI::move)
void JurAI::updateHasMoved(Move opponent_move) {
	if (opponent_move.x != -1) {
		int opp_des_x, opp_des_y;
		if (opponent_move.cardinal == NORTH){ opp_des_x = opponent_move.x; opp_des_y = opponent_move.y - 1; }
		else if (opponent_move.cardinal == EAST){ opp_des_x = opponent_move.x + 1; opp_des_y = opponent_move.y; }
		else if (opponent_move.cardinal == SOUTH){ opp_des_x = opponent_move.x; opp_des_y = opponent_move.y + 1; }
		else if (opponent_move.cardinal == WEST){ opp_des_x = opponent_move.x - 1; opp_des_y = opponent_move.y; }
		hasmoved[opp_des_x][opp_des_y] = 1;
	}
}

Move JurAI::move(Tile field[10][10], Move opponent_move) {
	updateHasMoved(opponent_move);
	Move output;
	output.x = 0;
	output.y = 0;
	output.cardinal = NORTH;

	return output;
}


