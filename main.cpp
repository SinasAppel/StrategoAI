#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "generating.h"
using namespace std;

/**
* Prints the current board
*/
void printField(Tile field[10][10]) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (field[i][j].piece.value < 0 || field[i][j].piece.value > 10) {
				printf(" %c", field[i][j].land);
			}
			else {
				printf(" %c", field[i][j].piece.name);
			}
		}
		printf("\n");
	}
}

/**
 * Evaluates combat
 * Returns -1 if lost, 0 if draw, 1 if win and 2 if flag is hit.
 */
int combatScore(Tile attacker, Tile defender) 
{
	// if defender is the vlag
	if (defender.piece.value == 12) {
		return 2;
	}
	// 3 against bomb:
	if (attacker.piece.value == 3 && defender.piece.value == 12) {
		return 1;
	}
	// 1 against 10:
	if (attacker.piece.value == 1 && defender.piece.value == 10) {
		return 1;
	}
	// normal combat
	if (attacker.piece.value > defender.piece.value) {
		return 1;
	} else if { attack.piece.value == defender.piece.value) {
		return 0;
	} else {
		return -1;
	}
}
/**
 * Handles the move from the AI or player
 * returns the player who won or 0, if the flag has not beed attacked
 */
int handleMove(Tile field[10][10], Move move) 
{
	switch (move.cardinal) {
		case 'N': // Piece should be moved North
			switch(combatScore(field[move.x][move.y], field[move.x-1][move.y]) {
				case 2:  return field[move.x][move.y].piece.owner; break;
				case 1:  field[move.x-1][move.y] = field[move.x][move.y];
						 field[move.x][move.y] = cleanGrassTile(); break;
				case 0:  field[move.x-1][move.y] = cleanGrassTile();
				case -1: field[move.x][move.y] = cleanGrassTile(); break;
				default: printf("Not a valid combat score!\n"); break;
			}
			break;
		case 'E': // Piece should be moved East
			switch(combatScore(field[move.x][move.y], field[move.x][move.y+1]) {
				case 2:  return field[move.x][move.y].piece.owner; break;
				case 1:  field[move.x][move.y+1] = field[move.x][move.y];
						 field[move.x][move.y] = cleanGrassTile(); break;
				case 0:  field[move.x][move.y+1] = cleanGrassTile();
				case -1: field[move.x][move.y] = cleanGrassTile(); break;
				default: printf("Not a valid combat score!\n"); break;
			}
			break;
		case 'S': // Piece should be moved South
			switch(combatScore(field[move.x][move.y], field[move.x+1][move.y]) {
				case 2:  return field[move.x][move.y].piece.owner; break;
				case 1:  field[move.x+1][move.y] = field[move.x][move.y];
						 field[move.x][move.y] = cleanGrassTile(); break;
				case 0:  field[move.x+1][move.y] = cleanGrassTile();
				case -1: field[move.x][move.y] = cleanGrassTile(); break;
				default: printf("Not a valid combat score!\n"); break;
			}
			break;
		case 'W': // Piece should be moved West
			switch(combatScore(field[move.x][move.y], field[move.x][move.y-1]) {
				case 2:  return field[move.x][move.y].piece.owner; break;
				case 1:  field[move.x][move.y-1] = field[move.x][move.y];
						 field[move.x][move.y] = cleanGrassTile(); break;
				case 0:  field[move.x][move.y-1] = cleanGrassTile();
				case -1: field[move.x][move.y] = cleanGrassTile(); break;
				default: printf("Not a valid combat score!\n"); break;
			}
			break;
		default:
			printf("Error, not a valid cardinal\n");
			break;
	}
	return 0;
}

int main() {
	Tile field[10][10];
	createBoard(field);
	fillBoard(field);
	printField(field);
	getchar();
	getchar();
	return 0;
}
