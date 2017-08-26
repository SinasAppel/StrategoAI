#include "generating.h"
#include "AI1.h"

//Creates the initial empty board
void createBoard(Tile field[10][10]) 
{
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if ((i == 4 || i == 5) && (j == 2 || j == 3 || j == 6 || j == 7)) {
				field[i][j] = cleanWaterTile();
			}
			else {
				field[i][j] = cleanGrassTile();
			}
		}
	}
}
/**
 * fillBoard handles the starting positions of the AI('s).
 * Manipulates the field of the base game.
 */
void fillBoard (Tile field[10][10]) 
{
	// No second ai yet, identical boards as a result
	AI1 AiOne;
	Start_pos startPos1 = AiOne.startPos();
	Start_pos startPos2 = AiOne.startPos();
	
	for (int i = 0; i < 10; i++) {
		int j = 10 - i - 1;
		field[0][i].piece = startPos1.row0[i];
		field[1][i].piece = startPos1.row1[i];
		field[2][i].piece = startPos1.row2[i];
		field[3][i].piece = startPos1.row3[i];
		field[6][i].piece = startPos2.row3[j];
		field[7][i].piece = startPos2.row2[j];
		field[8][i].piece = startPos2.row1[j];
		field[9][i].piece = startPos2.row0[j];
	}
}

/**
 * Creates an empty grass tile
 */
Tile cleanGrassTile() 
{
	Tile g;
	g.land = 'G';
	g.piece.value = -1;
	g.piece.name = 'E';
	return g;
}

/**
 * Creates an empty water tile
 */
Tile cleanGrassTile() 
{
	Tile w;
	w.land = 'W';
	w.piece.value = -1;
	w.piece.name = 'E';
	return g;
}
