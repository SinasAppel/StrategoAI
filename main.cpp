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
	for(int i=0; i < 10; i++) {
		for(int j=0; j < 10; j++) {
			printf(" %c", field[i][j].land;
		}
		printf("\n");
	}
}




int main() {
	Tile field[10][10];
	createBoard(field);
	printField(field);
	return 0;
}
