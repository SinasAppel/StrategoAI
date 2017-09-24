#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <ctime>
#include "generating.h"
#include "SanderAI.h"
using namespace std;

// Standard constructor for AI1
SanderAI::SanderAI(int p) : AI(p) {
}

// default constructor for Army
Army::Army(){
	int hid[11] = { 6, 1, 8, 5, 4, 4, 4, 3, 2, 1, 1 };
	for (int T1 = 0; T1 < 11; T1++){
		Hidden[T1] = hid[T1];
		Revealed[T1] = 0;
		Dead[T1] = 0;
	}
}

// gives a random unice piece from an array
Piece SanderAI::rand_from_array(int in[], int size)
{
	srand(time(0));
	int V = -1, R;
	while (V == -1)
	{
		R = rand() % size;
		V = in[R];
	}
	in[R] = -1;
	Piece p = Piece(V, playerNumber);
	return p;
}

//updates the private frontline arrays
void SanderAI::update_frontline(Tile field[10][10])
{
	int T3;
	for (int T1 = 0; T1 < 10; T1++){
		T3 = 9 - T1;
		for (int T2 = 0; T2 < 10; T2++){
			if (field[T1][T2].piece.owner == playerNumber){
				My.Line[T2] = field[T1][T2].piece;
				My.Ypos[T2] = T1;
			}
			
			if (field[T3][T2].piece.owner == (playerNumber == 1 ? 2 : 1)){
				Opponent.Line[T2] = field[T3][T2].piece;
				Opponent.Ypos[T2] = T1;
			}
		}
	}
}

void SanderAI::update_army(Tile field[10][10], Turn turn)
{
	int opponent = playerNumber == 1 ? 2 : 1;
	if (turn.you_killed[0].name != 'E' && turn.you_killed[0].owner == opponent){
		Enemy.Dead[turn.you_killed[0].value]++;
		if (turn.you_killed[0].visible){
			Enemy.Revealed[turn.you_killed[0].value]--;
		}
		else {
			Enemy.Hidden[turn.you_killed[0].value]--;
		}
	}

	for (int T1 = 0; T1 < 10; T1++){
		for (int T2 = 0; T2 < 10; T2++){
			if (field[T1][T2].piece.owner == opponent){
				
			}
		}
	}
}

// gives the base game the starting position.
Start_pos SanderAI::startPos()
{
	srand(time(0));
	Start_pos output;
	int row01[4] = { 0, 4, 5, 5 }, row02[6] = { 3, 4, 4, 4, 5, 5 };
	int row1[4] = { 1, 9, 8, 10 };
	int row3[8] = { 6, 6, 7, 7, 7, 8 };

	for (int T1 = 0; T1 < 10; T1++)
	{
		//making row0
		if (T1 < 2 || T1 > 7){
			output.row0[T1] = rand_from_array(row01, 4);
		}
		else if (T1 >= 2 && T1 <= 7){
			output.row0[T1] = rand_from_array(row02, 6);
		}
		//making row1
		if (T1 < 3 || T1 > 6){ output.row1[T1] = Piece(11, playerNumber); }
		else if (T1 >= 3 && T1 <= 6){ output.row1[T1] = Piece(row1[T1 - 3], playerNumber); }

		//making row2
		if (T1 < 2 || T1 > 7 || T1 == 4 || T1 == 5){ output.row2[T1] = Piece(2, playerNumber); }
		else if (T1 == 3 || T1 == 6){ output.row2[T1] = Piece(3, playerNumber); }
		else if (T1 == 2 || T1 == 7){ output.row2[T1] = Piece(6, playerNumber); }

		//making row3
		if (T1 == 3 || T1 == 6){ output.row3[T1] = Piece(2, playerNumber); }
		else if (T1 == 2 || T1 == 7){ output.row3[T1] = Piece(3, playerNumber); }
		else if (output.row3[T1].value != 2){
			output.row3[T1] = rand_from_array(row3, 6);
		}

	}

	mode = 1;// mode om in te beginnen

	return output;
}

Move SanderAI::move(Tile field[10][10], Move opponent_move, Turn turn)
{
	srand(time(0));
	Move output;

	update_frontline(field);
	update_army(field, turn);

	//update de hasmoved map
	if (opponent_move.x != -1){
		int opp_des_x, opp_des_y;
		if (opponent_move.cardinal == 'N'){ opp_des_x = opponent_move.x; opp_des_y = opponent_move.y - 1; }
		else if (opponent_move.cardinal == 'E'){ opp_des_x = opponent_move.x + 1; opp_des_y = opponent_move.y; }
		else if (opponent_move.cardinal == 'S'){ opp_des_x = opponent_move.x; opp_des_y = opponent_move.y + 1; }
		else if (opponent_move.cardinal == 'W'){ opp_des_x = opponent_move.x - 1; opp_des_y = opponent_move.y; }
		hasmoved[opp_des_x][opp_des_y] = 1;
	}

	//steps
	//mode1: breach through the small pieces
	if (mode == 1){
		for (int T1 = 0; T1 < 10; T1++){
			if (My.Line[T1].value > 5 && My.Line[T1].value < 9)
			{
				if (My.Ypos[T1] != 9){
					if (My.Line[T1].value >= Opponent.Line[T1].value){
						output.x = T1; output.y = My.Ypos[T1]; output.cardinal = 'S'; output.no_moves = false;
						return output;
					}
				}
				else {
					if (T1 != 0){
						output.x = T1; output.y = My.Ypos[T1]; output.cardinal = 'W'; output.no_moves = false;
						return output;
					}
					else{
						output.x = T1; output.y = My.Ypos[T1]; output.cardinal = 'E'; output.no_moves = false;
						return output;
					}
				}
			}
		}
		printf("MODE2 MODE2 MODE2 MODE2 MODE2 MODE2 MODE2\n");
		mode = 2;//mode 3 als de 10 of 9 bekend is
	}
	//mode2: reviele the big pieces
	if (mode == 2){
		
	}
	//mode3: make you pieces star and plus
	if (mode == 3){

	}
	//mode4: attack all that moves
	if (mode == 4){
	
	}

	output.no_moves = true;
	//printf("AI%i: %i, %i, %c\n", playerNumber, output.x, output.y, output.cardinal);
	return output;
}
