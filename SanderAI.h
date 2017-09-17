#define	SANDERAI_H

struct Frontline {
	Piece Line[10];
	int Ypos[10];
};

class SanderAI : public AI {
public:
	Start_pos startPos();
	Move move(Tile field[10][10], Move opponent_move);
	SanderAI::SanderAI(int p /*playernumber*/);
private:
	Piece rand_from_array(int in[], int size);
	void update_frontline(Tile field[10][10]);
	int hasmoved[10][10] = {};
	Frontline My;
	Frontline Opponent;
	int mode;
};

