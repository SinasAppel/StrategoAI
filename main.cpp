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

#define MAXGAMES 64800

using namespace std;


/**
 * Evaluates combat
 */
int combatScore(Tile attacker, Tile defender) {

	// if defender is the flag
	if (defender.piece.name == FLAG_NAME) {
		return COMBAT_WON;
	}

	// 3 against bomb:
	if (attacker.piece.value == 3 && defender.piece.name == BOMB_NAME) {
		return COMBAT_WON;
	}

	// 1 against 10:
	if (attacker.piece.value == 1 && defender.piece.value == 10) {
		return COMBAT_WON;
	}

	// combat versus bomb
	if (defender.piece.name == BOMB_NAME) {
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
 * Handles the move from the AI or player
 * Edits the board according to the move and
 * updates the turn structs of the AI that is doing the current turn
 */
Turn handleMove(Tile field[10][10], Turn playersTurn) {
	Move move = playersTurn.youMoved;
	if (move.noMoves) {
		playersTurn.error = true;
		return playersTurn;
	}// checks if it was the forfeit move

	int newX = move.x;
	int newY = move.y;

	switch (move.cardinal) {
		case NORTH:
			newY = move.y - playersTurn.youMoved.tiles;
			break;
		case EAST:
			newX = move.x + playersTurn.youMoved.tiles;
			break;
		case SOUTH:
			newY = move.y + playersTurn.youMoved.tiles;
			break;
		case WEST:
			newX = move.x - playersTurn.youMoved.tiles;
			break;
		default:
			throw("Error, not a valid cardinal\n");
	}

	Tile currentTile = field[move.y][move.x], targetTile = field[newY][newX];

	// check if the move is not out of bounds (out of array or water)
	if (newX < 0 || newX > 10 || newY < 0 || newY > 10 ||
		targetTile.land == TILE_WATER) {
		printf("Error, move is out of bounds\n");
		playersTurn.error = true;
		return playersTurn;
	}
	// check if AI is not attacking it's own pieces.
	if (currentTile.piece.owner == targetTile.piece.owner) {
		printf("Error, AI%d used friendly fire!\nMoved from %i, %i owner:%i to %i, %i owner:%i piece:%c\n", currentTile.piece.owner, move.x, move.y, currentTile.piece.owner, newX, newY, targetTile.piece.owner, targetTile.piece.name);
		playersTurn.error = true;
		return playersTurn;
	}
	// check if the AI does not move an empty piece.
	if (currentTile.piece.name == EMPTY_PIECE_NAME) {
		printf("no piece to move\n");
		playersTurn.error = true;
		return playersTurn;
	}

	switch (combatScore(currentTile, targetTile)) {
		case COMBAT_WON:
			playersTurn.youKilled[0] = field[newY][newX].piece;
			playersTurn.youKilled[1] = Piece();
			if (field[newY][newX].piece.name != EMPTY_PIECE_NAME && !currentTile.piece.visible) {
				playersTurn.youRevealed = currentTile.piece;
				currentTile.piece.visible = true;
			} else {
				playersTurn.youRevealed = Piece();
			}
			field[newY][newX].piece = currentTile.piece;
			field[move.y][move.x] = cleanGrassTile(move.x, move.y);
			break;
		case COMBAT_DRAW:
			playersTurn.youKilled[0] = field[newY][newX].piece;
			playersTurn.youKilled[1] = field[move.y][move.x].piece;
			playersTurn.youRevealed = Piece();
			field[move.y][move.x] = cleanGrassTile(move.x, move.y);
			field[newY][newX] = cleanGrassTile(newX, newY);
			break;
		case COMBAT_LOST:
			playersTurn.youKilled[0] = Piece();
			playersTurn.youKilled[1] = field[move.y][move.x].piece;
			if (!field[newY][newX].piece.visible) {
				playersTurn.youRevealed = field[newY][newX].piece;
				field[newY][newX].piece.visible = true;
			} else {
				playersTurn.youRevealed = Piece();
			}
			field[move.y][move.x] = cleanGrassTile(move.x, move.y);
			break;
		default:
			throw("Not a valid combat score!\n");
	}
	return playersTurn;
}

/**
 * Corrects the cardinal, the y and the x of the move given by
 * player 2 because it thinks it is player 1
 */
Move turnaround_Move(Move move)
{
	switch (move.cardinal) {
		case NORTH:
			move.cardinal = SOUTH;
			break;
		case SOUTH:
			move.cardinal = NORTH;
			break;
		case WEST:
			move.cardinal = EAST;
			break;
		case EAST:
			move.cardinal = WEST;
			break;
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
 * Plays game of two Ai's
 */
Game playAiGame(AI *player1, AI *player2) {
	// Timing
	srand(time(0));
	clock_t AI11, AI12, AI21, AI22;
	float AI1tot = 0, AI2tot = 0, AI1avr = 0, AI2avr = 0;

	// Create the board and fill with starting positions
	Board board(player1->startPos(), player2->startPos());
	
	bool isFinished = false;
	int AIsTurn = 1, end = 0, turnsDone = 0;
	Move move, previous_move;
	printf("start board\n");
	board.print(board.field, move);
	printf("\n");

	// Make custom private fields for AI's to prevent cheating
	Tile player1_field[10][10] = {};
	Tile player2_field[10][10] = {};
	Turn AI1_turn, AI2_turn;
	while (turnsDone < 1000000) {
		if (AIsTurn == 1) { // player1's turn
			AI1_turn.count++;
			AI1_turn = Turn(AI1_turn, AI2_turn);
			board.updatePlayerBoard(1);
			board.print(board.player1Field, AI2_turn.youMoved);

			AI11 = clock();
			AI1_turn.youMoved = player1->move(board.player1Field, AI1_turn);
			AI12 = clock();

			float diff ((float)AI12 - (float)AI11);
			AI1tot = AI1tot + (diff / CLOCKS_PER_SEC);

			printf("AI1: %i, %i, %c %i\n", AI1_turn.youMoved.x, AI1_turn.youMoved.y, AI1_turn.youMoved.cardinal, AI1_turn.youMoved.tiles);//print the move of the AI
			AI1_turn = handleMove(board.field, AI1_turn);
			board.print(board.field, AI1_turn.youMoved);
			printf("\n");

			AIsTurn++;
			turnsDone = AI1_turn.count;
		} else { // player2's turn
			AI2_turn = Turn(AI2_turn, AI1_turn);
			board.updatePlayerBoard(2);
			board.print(board.player2Field, AI1_turn.youMoved); // print what the AI sees

			AI21 = clock();
			AI2_turn.youMoved = player2->move(board.player2Field, AI2_turn);
			AI22 = clock();

			float diff ((float)AI22 - (float)AI21);
			AI2tot = AI2tot + (diff / CLOCKS_PER_SEC);

			AI2_turn.youMoved = turnaround_Move(AI2_turn.youMoved);
			printf("AI2: %i, %i, %c %i\n", AI2_turn.youMoved.x, AI2_turn.youMoved.y, AI2_turn.youMoved.cardinal, AI2_turn.youMoved.tiles);//print the move of the AI
			AI2_turn = handleMove(board.field, AI2_turn);
			board.print(board.field, AI2_turn.youMoved);//print the output of the AI
			printf("\n");

			AIsTurn--;
			turnsDone = AI2_turn.count;
		}

		if (AI1_turn.youKilled[0].name == FLAG_NAME || AI2_turn.youKilled[0].name == FLAG_NAME) {
			AI1avr = AI1tot / (turnsDone / 2);
			AI2avr = AI2tot / (turnsDone / 2);
			board.print(board.field, move);
			return{ AIsTurn == 1 ? 2 : 1 , turnsDone, AI1avr, AI2avr };
		}
		if (AI1_turn.error || AI2_turn.error) {
			AI1avr = AI1tot / (turnsDone / 2);
			AI2avr = AI2tot / (turnsDone / 2);
			board.print(board.field, move);
			return{ AIsTurn, turnsDone, AI1avr, AI2avr };
		}

		move = previous_move;
	}

	printf("This game is taking to long\n");
	board.print(board.field, move);
	AI1avr = AI1tot / (turnsDone / 2);
	AI2avr = AI2tot / (turnsDone / 2);

	return{ AIsTurn, turnsDone, AI1avr, AI2avr };
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

	// Play the games
	for (int games = 0; games < MAXGAMES; games++) {
		P1 = clock();
		Game game = playAiGame(player1, player2);
		game.playerWon == 1 ? P1wins++ : P2wins++;
		totalTurns += game.turns;
		P2 = clock();
		float diff((float)P2 - (float)P1);
		gameTime += diff / CLOCKS_PER_SEC;
		AI1Total += game.AI1Time;
		AI2Total += game.AI2Time;
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
