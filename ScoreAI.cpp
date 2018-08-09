#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include "generating.h"
#include "ScoreAI.h"
#include "definitions.cpp"
using namespace std;

ScoreAI::ScoreAI(int p) : AI(p) {
}

// default constructor for Army
ArmyState::ArmyState() {
	int hid[12] = { 1, 1, 8, 5, 4, 4, 4, 3, 2, 1, 1, 6 };// amount of pieces that the games starts with for eatch type F,1,2,3,4,5,6,7,8,9,10,B
	for (int T1 = 0; T1 < 12; T1++) {
		Hidden[T1] = hid[T1];
		Revealed[T1] = 0;
		Dead[T1] = 0;
	}
	totalHidden = 40;
	totalRevealed = 0;
	totalDead = 0;
}

// default constructor for Fract_Piece
FractPiece::FractPiece() {
	
	frac[0] = 0;
	frac[1] = 0;
	frac[2] = 0;
	frac[3] = 0;
	frac[4] = 0;
	frac[5] = 0;
	frac[6] = 0;
	frac[7] = 0;
	frac[8] = 0;
	frac[9] = 0;
	frac[10] = 0;
	frac[11] = 0;
}

// default constructor for scores
Scores::Scores() {
	int HPi[12] = { 70 , 31, 13, 13, 15, 17, 20, 23, 27, 31, 40, 22 }; // points for a hidden piece F,1,2,3,4,5,6,7,8,9,10,B
	int RPi[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 , 0};// points for a revealed piece 
	int DPi[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0};// points for a dead piece
	for (int T1 = 0; T1 < 12; T1++) {
		HiddenPoints[T1] = HPi[T1];
		RevealedPoints[T1] = RPi[T1];
		DeadPoints[T1] = DPi[T1];
	}
}

//update the ArmyStates with the information of the previous turns
void ScoreAI::update_army(Tile field[10][10], Turn turn)
{
	int opponent = playerNumber == 1 ? 2 : 1;
	int you = playerNumber;

	// you killed piece of opponent
	if (turn.youKilled[0].owner == opponent) {
		ArmyStateOpponent.Dead[turn.youKilled[0].value]++;
		ArmyStateOpponent.totalDead++;
		if (turn.youKilled[0].visible) {
			ArmyStateOpponent.Revealed[turn.youKilled[0].value]--;
			ArmyStateOpponent.totalRevealed--;
		}
		else {
			ArmyStateOpponent.Hidden[turn.youKilled[0].value]--;
			ArmyStateOpponent.totalHidden--;
		}
	}

	// opponend killed your piece
	if (turn.opponentKilled[1].owner == opponent) {
		ArmyStateOpponent.Dead[turn.opponentKilled[1].value]++;
		ArmyStateOpponent.totalDead++;
		if (turn.opponentKilled[1].visible) {
			ArmyStateOpponent.Revealed[turn.opponentKilled[1].value]--;
			ArmyStateOpponent.totalRevealed--;
		}
		else {
			ArmyStateOpponent.Hidden[turn.opponentKilled[1].value]--;
			ArmyStateOpponent.totalHidden--;
		}
	}

	// you revealed opponents piece
	if (turn.youRevealed.owner == opponent) {
		ArmyStateOpponent.Revealed[turn.youRevealed.value]++;
		ArmyStateOpponent.totalRevealed++;
		ArmyStateOpponent.Hidden[turn.youRevealed.value]--;
		ArmyStateOpponent.totalHidden--;
	}

	// opponend revealed his piece
	if (turn.opponentRevealed.owner == opponent) {
		ArmyStateOpponent.Revealed[turn.opponentRevealed.value]++;
		ArmyStateOpponent.totalRevealed++;
		ArmyStateOpponent.Hidden[turn.opponentRevealed.value]--;
		ArmyStateOpponent.totalHidden--;

	}

	// you killed your piece 
	if (turn.youKilled[1].owner == you) {
		ArmyStateMy.Dead[turn.youKilled[1].value]++;
		ArmyStateMy.totalDead++;
		if (turn.youKilled[1].visible) {
			ArmyStateMy.Revealed[turn.youKilled[1].value]--;
			ArmyStateMy.totalRevealed--;
			
		}
		else {
			ArmyStateMy.Hidden[turn.youKilled[1].value]--;
			ArmyStateMy.totalHidden--;
		}
	}

	// opponend killed your piece
	if (turn.opponentKilled[0].owner == you) {
		ArmyStateMy.Dead[turn.opponentKilled[0].value]++;
		ArmyStateMy.totalDead++;
		if (turn.opponentKilled[0].visible) {
			ArmyStateMy.Revealed[turn.opponentKilled[0].value]--;
			ArmyStateMy.totalRevealed--;
		}
		else {
			ArmyStateMy.Hidden[turn.opponentKilled[0].value]--;
			ArmyStateMy.totalHidden--;
		}
	}

	// you revealed your piece
	if (turn.youRevealed.owner == you) {
		ArmyStateMy.Revealed[turn.youRevealed.value]++;
		ArmyStateMy.totalRevealed++;
		ArmyStateMy.Hidden[turn.youRevealed.value]--;
		ArmyStateMy.totalHidden--;
	}

	// opponend reveales your piece
	if (turn.opponentRevealed.owner == you) {
		ArmyStateMy.Revealed[turn.opponentRevealed.value]++;
		ArmyStateMy.totalRevealed++;
		ArmyStateMy.Hidden[turn.opponentRevealed.value]--;
		ArmyStateMy.totalHidden--;
	}
}

//give a score of a army in contrast to the opponents army
float ScoreAI::evaluate_armies(void)
{
	// waardes van de stukken
	//Stukken	b	1	2	3	4	5	6	7	8	9	10
	//Hidden	22	31	13	13	15	17	20	23	27	31  40	
	//Revealed	0	1	2	3	4	5	6	7	8	9	10
	//Dead		0	0	0	0	0	0	0	0	0	0	0
	float total = 0;
	for (int T1 = 0; T1 < 11; T1++) // go though all the pieces (not the flag)
	{
		total = total 
			+ ArmyStateMy.Hidden[T1]		 * Points.HiddenPoints[T1] // amount of pieces of a type that are hidden multipplied by the points for a single hidden piece
			+ ArmyStateMy.Revealed[T1]	     * Points.RevealedPoints[T1] 
			+ ArmyStateMy.Dead[T1]		     * Points.DeadPoints[T1] 
			- ArmyStateOpponent.Hidden[T1]   * Points.HiddenPoints[T1] 
			- ArmyStateOpponent.Revealed[T1] * Points.RevealedPoints[T1] 
			- ArmyStateOpponent.Dead[T1]     * Points.DeadPoints[T1];
	}

	return total;	
}


//gives the score change for the attacker
float ScoreAI::evaluate_trade(Piece attack, Piece defence, Scores score) {
	float P = 0;
	int combatState;
	// if defender is the flag
	if (defence.name == FLAG_NAME) {
		combatState = COMBAT_WON;
	}

		// 3 against bomb:
	else if (attack.value == 3 && defence.name == BOMB_NAME) {
		combatState = COMBAT_WON;
	}

		// 1 against 10:
	else if (attack.value == 1 && defence.value == 10) {
		combatState = COMBAT_WON;
	}

		// combat versus bomb
	else if (defence.name == BOMB_NAME) {
		combatState = COMBAT_LOST;
	}

		// normal combat
	else if (attack.value > defence.value) {
		combatState = COMBAT_WON;
	}
	else if (attack.value == defence.value) {
		combatState = COMBAT_DRAW;
	}
	else {
		combatState = COMBAT_LOST;
	}

	if (combatState == COMBAT_WON) {
		if (defence.visible == false) {
			P = P + score.HiddenPoints[defence.value];
		}
		else {
			P = P + score.RevealedPoints[defence.value];
		}
		if (attack.visible == false) {
			P = P - score.HiddenPoints[attack.value] + score.RevealedPoints[attack.value];
		}

	}
	else if (combatState == COMBAT_LOST) {
		if (attack.visible == false) {
			P = P - score.HiddenPoints[attack.value];
		}
		else {
			P = P - score.RevealedPoints[attack.value];
		}
		if (defence.visible == false) {
			P = P + score.HiddenPoints[attack.value] - score.RevealedPoints[attack.value];
		}
	}
	else {
		if (attack.visible == false) {
			P = P - score.HiddenPoints[attack.value];
		}
		else {
			P = P - score.RevealedPoints[attack.value];
		}
		if (defence.visible == false) {
			P = P + score.HiddenPoints[defence.value];
		}
		else {
			P = P + score.RevealedPoints[defence.value];
		}
	}

	return P;
}

/**
 * @param attacker
 * @param defender
 * @param score
 * @return (float) the sum of (chance of a certain piece * points gained/lost by attacking that piece)
 */
float ScoreAI::evaluateFractTrade(Piece attacker, FractPiece defender, Scores score) {
	float p = 0;

	for (int i=0; i < 12; i++) {
		Piece defendingPiece(i, playerNumber);
		p += defender.frac[i] * evaluate_trade(attacker, defendingPiece, score);
	}

	return p;
}

/**
 * Start_pos startPos() returns the starting position when called.
 * Place certain pieces on strategic places.
 *
 * Currently implements the following rules:
 * Flag on the backmost row, bombs around it.
 * Scouts on non water columns
 * The rest of the pieces are placed randomly
 */
Start_pos ScoreAI::startPos() {
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
					printf("Error, JurAI tried to place 2 while not having a 2 left\n");
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
					printf("Error, JurAI tried to place 2 while not having a 2 left\n");
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
					printf("Error, JurAI tried to place 2 while not having a 2 left\n");
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
					printf("Error, JurAI tried to place 2 while not having a 2 left\n");
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

void ScoreAI::make_FractField(Tile field[10][10]) {
	for (int T1 = 0; T1 < 10; T1++) {
		for (int T2 = 0; T2 < 10; T2++) {
			FractPiece piece;
			if (field[T1][T2].piece.visible = true) {
				piece.frac[field[T1][T2].piece.value] = 1;
			} else {
				for (int T3 = 0; T3 < 12; T3++) {
					piece.frac[T3] = ArmyStateOpponent.Hidden[T3] / ArmyStateOpponent.totalHidden;
				}
			}
		}
	}
}

//make a move
Move ScoreAI::move(Tile field[10][10], Turn turn) {
	update_army(field, turn);
	Scores score;
	Move r;
	return r;
}
