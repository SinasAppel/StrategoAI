#define	SANDERAI2_H

struct ArmyState {
	int Hidden[12];// F,1,2,3,4,5,6,7,8,9,10,B
	int Revealed[12];
	int Dead[12];
	int totalHidden;
	int totalRevealed;
	int totalDead;
	ArmyState();
};

struct FractPiece {
	float frac[12]; // The chance(0 - 1) of that piece being a F,1,2,3,4,5,6,7,8,9,10,B
	bool canMove;// true if the piece can move
	int x;
	int y;
	bool empty;
	FractPiece();
};

struct Scores {
	float HiddenPoints[12];// The points for a hidden piece by type: flag 1,2,3,4,5,6,7,8,9,10 or a bomb
	float RevealedPoints[12];// The points for a revealed piece by type: flag 1,2,3,4,5,6,7,8,9,10 or a bomb
	float DeadPoints[12];// The points for a dead piece by type: flag 1,2,3,4,5,6,7,8,9,10 or a bomb
	Scores();
};

class ScoreAI : public AI {
public:
	Start_pos startPos();
	Move move(Tile field[10][10], Turn turn);
	ScoreAI(int p /*playernumber*/);
private:
	void update_army(Turn turn);
	float evaluate_armies(void);
	float evaluateTrade(Piece attacker, Piece defender);
	float evaluateFractTrade(Piece M, FractPiece T);
	void make_FractField(Tile field[10][10]);
	void check_for_moves(Tile field[10][10], Tile myMoves[40], FractPiece opponentMoves[40]);
	void search_map(bool seached[10][10], FractPiece pieceReach[40], int x, int y);
	void make_move_matrix(Tile field[10][10], Tile myMoves[40], FractPiece myReach[40][40]);
	void ScoreAI::score_moves(Tile myMoves[40], FractPiece opponentMoves[40], float scoreMatrix[40][40]);
	Move generate_move(Tile field[10][10], Tile Attacker, FractPiece Target);
	FractPiece FractField[10][10];
	Scores Points;
	ArmyState ArmyStateOpponent;
	ArmyState ArmyStateMy;
};
