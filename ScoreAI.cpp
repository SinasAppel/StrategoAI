#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include "generating.h"
#include "ScoreAI.h"
using namespace std;

ScoreAI::ScoreAI(int p) : AI(p) {
}

// default constructor for Army
ArmyState::ArmyState() {
	int hid[11] = { 6, 1, 8, 5, 4, 4, 4, 3, 2, 1, 1 };// amount of pieces that the games starts with for eatch type
	for (int T1 = 0; T1 < 11; T1++) {
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
	int HPi[12] = { 22, 31, 13, 13, 15, 17, 20, 23, 27, 31, 40 , 70}; // points for a hidden piece
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
	if (turn.you_killed[0].owner == opponent) {
		ArmyStateOpponent.Dead[turn.you_killed[0].value]++;
		if (turn.you_killed[0].visible) {
			ArmyStateOpponent.Revealed[turn.you_killed[0].value]--;
		}
		else {
			ArmyStateOpponent.Hidden[turn.you_killed[0].value]--;
		}
	}
	// opponend killed your piece
	if (turn.opponent_killed[1].owner == opponent) {
		ArmyStateOpponent.Dead[turn.opponent_killed[1].value]++;
		if (turn.opponent_killed[1].visible) {
			ArmyStateOpponent.Revealed[turn.opponent_killed[1].value]--;
		}
		else {
			ArmyStateOpponent.Hidden[turn.opponent_killed[1].value]--;
		}
	}
	// you revealed opponents piece
	if (turn.you_revealed.owner == opponent) {
		ArmyStateOpponent.Revealed[turn.you_revealed.value]++;
		ArmyStateOpponent.Hidden[turn.you_revealed.value]--;

	}
	// opponend revealed his piece
	if (turn.opponent_revealed.owner == opponent) {
		ArmyStateOpponent.Revealed[turn.opponent_revealed.value]++;
		ArmyStateOpponent.Hidden[turn.opponent_revealed.value]--;

	}

	// you killed your piece 
	if (turn.you_killed[1].owner == you) {
		ArmyStateMy.Dead[turn.you_killed[1].value]++;
		if (turn.you_killed[1].visible) {
			ArmyStateMy.Revealed[turn.you_killed[1].value]--;
		}
		else {
			ArmyStateMy.Hidden[turn.you_killed[1].value]--;
		}
	}
	// opponend killed your piece
	if (turn.opponent_killed[0].owner == you) {
		ArmyStateMy.Dead[turn.opponent_killed[0].value]++;
		if (turn.opponent_killed[0].visible) {
			ArmyStateMy.Revealed[turn.opponent_killed[0].value]--;
		}
		else {
			ArmyStateMy.Hidden[turn.opponent_killed[0].value]--;
		}
	}
	// you revealed your piece
	if (turn.you_revealed.owner == you) {
		ArmyStateMy.Revealed[turn.you_revealed.value]++;
		ArmyStateMy.Hidden[turn.you_revealed.value]--;

	}
	// opponend reveales your piece
	if (turn.opponent_revealed.owner == you) {
		ArmyStateMy.Revealed[turn.opponent_revealed.value]++;
		ArmyStateMy.Hidden[turn.opponent_revealed.value]--;

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

// returs float with the expected score difference of the trade
float ScoreAI::evaluate_trade(Piece M, FractPiece T) {
	float P = 0;
	return P;
}

//generates starting position
Start_pos ScoreAI::startPos() {

	Start_pos r;
	return r;
}

//make a move
Move ScoreAI::move(Tile field[10][10], Move opponent_move, Turn turn) {
	update_army(field, turn);
	Move r;
	return r;
}
