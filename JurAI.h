#define	JurAI_H

class JurAI {
	public:
		Start_pos startPos();
		Move move(Tile field[10][10], Move opponent_move);
		int playerNumber;
	private:
		int hasmoved[10][10] = {};
		int evaluate_tile(Tile target, int piece_falue);
};

