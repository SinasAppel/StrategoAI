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
	bool canMove;// true if the spiece can move
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
	void update_army(Tile field[10][10], Turn turn);
	float evaluate_armies(void);
	float evaluateTrade(Piece attacker, Piece defender, Scores score);
	float evaluateFractTrade(Piece M, FractPiece T, Scores score);
	void make_FractField(Tile field[10][10]);
	FractPiece FractField[10][10];
	Scores Points;
	ArmyState ArmyStateOpponent;
	ArmyState ArmyStateMy;
};
