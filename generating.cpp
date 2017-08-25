#include "generating.h"
#include "AI1.h"

//Creates the initial empty board
void createBoard(Tile field[10][10]) {
	for(int i=0; i < 10; i++) {
		for(int j=0; j < 10; j++) {
			if ((i==4 || i==5) && (j==2 || j==3 || j==6 || j==7)) {
				field[i][j].land = w;
			} 
			else {
				field[i][j].land = g;
			}
		}
	}
}

//fills the board with the start positions of the AIs
void fillBoard(Tile field[10][10])
{
	Start_pos AI1_pos = AI1_start_pos_gen();
	Start_pos AI2_pos = AI2_start_pos_gen(); //is nog geen voorbeeld voor
	
	for(int i=0; i < 10; i++) {
		int j = 10 -i;
		field[0][i].piece = AI1_pos.row0[i];
		field[1][i].piece = AI1_pos.row1[i];
		field[2][i].piece = AI1_pos.row2[i];
		field[3][i].piece = AI1_pos.row3[i];
		field[6][i].piece = AI2_pos.row3[j];
		field[7][i].piece = AI2_pos.row2[j];
		field[8][i].piece = AI2_pos.row1[j];
		field[9][i].piece = AI2_pos.row0[j];
	}
}
