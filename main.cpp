#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <time.h>
#include "definitions.cpp"
#include "generating.h"
#include "AI1.h"
#include "SanderAI.h"
#include "JurAI.h"
#include "field.h"

using namespace std;

/**
 * Evaluates combat
 */
int combatScore(Tile attacker, Tile defender) {
	// if defender is the Flag
	if (defender.piece.name == 'F') {
		return COMBAT_FLAG_HIT;
	}
	// 3 against bomb:
	if (attacker.piece.value == 3 && defender.piece.value == 12) {
		return COMBAT_WIN;
	}
	// 1 against 10:
	if (attacker.piece.value == 1 && defender.piece.value == 10) {
		return COMBAT_WIN;
	}
	// normal combat
	if (attacker.piece.value > defender.piece.value) {
		// win
		return COMBAT_WIN;
	} else if (attacker.piece.value == defender.piece.value) {
		// tie
		return COMBAT_DRAW;
	} else {
		// loss
		return COMBAT_LOST;
	}
}

/**
 * Handles the move from the AI or player
 * returns the player who won or 0, if the flag has not beed attacked
 */
int handleMove(Tile field[10][10], Move move) {
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
        throw "Error, not a valid cardinal\n";
	}
	Tile currectTile = field[move.y][move.x], targetTile = field[newY][newX];
	// check if the move is not out of bounds (out of array or water)
	if (newX < 0 || newX > 10 || newY < 0 || newY > 10 ||
		targetTile.land == 'W') {
	    throw "Error, move is out of bounds";
	}
	// check if AI is not attacking it's own pieces.
	if (currectTile.piece.owner == targetTile.piece.owner) {
        //TODO: Make a special exception for this error message
		printf("Error, AI%d used friendly fire!\nMoved from %i, %i owner:%i to %i, %i owner:%i piece:%c", currectTile.piece.owner, move.x, move.y, currectTile.piece.owner, newX, newY, targetTile.piece.owner, targetTile.piece.name);
		return currectTile.piece.owner == 1 ? 2 : 1;
	}

	switch (combatScore(currectTile, targetTile)) {
	    case COMBAT_FLAG_HIT:
	        return currectTile.piece.owner;
	    case COMBAT_WIN:
	        field[newY][newX] = currectTile;
	        field[move.y][move.x] = cleanGrassTile();
	        break;
	    case COMBAT_DRAW:
	        field[newY][newX] = cleanGrassTile();
	        break;
	    case COMBAT_LOST:
	        field[newY][newX].piece.visible = true;
	        field[move.y][move.x] = cleanGrassTile();
	        break;
	    default:
            throw "Not a valid combat score";
	}
	return 0;
}

/**
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

/**
 * Prints the options the player can choose from
 */
void printOptions(int pNum) {
	printf("Which AI is going to be player %d?\n", pNum);
	printf("1: AI1\n");
	printf("2: SanderAI\n");
	printf("3: JurAI\n");
}

/**
 * Handles the input of the user choosing
 * which AI they want to use.
 */
int getAiId() {
	int AiId = 0;
	try {
		scanf("%d", &AiId);
		if (AiId < 1 || AiId > NUM_OF_AI) {
			throw;
		}
	}
	catch (...) {
		printf("WrongInputException");
	}
	return AiId;
}

/**
 * plays game of two Ai's
 */
Game playAiGame(AI *player1, AI *player2) {
	//timing
	clock_t AI11, AI12, AI21, AI22;
	float AI1tot = 0, AI2tot = 0, AI1avr = 0, AI2avr = 0;

	// Create the board and fill with starting positions
	Field field(player1->startPos(), player2->startPos());
	
	bool isFinished = false;
	int turn = 1, end = 0, turns_done =0;
	Move move, movestore1[10], movestore2[10];
	Move previous_move = {-1, -1, 'N'};
	field.print();

	// Make custom private fields for AI's to prevent cheating
	PlayerField playerField1(1, field);
	PlayerField playerField2(2, field);

	while (!isFinished) {
		if (turn == 1) { // player1's turn
			playerField1.updateField(field);
			AI11 = clock();
			move = player1->move(playerField1.playerField, previous_move);
			AI12 = clock();
			float diff ((float)AI12 - (float)AI11);
			AI1tot = AI1tot + (diff / CLOCKS_PER_SEC);
			if (moveCheck(move, movestore1) == 1){ end = turn; }
			turn++;
			turns_done++;
		} else { // player2's turn
			playerField2.updateField(field);
			AI21 = clock();
			move = player2->move(playerField2.playerField, previous_move);
			AI22 = clock();
			float diff ((float)AI22 - (float)AI21);
			AI2tot = AI2tot + (diff / CLOCKS_PER_SEC);
			if (moveCheck(move, movestore2) == 1){ end = turn; }
			turn--;
			turns_done++;
		}

		int winningPlayer = 0;
		try {
            int winningPlayer = handleMove(field.mainField, move);
		} catch (char* message) {
		    printf(message);
		}

		if (winningPlayer != 0) {
			printf("AI%i: %i, %i, %c\n", winningPlayer, move.x, move.y, move.cardinal);
			field.print();
			AI1avr = AI1tot / (turns_done / 2);
			AI2avr = AI2tot / (turns_done / 2);
			return{ winningPlayer, turns_done, AI1avr, AI2avr };
			isFinished = true;
		}
		if (turns_done > 1000000 || end > 0) {
			if (end > 0) {
			    printf("AI%i is in a deadlock\n", end);
			} else {
			    printf("This game is taking to long\n");
			}

			field.print();
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

	// Play the games
	for (int games = 0; games < MAXGAMES; games++) {
		P1 = clock();
		Game game = playAiGame(player1, player2);
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
	getchar();
	return 0;
}
