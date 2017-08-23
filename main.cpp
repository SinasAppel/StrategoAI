#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "objects.h"
using namespace std;

/**
 * Prints the current board
 */
void printField(Tile field[10][10]) {
	for(int i=0; i < 10; i++) {
		for(int j=0; j < 10; j++) {
			printf(" %c", field[i][j].getName());
		}
		printf("\n");
	}
}

/**
 * Creates the initial (empty) board
 */
void createBoard(Tile field[10][10]) {
	for(int i=0; i < 10; i++) {
		for(int j=0; j < 10; j++) {
			if ((i==4 || i==5) && (j==2 || j==3 || j==6 || j==7)) {
				Water w;
				field[i][j] = w;
			} else {
				Grass g;
				field[i][j] = g;
			}
		}
	}
}

int main() {
	Tile field[10][10];
	createBoard(field);
	printField(field);
	return 0;
}
