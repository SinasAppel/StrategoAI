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
	canMove = false;
	empty = true;
	x = EMPTY_FRACTPIECE_X;
	y = EMPTY_FRACTPIECE_Y;
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
void ScoreAI::update_army(Turn turn)
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
float ScoreAI::evaluateTrade(Piece attacker, Piece defender) {
	float p = 0;
	int combatResult = -10;
	Scores score;

	// if defender is the flag
	if (defender.name == FLAG_NAME) {
		combatResult = COMBAT_WON;
	}

	// 3 against bomb:
	if (attacker.value == 3 && defender.name == BOMB_NAME) {
		combatResult = COMBAT_WON;
	}

	// 1 against 10:
	if (attacker.value == 1 && defender.value == 10) {
		combatResult = COMBAT_WON;
	}

	// combat versus bomb
	if (defender.name == BOMB_NAME) {
		combatResult = COMBAT_LOST;
	}

	if (combatResult == -10) {
		// normal combat
		if (attacker.value > defender.value) {
			combatResult = COMBAT_WON;
		} else if (attacker.value == defender.value) {
			combatResult = COMBAT_DRAW;
		} else {
			combatResult = COMBAT_LOST;
		}
	}

	if (combatResult == COMBAT_WON) {
		if (!defender.visible) {
			p = p + score.HiddenPoints[defender.value];
		}
		else {
			p = p + score.RevealedPoints[defender.value];
		}
		if (!attacker.visible) {
			p = p - score.HiddenPoints[attacker.value] + score.RevealedPoints[attacker.value];
		}

	} else if (combatResult == COMBAT_LOST) {
		if (!attacker.visible) {
			p = p - score.HiddenPoints[attacker.value];
		}
		else {
			p = p - score.RevealedPoints[attacker.value];
		}
		if (!defender.visible) {
			p = p + score.HiddenPoints[attacker.value] - score.RevealedPoints[attacker.value];
		}
	} else {
		if (!attacker.visible) {
			p = p - score.HiddenPoints[attacker.value];
		}
		else {
			p = p - score.RevealedPoints[attacker.value];
		}
		if (defender.visible == false) {
			p = p + score.HiddenPoints[defender.value];
		}
		else {
			p = p + score.RevealedPoints[defender.value];
		}
	}

	return p;
}

/**
 * @param attacker
 * @param defender
 * @param score
 * @return (float) the sum of (chance of a certain piece * points gained/lost by attacking that piece)
 */
float ScoreAI::evaluateFractTrade(Piece attacker, FractPiece defender) {
	float p = 0;

	for (int i=0; i < 12; i++) {
		Piece defendingPiece(i, playerNumber);
		p += defender.frac[i] * evaluateTrade(attacker, defendingPiece);
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

// constructs a fract field from the current field
void ScoreAI::make_FractField(Tile field[10][10]) {
	for (int T1 = 0; T1 < 10; T1++) {
		for (int T2 = 0; T2 < 10; T2++) {// Go though the field
			if (field[T1][T2].piece.name != EMPTY_PIECE_NAME){// only give real pieces a value
				FractField[T1][T2].empty = false;
				if (field[T1][T2].piece.name != INVISIBLE_PIECE_NAME) {// if the piece is fisible you know the value
					FractField[T1][T2].frac[field[T1][T2].piece.value] = 1;// set the correct value
				}
				else { // else try to guess the value
					for (int T3 = 0; T3 < 12; T3++) {// go though all the chances of a piece
						FractField[T1][T2].frac[T3] = ArmyStateOpponent.Hidden[T3];// make a guess with the amount of hidden pieces of the kind
						FractField[T1][T2].frac[T3] = FractField[T1][T2].frac[T3] / ArmyStateOpponent.totalHidden;
					}
				}
			} else {// if the piece is empty set empty to true on the fractpiece
				FractField[T1][T2].empty = true;
			}
			FractField[T1][T2].x = T2;
			FractField[T1][T2].y = T1;
		}
	}
}

// computes witch pieces can move and adds them to the Moves array
void ScoreAI::check_for_moves(Tile field[10][10], Tile myMoves[40], FractPiece opponentMoves[40]){
	int opponentNumber = playerNumber == 1 ? 2 : 1;
	for (int T1 = 0; T1 < 10; T1++){
		for (int T2 = 0; T2 < 10; T2++){// Go though the field
			int neighbours = 0;// begin with zero neighbours
			if (T1 == 0 || field[T1 - 1][T2].piece.name != EMPTY_PIECE_NAME || field[T1 - 1][T2].land == TILE_WATER){ neighbours++; }// If a piece can't move in a direction add a neighbour
			if (T1 == 9 || field[T1 + 1][T2].piece.name != EMPTY_PIECE_NAME || field[T1 + 1][T2].land == TILE_WATER){ neighbours++; }
			if (T2 == 0 || field[T1][T2 - 1].piece.name != EMPTY_PIECE_NAME || field[T1][T2 - 1].land == TILE_WATER){ neighbours++; }
			if (T2 == 9 || field[T1][T2 + 1].piece.name != EMPTY_PIECE_NAME || field[T1][T2 + 1].land == TILE_WATER){ neighbours++; }
			if (neighbours != 4 && field[T1][T2].piece.name != FLAG_NAME && field[T1][T2].piece.name != BOMB_NAME){// it can move if it does not have four neighbours and is not a flag and not a bomb
				FractField[T1][T2].canMove = true; // if the amount of neighbours is not four set it can move
				for (int T3 = 0; T3 < 40; T3++){// go though the list of pieces that can move
					if (field[T1][T2].piece.owner == playerNumber){// if it is your piece check myMoves
						if (myMoves[T3].piece.name == EMPTY_PIECE_NAME){// only place the piece if there is an empty spot
							myMoves[T3] = field[T1][T2];
							break;
						}
					}
					if (field[T1][T2].piece.owner == opponentNumber){// if it is not your piece check opponentMoves
						if (opponentMoves[T3].empty){// only place the piece if there is an empty spot
							opponentMoves[T3] = FractField[T1][T2];
							break;
						}
					}
				}
			}
		}
	}
}

//scores all the possible combinations of combat with distance
void ScoreAI::score_moves(Tile myMoves[40], FractPiece opponentMoves[40], float scoreMatrix[40][40]){
	int bestT1 = 0, bestT2 = 0;// keep track of the best trade
	for (int T1 = 0; T1 < 40; T1++){
		//if (myMoves[T1].piece.name != 'E'){ printf("\n"); printf("%c\n", myMoves[T1].piece.name); }
		for (int T2 = 0; T2 < 40; T2++){// go though all the combinations
			if (myMoves[T1].piece.name != EMPTY_PIECE_NAME && !opponentMoves[T2].empty){// if they are both falid pieces compute trade value
				float disMod = 1 / sqrt(abs(myMoves[T1].x - opponentMoves[T2].x) + abs(myMoves[T1].y - opponentMoves[T2].y));// calculate a distance modifier
				scoreMatrix[T1][T2] = evaluateFractTrade(myMoves[T1].piece, opponentMoves[T2]) * disMod;
				if (scoreMatrix[T1][T2] > scoreMatrix[bestT1][bestT2]){// if this combat has a higher score than the record make that the record
					bestT1 = T1;
					bestT2 = T2;
				}
				//printf("%.2f ", scoreMatrix[T1][T2]);
			}
		}
	}
	myMoves[0] = myMoves[bestT1];// move the best combat option to the front to pass pass it on
	opponentMoves[0] = opponentMoves[bestT2];
}

Move ScoreAI::generate_move(Tile field[10][10], Tile Attacker, FractPiece Target){
	int xdif = Target.x - Attacker.x, ydif = Target.y - Attacker.y;// calculate the directions the attacker wants to move in
	int N = -10000, S = -10000, E = -10000, W = -10000, best = -1000, T1 = 1;
	int opponentNumber = playerNumber == 1 ? 2 : 1;
	char cardinal = SOUTH;
	if (Attacker.piece.name == EMPTY_PIECE_NAME){ Move outputMove; outputMove.noMoves = true; return outputMove; }// if the piece is empty there are no pissible moves 

	//check the availeble cardinals
	// if the piece can move in that direction set the points to that option equal to the amount it wants to move in that direction
	// inbetween the biggest score is updated
	if (Attacker.y != 0 && field[Attacker.y - 1][Attacker.x].piece.owner != playerNumber && field[Attacker.y - 1][Attacker.x].land != TILE_WATER){ N = -1*ydif; }
	if (N > best){ cardinal = NORTH; best = N; }
	if (Attacker.y != 9 && field[Attacker.y + 1][Attacker.x].piece.owner != playerNumber && field[Attacker.y + 1][Attacker.x].land != TILE_WATER){ S = ydif; }
	if (S > best){ cardinal = SOUTH; best = S; }
	if (Attacker.x != 0 && field[Attacker.y][Attacker.x - 1].piece.owner != playerNumber && field[Attacker.y][Attacker.x - 1].land != TILE_WATER){ W = -1 * xdif; }
	if (W > best){ cardinal = WEST; best = W; }
	if (Attacker.x != 9 && field[Attacker.y][Attacker.x + 1].piece.owner != playerNumber && field[Attacker.y][Attacker.x + 1].land != TILE_WATER){ E = xdif; }
	if (E > best){ cardinal = EAST; best = E; }

	if (Attacker.piece.value == 2) {// a two can move further then one 
		for (T1; T1 < 10; T1++){// increase until the piece cant move annymove
			if(cardinal == NORTH &&			(Attacker.y == T1-1		|| field[Attacker.y - T1 - 1][Attacker.x].piece.owner == playerNumber || field[Attacker.y - T1][Attacker.x].piece.owner == opponentNumber || field[Attacker.y - T1][Attacker.x].land == TILE_WATER || T1 == abs(ydif))){ break; }
			else if (cardinal == SOUTH &&	(Attacker.y == 10 - T1	|| field[Attacker.y + T1 + 1][Attacker.x].piece.owner == playerNumber || field[Attacker.y + T1][Attacker.x].piece.owner == opponentNumber || field[Attacker.y + T1][Attacker.x].land == TILE_WATER || T1 == abs(ydif))) { break; }
			else if (cardinal == WEST &&	(Attacker.y == T1 - 1	|| field[Attacker.y][Attacker.x - T1 - 1].piece.owner == playerNumber || field[Attacker.y][Attacker.x - T1].piece.owner == opponentNumber || field[Attacker.y][Attacker.x - T1].land == TILE_WATER || T1 == abs(xdif))) { break; }
			else if (cardinal == EAST &&	(Attacker.y == 10 - T1	|| field[Attacker.y][Attacker.x + T1 + 1].piece.owner == playerNumber || field[Attacker.y][Attacker.x + T1].piece.owner == opponentNumber || field[Attacker.y][Attacker.x + T1].land == TILE_WATER || T1 == abs(xdif))) { break; }
		}
	}

	Move outputMove;
	outputMove.noMoves = false;
	outputMove.x = Attacker.x;
	outputMove.y = Attacker.y;
	outputMove.tiles = T1; 
	outputMove.cardinal = cardinal;
	if (best == -1000){ outputMove.noMoves = true; }// kill the move if the best is still -1000, that means it can not move. Some weird code but it works
	return outputMove;
}

//make a move
Move ScoreAI::move(Tile field[10][10], Turn turn) {
	update_army(turn);
	make_FractField(field);
	FractPiece opponentMoves[40];
	Tile myMoves[40];
	float scoreMatrix[40][40] = {};
	check_for_moves(field, myMoves, opponentMoves);
	score_moves(myMoves, opponentMoves, scoreMatrix);
	Move r = generate_move(field, myMoves[0], opponentMoves[0]);
	return r;
}
