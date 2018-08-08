#ifndef STRATEGOAI_BOARD_H
#define STRATEGOAI_BOARD_H
#include "generating.h"

class Board {

public:
    Board(Start_pos startPos1, Start_pos startPos2);
    void print(const Tile board[10][10]);
    Tile field[10][10];
    Tile player1Field[10][10];
    Tile player2Field[10][10];
    void updatePlayerBoard(int playerNumber);
private:
};


#endif //STRATEGOAI_BOARD_H
