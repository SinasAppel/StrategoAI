#ifndef Generating_H
#define Generating_H
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
	int x;
	int y;
	Piece piece;
};

struct Grass: public Tile {
	Grass(int X, int Y);
};

struct Water: public Tile {
	Water(int X, int Y);
};

/*
 * Move struct that an AI returns when called for it's next move
 * x and y are the coordinates of the current position.
 * cardinal is the direction the piece should go.
 * cardinal can be 'N' for north, 'E' for east,
 * 'W' for west and 'S' for south
 * incase the 2 wants to move 3 tiles that is specivied in tiles
 */

struct Move {
	int x;
	int y;
	char cardinal;
	int tiles;
	bool noMoves;
	Move();
};

struct Turn{
	int count;
	Move youMoved;
	Move opponentMoved;
	Piece youKilled[2];//[0]: opponent piece [1] own piece
	Piece opponentKilled[2];//[0]: your piece [1] own piece
	Piece youRevealed;
	Piece opponentRevealed;
	bool error;
	Turn();
	Turn(Turn players_turn, Turn opponents_turn);
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
	float AI1Time;
	float AI2Time;
};

class AI {
	public:
		int playerNumber;
		AI(int p /*playernumber*/);
		virtual Start_pos startPos() {
			Start_pos output;
			return output;
		}
		virtual Move move(Tile field[10][10], Turn turn) {
			Move move = Move();
			return move;
		}
};

void createBoard(Tile field[10][10]);
void fillBoard(Tile field[10][10], Start_pos startPos1, Start_pos startPos2);
Tile cleanGrassTile(int X, int Y);
Tile cleanWaterTile(int X, int Y);
#endif