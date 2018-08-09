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
		if (turn.youKilled[0].visible) {
			ArmyStateOpponent.Revealed[turn.youKilled[0].value]--;
		}
		else {
			ArmyStateOpponent.Hidden[turn.youKilled[0].value]--;
		}
	}
	// opponend killed your piece
	if (turn.opponentKilled[1].owner == opponent) {
		ArmyStateOpponent.Dead[turn.opponentKilled[1].value]++;
		if (turn.opponentKilled[1].visible) {
			ArmyStateOpponent.Revealed[turn.opponentKilled[1].value]--;
		}
		else {
			ArmyStateOpponent.Hidden[turn.opponentKilled[1].value]--;
		}
	}
	// you revealed opponents piece
	if (turn.youRevealed.owner == opponent) {
		ArmyStateOpponent.Revealed[turn.youRevealed.value]++;
		ArmyStateOpponent.Hidden[turn.youRevealed.value]--;

	}
	// opponend revealed his piece
	if (turn.opponentRevealed.owner == opponent) {
		ArmyStateOpponent.Revealed[turn.opponentRevealed.value]++;
		ArmyStateOpponent.Hidden[turn.opponentRevealed.value]--;

	}

	// you killed your piece 
	if (turn.youKilled[1].owner == you) {
		ArmyStateMy.Dead[turn.youKilled[1].value]++;
		if (turn.youKilled[1].visible) {
			ArmyStateMy.Revealed[turn.youKilled[1].value]--;
		}
		else {
			ArmyStateMy.Hidden[turn.youKilled[1].value]--;
		}
	}
	// opponend killed your piece
	if (turn.opponentKilled[0].owner == you) {
		ArmyStateMy.Dead[turn.opponentKilled[0].value]++;
		if (turn.opponentKilled[0].visible) {
			ArmyStateMy.Revealed[turn.opponentKilled[0].value]--;
		}
		else {
			ArmyStateMy.Hidden[turn.opponentKilled[0].value]--;
		}
	}
	// you revealed your piece
	if (turn.youRevealed.owner == you) {
		ArmyStateMy.Revealed[turn.youRevealed.value]++;
		ArmyStateMy.Hidden[turn.youRevealed.value]--;

	}
	// opponend reveales your piece
	if (turn.opponentRevealed.owner == you) {
		ArmyStateMy.Revealed[turn.opponentRevealed.value]++;
		ArmyStateMy.Hidden[turn.opponentRevealed.value]--;

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

// returs float with the expected score difference of the trade
float ScoreAI::evaluate_FractTrade(Piece M, FractPiece T, Scores score) {
	float P = 0;
	return P;
}

//generates starting position
Start_pos ScoreAI::startPos() {

	Start_pos r;
	return r;
}

//make a move
Move ScoreAI::move(Tile field[10][10], Turn turn) {
	update_army(field, turn);
	Scores score;
	Move r;
	return r;
}
