#include "Board.h"
#include "generating.h"
#include "definitions.cpp"
#include <iostream>
#include <termcolor.hpp>
#include <stdio.h>

Board::Board(Start_pos startPos1, Start_pos startPos2) {
	// create tiles
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if ((i == 4 || i == 5) && (j == 2 || j == 3 || j == 6 || j == 7)) {
				this->field[i][j] = cleanWaterTile(j,i);
			}
			else {
				this->field[i][j] = cleanGrassTile(j,i);
			}
		}
	}

	for (int i = 0; i < 10; i++) {
		int j = 10 - i - 1;
		field[0][i].piece = startPos1.row0[i];
		field[1][i].piece = startPos1.row1[i];
		field[2][i].piece = startPos1.row2[i];
		field[3][i].piece = startPos1.row3[i];
		field[6][i].piece = startPos2.row3[j];
		field[7][i].piece = startPos2.row2[j];
		field[8][i].piece = startPos2.row1[j];
		field[9][i].piece = startPos2.row0[j];
	}
}

void Board::updatePlayerBoard(int playerNumber) {

	for (int T1 = 0; T1 < 10; T1++) {
		for (int T2 = 0; T2 < 10; T2++) {
			player1Field[T1][T2] = field[T1][T2];
			if (field[T1][T2].piece.owner != playerNumber && !field[T1][T2].piece.visible && field[T1][T2].piece.owner != 0) {
				player1Field[T1][T2].piece.name = INVISIBLE_PIECE_NAME;
				player1Field[T1][T2].piece.value = INVISIBLE_PIECE_VALUE;
			}
		}
	}

	for (int T1 = 0; T1 < 10; T1++) {
		for (int T2 = 0; T2 < 10; T2++) {
			player2Field[T1][T2] = field[T1][T2];
			if (field[T1][T2].piece.owner != playerNumber && !field[T1][T2].piece.visible && field[T1][T2].piece.owner != 0) {
				player2Field[T1][T2].piece.name = INVISIBLE_PIECE_NAME;
				player2Field[T1][T2].piece.value = INVISIBLE_PIECE_VALUE;
			}
		}
	}

	if (playerNumber == 2) {
		// if player 2, turn the field upside down so the AI is always on top.
		Piece temp;
		for (int T3 = 0; T3 < 5; T3++) {
			for (int T4 = 0; T4 < 10; T4++) {
				temp = player2Field[T3][T4].piece;
				player2Field[T3][T4].piece = player2Field[9 - T3][9 - T4].piece;
				player2Field[9 - T3][9 - T4].piece = temp;
			}
		}
	}
}

void Board::print(const Tile board[10][10]) {
	std::cout << termcolor::red << "Hello, Colorful World!" << std::endl;
	for (int T1 = 0; T1 < 10; T1++) {
		for (int T2 = 0; T2 < 10; T2++) {
			if (board[T1][T2].piece.name == EMPTY_PIECE_NAME) {
				printf(" %c", board[T1][T2].land);
			} else {
				printf(" %c", board[T1][T2].piece.name);
			}
		}
		printf("\n");
	}
}