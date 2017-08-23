#define	Objects_H

// General piece, can be a 1, 2, 3, etc.
class Piece {
	protected:
		int value;
	public:
		int getValue();
};

// General Tile, can be water or grass
class Tile {
	protected:
		char name;
		Piece piece;
	public:
		char getName();
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

