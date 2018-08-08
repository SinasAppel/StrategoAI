#define	SANDERAI2_H

struct Army2 {
	int Hidden[11];// 0 is bombs and 1 is number of ones and so on
	int Revealed[11];
	int Dead[11];
	Army2();
};

struct FractPiece {
	float pro[12];
	FractPiece();
};

struct Scores {
	float HP[12];
	float RP[12];
	float DP[12];
	float ST[12][12];
	Scores();
};

class ScoreAI : public AI {
public:
	Start_pos startPos();
	Move move(Tile field[10][10], Move opponent_move, Turn turn);
	ScoreAI(int p /*playernumber*/);
private:
	void update_army(Tile field[10][10], Turn turn);
	float evaluate_armies(void);
	float evaluate_trade(Piece M, FractPiece T);
	FractPiece Fractfield[10][10];
	Scores Points;
	Army2 Enemy;
	Army2 Soldiers;
};
