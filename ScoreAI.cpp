#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include "generating.h"
#include "ScoreAI.h"
using namespace std;

SanderAI2::SanderAI2(int p) : AI(p) {
}

// default constructor for Army
Army2::Army2() {
	int hid[11] = { 6, 1, 8, 5, 4, 4, 4, 3, 2, 1, 1 };
	for (int T1 = 0; T1 < 11; T1++) {
		Hidden[T1] = hid[T1];
		Revealed[T1] = 0;
		Dead[T1] = 0;
	}
}

// default constructor for Fract_Piece
FractPiece::FractPiece() {
	
	pro[0] = 0;
	pro[1] = 0;
	pro[2] = 0;
	pro[3] = 0;
	pro[4] = 0;
	pro[5] = 0;
	pro[6] = 0;
	pro[7] = 0;
	pro[8] = 0;
	pro[9] = 0;
	pro[10] = 0;
	pro[11] = 0;
}

// default constructor for scores
Scores::Scores() {
	int HPi[12] = { 22, 31, 13, 13, 15, 17, 20, 23, 27, 31, 40 , 70};
	int RPi[12] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 , 0};
	int DPi[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 , 0};
	for (int T1 = 0; T1 < 12; T1++) {
		HP[T1] = HPi[T1];
		RP[T1] = RPi[T1];
		DP[T1] = DPi[T1];
	}
}

void SanderAI2::update_army(Tile field[10][10], Turn turn)
{
	int opponent = playerNumber == 1 ? 2 : 1;
	int you = playerNumber;
	// you killed piece of opponent
	if (turn.you_killed[0].owner == opponent) {
		Enemy.Dead[turn.you_killed[0].value]++;
		if (turn.you_killed[0].visible) {
			Enemy.Revealed[turn.you_killed[0].value]--;
		}
		else {
			Enemy.Hidden[turn.you_killed[0].value]--;
		}
	}
	// opponend killed your piece
	if (turn.opponent_killed[1].owner == opponent) {
		Enemy.Dead[turn.opponent_killed[1].value]++;
		if (turn.opponent_killed[1].visible) {
			Enemy.Revealed[turn.opponent_killed[1].value]--;
		}
		else {
			Enemy.Hidden[turn.opponent_killed[1].value]--;
		}
	}
	// you revealed opponents piece
	if (turn.you_revealed.owner == opponent) {
		Enemy.Revealed[turn.you_revealed.value]++;
		Enemy.Hidden[turn.you_revealed.value]--;

	}
	// opponend revealed his piece
	if (turn.opponent_revealed.owner == opponent) {
		Enemy.Revealed[turn.opponent_revealed.value]++;
		Enemy.Hidden[turn.opponent_revealed.value]--;

	}

	// you killed your piece 
	if (turn.you_killed[1].owner == you) {
		Soldiers.Dead[turn.you_killed[1].value]++;
		if (turn.you_killed[1].visible) {
			Soldiers.Revealed[turn.you_killed[1].value]--;
		}
		else {
			Soldiers.Hidden[turn.you_killed[1].value]--;
		}
	}
	// opponend killed your piece
	if (turn.opponent_killed[0].owner == you) {
		Soldiers.Dead[turn.opponent_killed[0].value]++;
		if (turn.opponent_killed[0].visible) {
			Soldiers.Revealed[turn.opponent_killed[0].value]--;
		}
		else {
			Soldiers.Hidden[turn.opponent_killed[0].value]--;
		}
	}
	// you revealed your piece
	if (turn.you_revealed.owner == you) {
		Soldiers.Revealed[turn.you_revealed.value]++;
		Soldiers.Hidden[turn.you_revealed.value]--;

	}
	// opponend reveales your piece
	if (turn.opponent_revealed.owner == you) {
		Soldiers.Revealed[turn.opponent_revealed.value]++;
		Soldiers.Hidden[turn.opponent_revealed.value]--;

	}
}

//give a score of a field
float SanderAI2::evaluate_armies(void)
{
	// waardes van de stukken
	//Stukken	b	1	2	3	4	5	6	7	8	9	10
	//Hidden	22	31	13	13	15	17	20	23	27	31  40	
	//Revealed	0	1	2	3	4	5	6	7	8	9	10
	//Dead		0	0	0	0	0	0	0	0	0	0	0
	float total = 0;
	for (int T1 = 0; T1 < 11; T1++) 
	{
		total = total + Soldiers.Hidden[T1] * Points.HP[T1] + Soldiers.Revealed[T1] * Points.RP[T1] + Soldiers.Dead[T1] * Points.DP[T1] - Enemy.Hidden[T1] * Points.HP[T1] - Enemy.Revealed[T1] * Points.RP[T1] - Enemy.Dead[T1] * Points.DP[T1];
	}

	return total;	
}

float SanderAI2::evaluate_trade(Piece M, FractPiece T) {
	float P = 0;
	return P;
}

Start_pos SanderAI2::startPos() {

	Start_pos r;
	return r;
}

Move SanderAI2::move(Tile field[10][10], Move opponent_move, Turn turn) {
	update_army(field, turn);
	Move r;
	return r;
}
