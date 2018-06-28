#ifndef STRATEGOAI_FIELD_H
#define STRATEGOAI_FIELD_H
#include "generating.h"


class Field {
public:
    Tile mainField[10][10];
    Field(Start_pos startPos1, Start_pos startPos2);
    void print();
};

/**
 * This class is used to send data to the AI,
 * but only the data the AI is allowed to see.
 *
 * Construct a PlayerField object by giving it a playerNumber and the main field,
 * then you can access the playerField.
 * Example:
 * playerOneFieldObject = new PlayerField(1, mainField);
 * playerOneField = playerOneFieldObject->playerField;
 *
 */
class PlayerField {
public:
    Tile playerField[10][10] = {};
    PlayerField(int playerNumber, Field mainField);
};

#endif
