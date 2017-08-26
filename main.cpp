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
	} else if (attacker.piece.value == defender.piece.value) {
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
	int newX = 0;
	int newY = 0;
	switch (move.cardinal) {
		case 'N': // Piece should be moved North
			newX = move.x-1;
			newY = move.y;
			break;
		case 'E': // Piece should be moved East
			newX = move.x;
			newY = move.y+1;
			break;
		case 'S': // Piece should be moved South
			newX = move.x+1;
			newY = move.y;
			break;
		case 'W': // Piece should be moved West
			newX = move.x;
			newY = move.y-1;
			break;
		default:
			printf("Error, not a valid cardinal\n");
			break;
	}
	switch(combatScore(field[move.x][move.y], field[newX][newY])) {
		case 2:  return field[move.x][move.y].piece.owner; break;
		case 1:  field[newX][newY] = field[move.x][move.y];
				 field[move.x][move.y] = cleanGrassTile(); break;
		case 0:  field[newX][newY] = cleanGrassTile();
		case -1: field[move.x][move.y] = cleanGrassTile(); break;
		default: printf("Not a valid combat score!\n"); break;
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
