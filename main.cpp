#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <time.h>
#include "generating.h"
#include "AI1.h"
#include "SanderAI.h"
#include "JurAI.h"

using namespace std;

const int MAXGAMES = 3;
const int NUM_OF_AI = 3;


/*
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

/*
 * Hides the data the AI should not be able to see from its opponent
 * Input: The 10 by 10 field of Tiles that the game keeps track on.
 * Input: The player number of the player that is going to recieve the board.
 * Output: The field that the player gets to see.
 * it now also turn the field arround for player2 so the Ai always plays on top
 */
void makeDataInvisible(const Tile field[10][10], const int playerNumber, Tile playerField[10][10]) {
	for (int T1 = 0; T1 < 10; T1++) {
		for (int T2 = 0; T2 < 10; T2++) {
			playerField[T1][T2] = field[T1][T2];
			if (field[T1][T2].piece.owner != playerNumber && !field[T1][T2].piece.visible && field[T1][T2].piece.owner != 0) {
				playerField[T1][T2].piece.name = '?';
				// -2 means that it is not visible what the value is
				playerField[T1][T2].piece.value = -2;
			}
		}
		}
	if (playerNumber == 2){
		Tile temp;
		for (int T3 = 0; T3 < 5; T3++) {
			for (int T4 = 0; T4 < 10; T4++) {
				temp = playerField[T3][T4];
				playerField[T3][T4] = playerField[9 - T3][9 - T4];
				playerField[9 - T3][9 - T4] = temp;
				}
			}
		}
	}

/*
 * Evaluates combat
 * Returns -1 if lost, 0 if draw, 1 if win and 2 if flag is hit.
 */
int combatScore(Tile attacker, Tile defender) {
	printf("%i against %i\n", attacker.piece.value, defender.piece.value);
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
		// win
		return 1;
	} else if (attacker.piece.value == defender.piece.value) {
		// tie
		return 0;
	} else {
		// loss
		return -1;
	}
}
/*
 * Handles the move from the AI or player
 * returns the player who won or 0, if the flag has not beed attacked
 */
int handleMove(Tile field[10][10], Move move, int turn) {
	if (move.no_moves == true){ return move.y == 1 ? 2 : 1; }// checks if it was the forfit move
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
		return turn;
	}
	// check if AI is not attacking it's own pieces.
	if (currectTile.piece.owner == targetTile.piece.owner) {
		printf("Error, AI%d used friendly fire!\nMoved from %i, %i owner:%i to %i, %i owner:%i piece:%c", currectTile.piece.owner, move.x, move.y, currectTile.piece.owner, newX, newY, targetTile.piece.owner, targetTile.piece.name);
		return turn;
	}
	if (currectTile.piece.name == 'E'){ printf("no piece to move\n"); return turn; }

	switch (combatScore(currectTile, targetTile)) {
	case 2:  return turn; break;
	case 1:  field[newY][newX] = currectTile;
		field[move.y][move.x] = cleanGrassTile(); break;
	case 0:  field[newY][newX] = cleanGrassTile(); break;
	case -1: field[newY][newX].piece.visible = true;
		field[move.y][move.x] = cleanGrassTile(); break;
	default: printf("Not a valid combat score!\n"); break;
	}
	return 0;
}

/*
 * corrects the cardinal and the x and x of the move given by player 2 becaus it thinks it is player 1
*/
Move turnaround_Move(Move move)
{
	switch(move.cardinal){
	case 'N': move.cardinal = 'S'; break;
	case 'S': move.cardinal = 'N'; break;
	case 'W': move.cardinal = 'E'; break;
	case 'E': move.cardinal = 'W'; break;
	}
	move.x = 9 - move.x;
	move.y = 9 - move.y;
	return move;
}

/*
 * movecheck checks if an AI is not doing the same move
 * in an infinite loop
 */
int moveCheck(Move move, Move movestore[]) {
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
	if (dubble > 25){ return 1; }
	return 0;
}

// Prints the options the player can choose from
void printOptions(int pNum) {
	printf("Which AI is going to be player %d?\n", pNum);
	printf("1: AI1\n");
	printf("2: SanderAI\n");
	printf("3: JurAI\n");
}

// Handles the input of the user choosing
// which AI they want to use.
int getAiId() {
	int AiId = 0;
	try {
		scanf_s("%d", &AiId);
		if (AiId < 1 || AiId > NUM_OF_AI) {
			throw;
		}
	}
	catch (...) {
		printf("WrongInputException");
	}
	return AiId;
}

/*
 * plays game of two Ai's
 */
Game playAiGame() {
	//timing
	clock_t AI11, AI12, AI21, AI22;
	float AI1tot = 0, AI2tot = 0, AI1avr = 0, AI2avr = 0;
	
	// Get AI's to play the games with
	printOptions(1);
	int AiId = getAiId();
	AI *player1;
	switch (AiId) {
	case 2: player1 = new SanderAI(1); break;
	case 3: player1 = new JurAI(1); break;
	default: player1 = new AI1(1); break;
	}
	AI *player2;
	printOptions(2);
	AiId = getAiId();
	switch (AiId) {
	case 2: player2 = new SanderAI(2); break;
	case 3: player2 = new JurAI(2); break;
	default: player2 = new AI1(1); break;
	}


	// Create the board and fill with starting positions
	Tile field[10][10];
	createBoard(field);
	fillBoard(field, player1->startPos(), player2->startPos());
	
	bool isFinished = false;
	int turn = 1, end = 0, turns_done =0;
	Move move, movestore1[10], movestore2[10];
	Move previous_move = {-1, -1, 'N'};
	printf("start board\n");
	printField(field);
	printf("\n");

	// Make custom private fields for AI's to prevent cheating
	Tile player1_field[10][10] = {};
	Tile player2_field[10][10] = {};
	int winningPlayer;
	while (!isFinished) {
		if (turn == 1) { // player1's turn
			makeDataInvisible(field, 1, player1_field);
			printField(player1_field);// print the input of the AI

			AI11 = clock();
			move = player1->move(player1_field, previous_move);
			AI12 = clock();

			float diff ((float)AI12 - (float)AI11);
			AI1tot = AI1tot + (diff / CLOCKS_PER_SEC);
			if (moveCheck(move, movestore1) == 1){ end = turn; }

			printf("AI1: %i, %i, %c\n", move.x, move.y, move.cardinal);//print the move of the AI
			winningPlayer = handleMove(field, move, turn);
			printField(field);//print the output of the AI
			printf("\n");

			turn++;
			turns_done++;
		} else { // player2's turn
			makeDataInvisible(field, 2, player2_field);
			printField(player2_field);// print the input of the AI

			AI21 = clock();
			move = player2->move(player2_field, previous_move);
			AI22 = clock();

			float diff ((float)AI22 - (float)AI21);
			AI2tot = AI2tot + (diff / CLOCKS_PER_SEC);
			if (moveCheck(move, movestore2) == 1){ end = turn; }

			move = turnaround_Move(move);
			printf("AI2: %i, %i, %c\n", move.x, move.y, move.cardinal);//print the move of the AI
			winningPlayer = handleMove(field, move, turn);
			printField(field);//print the output of the AI
			printf("\n");

			turn--;
			turns_done++;
		}

		if (winningPlayer != 0) {
			printf("AI%i: %i, %i, %c\n", winningPlayer, move.x, move.y, move.cardinal);
			printField(field);
			AI1avr = AI1tot / (turns_done / 2);
			AI2avr = AI2tot / (turns_done / 2);
			return{ winningPlayer, turns_done, AI1avr, AI2avr };
			isFinished = true;
		}
		if (turns_done > 1000000 || end > 0) {
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

void playGames() {
	int P1wins = 0, P2wins = 0, totalTurns = 0, averageTurns = 0;
	float gameTime = 0, gameTimeAverage = 0, AI1Total = 0, AI2Total = 0, AI1Average = 0, AI2Average = 0;
	clock_t P1, P2;

	// Play the games
	for (int games = 0; games < MAXGAMES; games++) {
		P1 = clock();
		Game game = playAiGame();
		game.playerWon == 1 ? P1wins++ : P2wins++;
		totalTurns += game.turns;
		P2 = clock();
		float diff((float)P2 - (float)P1);
		gameTime += diff / CLOCKS_PER_SEC;
		AI1Total += game.AI1_time;
		AI2Total += game.AI2_time;
	}
	averageTurns = totalTurns / MAXGAMES;
	gameTimeAverage = gameTime / MAXGAMES;
	AI1Average = AI1Total / MAXGAMES * 1000;
	AI2Average = AI2Total / MAXGAMES * 1000;
	if (P1wins > P2wins) {
		printf("player 1 WON!\nWith %i points of %i.\n", P1wins, MAXGAMES);
	}
	else {
		printf("player 2 WON!\nWith %i points of %i.\n", P2wins, MAXGAMES);
	}
	printf("Avarage turns per games: %i\n", averageTurns);
	printf("Avarage gametime: %0.3f seconds\n", gameTimeAverage);
	printf("Avarage think time of AI1: %0.5f miliseconds\n", AI1Average);
	printf("Avarage think time of AI2: %0.5f miliseconds\n", AI2Average);
}

int main() {
	playGames();
	getchar();
	getchar();
	return 0;
}
