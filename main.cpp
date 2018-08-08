#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <time.h>
#include "generating.h"
#include "AI1.h"
#include "SanderAI.h"
#include "ScoreAI.h"
#include "JurAI.h"
#include "definitions.cpp"
#include "Board.h"


using namespace std;

/**
 * Prints a board of Tiles
 * Input: a 10 by 10 array of Tiles.
 * Output: nothing.
 */
void printField(const Tile field[10][10]) {
	for (int T1 = 0; T1 < 10; T1++) {
		for (int T2 = 0; T2 < 10; T2++) {
			if (field[T1][T2].piece.value == EMPTY_PIECE) {
				printf(" %c", field[T1][T2].land);
			} else {
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

/**
 * Evaluates combat
 */
int combatScore(Tile attacker, Tile defender) {

	// if defender is the flag
	if (defender.piece.name == 'F') {
		return COMBAT_WON;
	}

	// 3 against bomb:
	if (attacker.piece.value == 3 && defender.piece.name == 'B') {
		return COMBAT_WON;
	}

	// 1 against 10:
	if (attacker.piece.value == 1 && defender.piece.value == 10) {
		return COMBAT_WON;
	}

	// combat versus bomb
	if (defender.piece.name == 'B') {
		return COMBAT_LOST;
	}

	// normal combat
	if (attacker.piece.value > defender.piece.value) {
		return COMBAT_WON;
	} else if (attacker.piece.value == defender.piece.value) {
		return COMBAT_DRAW;
	} else {
		return COMBAT_LOST;
	}
}
/**
 * TODO: UPDATE THIS COMMENT
 * Handles the move from the AI or player
 * returns the player who won or 0, if the flag has not been attacked
 * If a unit is killed it returns the unit so the AI can see what it has killed
 */
Turn handleMove(Tile field[10][10], Turn players_turn) {
	Move move = players_turn.you_moved;
	int AI_turn = players_turn.count +1 %2 +1;
	if (move.no_moves) {
		players_turn.error = true;
		return players_turn;
	}// checks if it was the forfeit move

	int newX = move.x;
	int newY = move.y;
	switch (move.cardinal) {
	case NORTH:
		newY = move.y - players_turn.you_moved.tiles;
		break;
	case EAST:
		newX = move.x + players_turn.you_moved.tiles;
		break;
	case SOUTH:
		newY = move.y + players_turn.you_moved.tiles;
		break;
	case WEST:
		newX = move.x - players_turn.you_moved.tiles;
		break;
	default:
		printf("Error, not a valid cardinal\n");
		break;
	}
	Tile currectTile = field[move.y][move.x], targetTile = field[newY][newX];
	// check if the move is not out of bounds (out of array or water)
	if (newX < 0 || newX > 10 || newY < 0 || newY > 10 ||
		targetTile.land == 'W') {
		printf("Error, move is out of bounds\n");
		players_turn.error = true;
		return players_turn;
	}
	// check if AI is not attacking it's own pieces.
	if (currectTile.piece.owner == targetTile.piece.owner) {
		printf("Error, AI%d used friendly fire!\nMoved from %i, %i owner:%i to %i, %i owner:%i piece:%c\n", currectTile.piece.owner, move.x, move.y, currectTile.piece.owner, newX, newY, targetTile.piece.owner, targetTile.piece.name);
		players_turn.error = true;
		return players_turn;
	}
	//check if the AI does not move an empty piece.
	if (currectTile.piece.name == 'E'){
		printf("no piece to move\n"); 
		players_turn.error = true;
		return players_turn;
	}

	switch (combatScore(currectTile, targetTile)) {

	case 1:  players_turn.you_killed[0] = field[newY][newX].piece;
		players_turn.you_killed[1] = Piece();
		if (field[newY][newX].piece.name != 'E' && currectTile.piece.visible != true){
			players_turn.you_revealed = currectTile.piece;
			currectTile.piece.visible = true;
		}
		else {
			players_turn.you_revealed = Piece();
		}
		field[newY][newX] = currectTile;
		field[move.y][move.x] = cleanGrassTile(move.x, move.y);  break;

	case 0:  players_turn.you_killed[0] = field[newY][newX].piece;
		players_turn.you_killed[1] = field[move.y][move.x].piece;
		players_turn.you_revealed = Piece();
		field[move.y][move.x] = cleanGrassTile(move.x, move.y);
		field[newY][newX] = cleanGrassTile(move.x, move.y); break;

	case -1: players_turn.you_killed[0] = Piece();
		players_turn.you_killed[1] = field[move.y][move.x].piece;
		if (field[newY][newX].piece.visible != true){
			players_turn.you_revealed = field[newY][newX].piece;
			field[newY][newX].piece.visible = true;
		}
		else {
			players_turn.you_revealed = Piece();
		}
		field[move.y][move.x] = cleanGrassTile(move.x, move.y); break;

	default: printf("Not a valid combat score!\n"); break;
	}
	return players_turn;
}

/**
 * corrects the cardinal and the x and x of the move given by player 2 becaus it thinks it is player 1
 */
Move turnaround_Move(Move move)
{
	switch(move.cardinal){
	case NORTH: move.cardinal = SOUTH; break;
	case SOUTH: move.cardinal = NORTH; break;
	case WEST: move.cardinal = EAST; break;
	case EAST: move.cardinal = WEST; break;
	}
	move.x = 9 - move.x;
	move.y = 9 - move.y;
	return move;
}

/**
 * Prints the options the player can choose from
 */
void printOptions(int pNum) {
	printf("Which AI is going to be player %d?\n", pNum);
	printf("1: AI1\n");
	printf("2: SanderAI\n");
	printf("3: ScoreAI\n");
	printf("4: JurAI\n");
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
Game playAiGame() {
	//timing
	srand(time(0));
	clock_t AI11, AI12, AI21, AI22;
	float AI1tot = 0, AI2tot = 0, AI1avr = 0, AI2avr = 0;
	
	// Get AI's to play the games with
	printOptions(1);
	int AiId = getAiId();
	AI *player1;
	switch (AiId) {
	case 2: player1 = new SanderAI(1); break;
	case 3: player1 = new ScoreAI(1); break;
	case 4: player1 = new JurAI(1); break;
	default: player1 = new AI1(1); break;
	}
	AI *player2;
	printOptions(2);
	AiId = getAiId();
	switch (AiId) {
	case 2: player2 = new SanderAI(2); break;
	case 3: player2 = new ScoreAI(2); break;
	case 4: player2 = new JurAI(2); break;
	default: player2 = new AI1(2); break;
	}


	// Create the board and fill with starting positions
	Board board(player1->startPos(), player2->startPos());
	
	bool isFinished = false;
	int AIturn = 1, end = 0, turns_done =0;
	Move move, previous_move;
	printf("start board\n");
	board.print(board.field);
	printf("\n");

	// Make custom private fields for AI's to prevent cheating
	Tile player1_field[10][10] = {};
	Tile player2_field[10][10] = {};
	Turn AI1_turn, AI2_turn;
	while (!isFinished) {
		if (AIturn == 1) { // player1's turn
			AI1_turn.count++;
			AI1_turn = Turn(AI1_turn, AI2_turn);
			board.updatePlayerBoard(1);
			board.print(board.player1Field);

			AI11 = clock();
			AI1_turn.you_moved = player1->move(board.player1Field, previous_move, AI1_turn);
			AI12 = clock();

			float diff ((float)AI12 - (float)AI11);
			AI1tot = AI1tot + (diff / CLOCKS_PER_SEC);

			printf("AI1: %i, %i, %c %i\n", AI1_turn.you_moved.x, AI1_turn.you_moved.y, AI1_turn.you_moved.cardinal, AI1_turn.you_moved.tiles);//print the move of the AI
			AI1_turn = handleMove(board.field, AI1_turn);
			board.print(board.field);
			printf("\n");

			AIturn++;
			turns_done = AI1_turn.count;
		} else { // player2's turn
			AI2_turn = Turn(AI2_turn, AI1_turn);
			board.updatePlayerBoard(2);
			board.print(board.player2Field); // print what the AI sees

			AI21 = clock();
			AI2_turn.you_moved = player2->move(board.player2Field, previous_move, AI2_turn);
			AI22 = clock();

			float diff ((float)AI22 - (float)AI21);
			AI2tot = AI2tot + (diff / CLOCKS_PER_SEC);

			AI2_turn.you_moved = turnaround_Move(AI2_turn.you_moved);
			printf("AI2: %i, %i, %c %i\n", AI2_turn.you_moved.x, AI2_turn.you_moved.y, AI2_turn.you_moved.cardinal, AI2_turn.you_moved.tiles);//print the move of the AI
			AI2_turn = handleMove(board.field, AI2_turn);
			board.print(board.field);//print the output of the AI
			printf("\n");

			AIturn--;
			turns_done = AI2_turn.count;
		}

		if (AI1_turn.you_killed[0].name == 'F' || AI2_turn.you_killed[0].name == 'F' || AI1_turn.error || AI2_turn.error) {
			AI1avr = AI1tot / (turns_done / 2);
			AI2avr = AI2tot / (turns_done / 2);
			return{ AIturn == 1 ? 2 : 1 , turns_done, AI1avr, AI2avr };
			isFinished = true;
		}
		if (turns_done > 1000000 ) {
			printf("This game is taking to long\n"); 
			board.print(board.field);
			AI1avr = AI1tot / (turns_done / 2);
			AI2avr = AI2tot / (turns_done / 2);
			return{ AIturn, turns_done, AI1avr, AI2avr };
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
