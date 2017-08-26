#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "generating.h"
using namespace std;

/**
* Prints the current board
*/
void printField(Tile field[10][10]) {
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (field[i][j].piece.value < 0 || field[i][j].piece.value > 10) {
				printf(" %c", field[i][j].land);
			}
			else {
				printf(" %c", field[i][j].piece.name);
			}
		}
		printf("\n");
	}
}

int main() {
	Tile field[10][10];
	createBoard(field);
	fillBoard(field);
	printField(field);
	getchar();
	getchar();
	return 0;
}
