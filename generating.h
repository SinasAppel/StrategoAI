#ifndef STRATEGOAI_GENERATING_H
#define	STRATEGOAI_GENERATING_H

// General piece, can be a F, B, 1, 2, 3, etc.
struct Piece {
	int value;
	char name;
	int owner;
	bool visible;
	Piece();
	Piece(char n, int o);
	Piece(int v, int o);
};

// General Tile, land can be water or grass and can contain a piece
struct Tile {
	char land;
	Piece piece;
};

struct Grass: public Tile {
	Grass();
};

struct Water: public Tile {
	Water();
};

/*
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
	bool no_moves;
};

// A way to pass the starting positions easely
struct Start_pos {
	Piece row0[10];
	Piece row1[10];
	Piece row2[10];
	Piece row3[10];
};

/*
 * Data struct that gets returned after a game
 * which holds relevant data to report
 */
struct Game {
	int playerWon;
	int turns;
	float AI1_time;
	float AI2_time;
};

class AI {
	public:
		int playerNumber;
		AI(int p /*playernumber*/);
		virtual Start_pos startPos() {
			Start_pos output;
			return output;
		}
		virtual Move move(Tile field[10][10], Move opponent_move) {
			Move move = { 0, 0, '0', false };
			return move;
		}
};

void createBoard(Tile field[10][10]);
void fillBoard(Tile field[10][10], Start_pos startPos1, Start_pos startPos2);
Tile cleanGrassTile();
Tile cleanWaterTile();

#endif