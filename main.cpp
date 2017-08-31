#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <time.h>
#include "generating.h"
#include "AI1.h"
using namespace std;

/**
* Prints a board of Tiles
* Input: a 10 by 10 array of Tiles.
* Output: nothing.
*/
void printField(const Tile field[10][10]) {
	for (int T1 = 0; T1 < 10; T1++) {
		for (int T2 = 0; T2 < 10; T2++) {
			//the value of a empty tile is -1 so then the land type needs to be printed
			if (field[T1][T2].piece.value == -1) {
				printf(" %c", field[T1][T2].land);
			}
			//If the Tile is not empty the piece will be represented by its name. 
			//With F being the flag, B being the bom and T being the ten so the program does not need to print dubble digits.
			else {
				printf(" %c", field[T1][T2].piece.name);
			}
		}
		printf("\n");
	}
}

/**
* Hides the data the AI should not be able to see from its opponent
* Input: The 10 by 10 field of Tiles that the game keeps track on.
* Input: The player number of the player that is going to recieve the board.
* Output: The field that the player gets to see.
*/
void makeDataInvisible(const Tile field[10][10], const int playerNumber, Tile playerField[10][10]) {
	for (int T1 = 0; T1 < 10; T1++) {
		for (int T2 = 0; T2 < 10; T2++) {
			if (!(field[T1][T2].piece.owner == playerNumber || field[T1][T2].piece.visible)) {
				playerField[T1][T2].piece.name = '?';
				// -2 means that it is not visible what the value is
				playerField[T1][T2].piece.value = -2;
			}
			else {
				// is visible or is owned by the player, all data is visible
				playerField[T1][T2] = field[T1][T2];
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
	if (defender.piece.name == 'F') {
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
	if (move.x == 42){ return move.y == 1 ? 2 : 1; }// checks if it was the forfit move
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

int movecheck(Move move, Move movestore[]){
	int dubble = 0;
	for (int T1 = 8; T1 > -1; T1--)
	{
		movestore[T1 + 1] = movestore[T1];
	}
	movestore[0] = move;
	for (int T2 = 0; T2 < 10; T2++)
	{
		for (int T3 = 1; T3 < (10 - T2); T3++)
		{
			if (movestore[T2].x == movestore[T2 + T3].x && movestore[T2].y == movestore[T2 + T3].y && movestore[T2].cardinal == movestore[T2 + T3].cardinal && movestore[T2].x > -10)
			{
				dubble++;
			}
		}
	}
	if (dubble > 15){ return 1; }
	return 0;
}

/**
* plays game of two Ai's
*/
Game playAiGame()
{
	//timing
	clock_t AI11, AI12, AI21, AI22;
	float AI1tot = 0, AI2tot = 0, AI1avr = 0, AI2avr = 0;
	// No second ai yet, identical boards as a result
	AI1 player1;
	AI1 player2;
	player1.playerNumber = 1;
	player2.playerNumber = 2;
	Tile field[10][10];
	createBoard(field);
	fillBoard(field, player1.startPos(), player2.startPos());
	bool isFinished = false;
	int turn = 1, end = 0, turns_done =0;
	Move move, movestore1[10], movestore2[10];
	Move previous_move = {-1, -1, 'N'};

	// Make custom private fields for AI's to prevent cheating
	Tile player1_field[10][10] = {};
	Tile player2_field[10][10] = {};

	while (!isFinished) {
		if (turn == 1) {
			makeDataInvisible(field, 1, player1_field);
			//printField(player1_field);
			AI11 = clock();
			move = player1.move(field, previous_move);
			AI12 = clock();
			float diff ((float)AI12 - (float)AI11);
			AI1tot = AI1tot + (diff / CLOCKS_PER_SEC);
			if (movecheck(move, movestore1) == 1){ end = turn; }
			turn++;
			turns_done++;
		}
		else {
			makeDataInvisible(field, 2, player2_field);
			//printField(player2_field);
			AI21 = clock();
			move = player2.move(field, previous_move);
			AI22 = clock();
			float diff ((float)AI22 - (float)AI21);
			AI2tot = AI2tot + (diff / CLOCKS_PER_SEC);
			if (movecheck(move, movestore2) == 1){ end = turn; }
			turn--;
			turns_done++;
		}
		int winningPlayer = handleMove(field, move);
		if (winningPlayer != 0) {
			printf("AI%i: %i, %i, %c\n", winningPlayer, move.x, move.y, move.cardinal);
			printField(field);
			AI1avr = AI1tot / (turns_done / 2);
			AI2avr = AI2tot / (turns_done / 2);
			return{ winningPlayer, turns_done, AI1avr, AI2avr };
			isFinished = true;
		}
		if (turns_done > 1000000 || end > 0)
		{
			if (end > 0){ printf("AI%i is in a deadlock\n", end); }
			else{ printf("This game is taking to long\n"); }
			printField(field);
			AI1avr = AI1tot / (turns_done / 2);
			AI2avr = AI2tot / (turns_done / 2);
			return{ turn, turns_done, AI1avr, AI2avr };
			isFinished = true;
		}
		move = previous_move;
	}
}

int main() {
	int P1wins = 0, P2wins = 0, maxgames = 1001, turns_total = 0, turns_avarage;
	float Game_time = 0, Game_time_avarage = 0, AI1_total = 0, AI2_total = 0, AI1_avarage = 0, AI2_avarage = 0;
	clock_t P1, P2;
	for (int Games = 0; Games < maxgames; Games++)
	{
		P1 = clock();
		Game game = playAiGame();
		if (game.playerWon == 1){ P1wins++;}
		else{ P2wins++; }
		turns_total = turns_total + game.turns;
		P2 = clock();
		float diff ((float)P2 - (float)P1);
		Game_time = Game_time + (diff / CLOCKS_PER_SEC);
		AI1_total = AI1_total + game.AI1_time;
		AI2_total = AI2_total + game.AI2_time;
	}
	turns_avarage = turns_total / maxgames;
	Game_time_avarage = Game_time / maxgames;
	AI1_avarage = AI1_total / maxgames * 1000;
	AI2_avarage = AI2_total / maxgames * 1000;
	if (P1wins > P2wins){ printf("player 1 WON!\nWith %i points of %i.\n", P1wins, maxgames); }
	else{ printf("player 2 WON!\nWith %i points of %i.\n", P2wins, maxgames); }
	printf("Avarage turns per games: %i\n", turns_avarage);
	printf("Avarage gametime: %0.3f seconds\n", Game_time_avarage);
	printf("Avarage think time of AI1: %0.5f miliseconds\n", AI1_avarage);
	printf("Avarage think time of AI2: %0.5f miliseconds\n", AI2_avarage);
	getchar();
	return 0;
}
