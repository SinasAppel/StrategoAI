#ifndef STRATEGOAI_BOARD_H
#define STRATEGOAI_BOARD_H
#include "generating.h"

class Board {

public:
    Board(Start_pos startPos1, Start_pos startPos2);
    void print(const Tile board[10][10]);
    Tile field[10][10];

    // playerfields; fields with only the information an AI is allowed to know
    Tile player1Field[10][10];
    Tile player2Field[10][10];

	/**
 	 * Hides the data the AI should not be able to see from its opponent
 	 * Input: The 10 by 10 field of Tiles that the game keeps track on.
 	 * Input: The player number of the player that is going to recieve the board.
 	 * Output: The field that the player gets to see.
 	 * it now also turn the field arround for player2 so the Ai always plays on top
 	 */
    void updatePlayerBoard(int playerNumber);
private:
};


#endif //STRATEGOAI_BOARD_H
