#define	SANDERAI_H

class SanderAI : public AI {
public:
	Start_pos startPos();
	Move move(Tile field[10][10], Move opponent_move);
	SanderAI::SanderAI(int p /*playernumber*/);
private:
	Piece rand_from_array(int in[], int size);
	int hasmoved[10][10] = {};
};
