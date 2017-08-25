#define	Generating_H

// General piece, can be a F, B, 1, 2, 3, etc.
struct Piece {
	int value;
	char name;
};

// General Tile, land can be water or grass and can contain a piece
struct Tile {
	char land;
	Piece piece;
};

// A way to pass the starting positions easely
struct Start_pos {
	Piece row0[10];
	Piece row1[10];
	Piece row2[10];
	Piece row3[10];
};
