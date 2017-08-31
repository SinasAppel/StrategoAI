#define	AI1_H

class AI1 {
	public:
		Start_pos startPos();
		Move move(Tile field[10][10], Move opponent_move);
		int playerNumber;
		AI1(int p /*playernumber*/);
	private:
		int hasmoved[10][10] = {};
		int evaluate_tile(Tile target, int piece_falue);
};
