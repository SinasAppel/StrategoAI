#include "objects.h"

char Piece::getValue() 
{
	return value;
}

char Tile::getValue() 
{
	return this->piece.getValue() != 0 ? this->piece.getValue() : value;
}

Grass::Grass() {
	value = '0';
}

void Grass::setPiece(Piece p) {
	piece = p;
}

Water::Water() {
	value = 'W';
}
