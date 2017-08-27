#define	Generating_H

// General piece, can be a F, B, 1, 2, 3, etc.
struct Piece {
	int value;
	char name;
	int owner;
	bool visible;
	Piece();
	Piece(char n);
	Piece(int v);
};

// General Tile, land can be water or grass and can contain a piece
struct Tile {
	char land;
	Piece piece;
};

// Grass struct
struct Grass: public Tile {
	Grass();
};

// Water struct
struct Water: public Tile {
	Water();
};

/**
 * Move struct that an AI returns when called for it's next move
 * x and y are the coordinates of the current position.
 * cardinal is the direction the piece should go.
 * cardinal can be 'N' for north, 'E' for east, 
 * 'W' for west and 'S' for south 
 */
struct Move {
	int x;
	int y;
	char cardinal;
};

// A way to pass the starting positions easely
struct Start_pos {
	Piece row0[10];
	Piece row1[10];
	Piece row2[10];
	Piece row3[10];
};

void createBoard(Tile field[10][10]);
void fillBoard(Tile field[10][10], Start_pos startPos1, Start_pos startPos2);
Tile cleanGrassTile();
Tile cleanWaterTile();
