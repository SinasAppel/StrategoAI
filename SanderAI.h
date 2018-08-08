#define	SANDERAI_H

struct Frontline {
	Piece Line[10];
	int Ypos[10];
};

struct Army {
	int Hidden[11];// 0 is bombs and 1 is number of ones and so on
	int Revealed[11];
	int Dead[11];
	Army();
};

class SanderAI : public AI {
public:
	Start_pos startPos();
	Move move(Tile field[10][10], Move opponent_move, Turn turn);
	SanderAI(int p /*playernumber*/);
private:
	Piece rand_from_array(int in[], int size);
	void update_frontline(Tile field[10][10]);
	void update_army(Tile field[10][10], Turn turn);
	Tile search_near(Piece target, Tile begin);
	int hasmoved[10][10] = {};
	Frontline My;
	Frontline Opponent;
	Army Enemy;
	Army Soldiers;
	int mode;
};

