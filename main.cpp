#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "generating.h"
#include "AI1.h"
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
* Makes sure the AI can't cheat by deleting invisible data
* This function is pretty inefficient but will suffice until
* maybe a better solution is found.
*/
void makeDataInvisible(Tile field[10][10], int playerNumber, Tile playerField[10][10]) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (!(field[i][j].piece.owner == playerNumber || field[i][j].piece.visible)) {
				playerField[i][j].piece.name = '?';
				// -2 means that it is not visible what the value is
				playerField[i][j].piece.value = -2;
			}
			else {
				// is visible or is owned by the player, all data is visible
				playerField[i][j] = field[i][j];
			}
		}
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
	}
	else if (attacker.piece.value == defender.piece.value) {
		return 0;
	}
	else {
		return -1;
	}
}
/**
* Handles the move from the AI or player
* returns the player who won or 0, if the flag has not beed attacked
*/
int handleMove(Tile field[10][10], Move move)
{
	int newX = move.x;
	int newY = move.y;
	switch (move.cardinal) {
	case 'N': // Piece should be moved North
		newY = move.y - 1;
		break;
	case 'E': // Piece should be moved East
		newX = move.x + 1;
		break;
	case 'S': // Piece should be moved South
		newY = move.y + 1;
		break;
	case 'W': // Piece should be moved West
		newX = move.x - 1;
		break;
	default:
		printf("Error, not a valid cardinal\n");
		break;
	}
	Tile currectTile = field[move.y][move.x], targetTile = field[newY][newX];
	// check if the move is not out of bounds (out of array or water)
	if (newX < 0 || newX > 10 || newY < 0 || newY > 10 ||
		targetTile.land == 'W') {
		printf("Error, move is out of bounds");
		return currectTile.piece.owner == 1 ? 2 : 1;
	}
	// check if AI is now attacking it's own pieces.
	if (currectTile.piece.owner == targetTile.piece.owner) {
		printf("Error, AI%d used friendly fire!\nMoved from %i, %i owner:%i to %i, %i owner:%i piece:%c", currectTile.piece.owner, move.x, move.y, currectTile.piece.owner, newX, newY, targetTile.piece.owner, targetTile.piece.name);
		return currectTile.piece.owner == 1 ? 2 : 1;
	}
	switch (combatScore(currectTile, targetTile)) {
	case 2:  return currectTile.piece.owner; break;
	case 1:  field[newY][newX] = currectTile;
		field[move.y][move.x] = cleanGrassTile(); break;
	case 0:  field[newY][newX] = cleanGrassTile(); break;
	case -1: field[newY][newX].piece.visible = true;
		field[move.y][move.x] = cleanGrassTile(); break;
	default: printf("Not a valid combat score!\n"); break;
	}
	return 0;
}

/**
* plays game of two Ai's
*/
int playAiGame()
{
	// No second ai yet, identical boards as a result
	AI1 player1;
	AI1 player2;
	player1.playerNumber = 1;
	player2.playerNumber = 2;
	Tile field[10][10];
	createBoard(field);
	fillBoard(field, player1.startPos(), player2.startPos());
	bool isFinished = false;
	int turn = 1, turns_done = 0;
	Move move;
	Move previous_move = {-1, -1, 'N'};

	// Make custom private fields for AI's to prevent cheating
	Tile player1_field[10][10] = {};
	Tile player2_field[10][10] = {};

	while (!isFinished) {
		if (turn == 1) {
			makeDataInvisible(field, 1, player1_field);
			//printField(player1_field);
			move = player1.move(field, previous_move);
			turn++;
			turns_done++;
		}
		else {
			makeDataInvisible(field, 2, player2_field);
			//printField(player2_field);
			move = player2.move(field, previous_move);
			turn--;
			turns_done++;
		}
		int winningPlayer = handleMove(field, move);
		if (winningPlayer != 0) {
			return winningPlayer;
			isFinished = true;
		}
		if (turns_done > 1000)
		{
			printField(field);
			return turn;
			isFinished = true;
		}
		move = previous_move;
	}
}

int main() {
	playAiGame();
	getchar();
	getchar();
	return 0;
}
