#define	JurAI_H

class JurAI : public AI {
	public:
		Start_pos startPos();
		Move move(Tile field[10][10], Turn turn);
		JurAI(int p /*playernumber*/);
	private:
		int hasmoved[10][10] = {};
		int evaluate_tile(Tile target, int piece_falue);
		void updateHasMoved(Move);
};

