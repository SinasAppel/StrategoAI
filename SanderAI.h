#define	SANDERAI_H

class SanderAI : public AI {
public:
	Start_pos startPos();
	Move move(Tile field[10][10], Move opponent_move);
	SanderAI::SanderAI(int p /*playernumber*/);
private:
	Piece rand_from_array(int in[], int size);
	void update_frontline(Tile field[10][10]);
	int hasmoved[10][10] = {};
	Piece Frontline_Opponent[10];
	Piece Frontline_My[10];
	int mode;
};
