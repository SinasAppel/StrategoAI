#define	SANDERAI2_H

struct ArmyState {
	int Hidden[11];// 0 is bombs and 1 is number of ones and so on
	int Revealed[11];
	int Dead[11];
	ArmyState();
};

struct FractPiece {
	float frac[12]; // The change of that piece being a flag 1,2,3,4,5,6,7,8,9,10 or a bomb
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
	float ScoreAI::evaluate_trade(Piece M, Piece T, Scores score);
	float evaluate_FractTrade(Piece M, FractPiece T, Scores score);
	FractPiece Fractfield[10][10];
	Scores Points;
	ArmyState ArmyStateOpponent;
	ArmyState ArmyStateMy;
};
