#define	Objects_H

// General piece, can be a 1, 2, 3, etc.
class Piece {
	protected:
		char value;
	public:
		char getValue();
};

// General Tile, can be water or grass
class Tile {
	protected:
		char value;
		Piece piece;
	public:
		char getValue();
};

// Specific Grass tile
class Grass: public Tile {
	public:
		Grass();
		void setPiece(Piece p);
};

// Specific Water tile
class Water: public Tile {
	public:
		Water();
};

