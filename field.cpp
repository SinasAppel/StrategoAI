#include "field.h"
#include <stdio.h>
#include "generating.cpp"

/**
 * Constructor for the Field class, for making a main field, with start positions
 * Creates an empty board and then fills it with the start positions of the AI's
 * which are given as parameters.
 */
Field::Field(Start_pos startPos1, Start_pos startPos2) {
    //Create the initial empty board
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if ((i == 4 || i == 5) && (j == 2 || j == 3 || j == 6 || j == 7)) {
                this->mainField[i][j] = cleanWaterTile();
            } else {
                this->mainField[i][j] = cleanGrassTile();
            }
        }
    }

    //Fill the board with the start positions
    //TODO: this does not look sexy, maybe this can be nicer? works though
    for (int i = 0; i < 10; i++) {
        int j = 10 - i - 1;
        this->mainField[0][i].piece = startPos1.row0[i];
        this->mainField[1][i].piece = startPos1.row1[i];
        this->mainField[2][i].piece = startPos1.row2[i];
        this->mainField[3][i].piece = startPos1.row3[i];
        this->mainField[6][i].piece = startPos2.row3[j];
        this->mainField[7][i].piece = startPos2.row2[j];
        this->mainField[8][i].piece = startPos2.row1[j];
        this->mainField[9][i].piece = startPos2.row0[j];
    }
}

/**
 * Prints a board of Tiles
 * Input: a 10 by 10 array of Tiles.
 * Output: nothing.
 */
void Field::print() {
    for (int T1 = 0; T1 < 10; T1++) {
        for (int T2 = 0; T2 < 10; T2++) {
            //the value of a empty tile is -1 so then the land type needs to be printed
            if (this->mainField[T1][T2].piece.value == EMPTY_PIECE_VALUE) {
                printf(" %c", this->mainField[T1][T2].land);
            }
                //If the Tile is not empty the piece will be represented by its name.
                //With F being the flag, B being the bom and T being the ten so the program does not need to print dubble digits.
            else {
                printf(" %c", this->mainField[T1][T2].piece.name);
            }
        }
        printf("\n");
    }
    printf("\n");
}

/*
 * Hides the data the AI should not be able to see from its opponent
 * Input: The 10 by 10 field of Tiles that the game keeps track of.
 * Input: The player number of the player that is going to recieve the board.
 * Output: The field that the player gets to see.
 */
PlayerField::PlayerField(int playerNumber, Field field) {
    this->playerNumber = playerNumber;
    this->updateField(field);
}

/*
 * Call this when you want an updated version of the
 * playerField data
 */
void PlayerField::updateField(Field field) {
    for (int T1 = 0; T1 < 10; T1++) {
        for (int T2 = 0; T2 < 10; T2++) {
            if (!(field.mainField[T1][T2].piece.owner == this->playerNumber || field.mainField[T1][T2].piece.visible)) {
                this->playerField[T1][T2].piece.name = INVISIBLE_PIECE_NAME;
                this->playerField[T1][T2].piece.value = INVISIBLE_PIECE_VALUE;
            } else {
                // is visible or is owned by the player, all data is visible
                this->playerField[T1][T2] = field.mainField[T1][T2];
            }
        }
    }
}