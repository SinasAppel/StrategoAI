#include "objects.h"

int Piece::getValue() 
{
	return value;
}


char Tile::getName() 
{
	return name;
}

Grass::Grass() {
	name = '0';
}

void Grass::setPiece(Piece p) {
	piece = p;
}

Water::Water() {
	name = 'W';
}
