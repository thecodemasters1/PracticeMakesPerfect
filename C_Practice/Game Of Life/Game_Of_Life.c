#include <stdio.h>
#include<string.h>
#include <time.h>


#define BOARD_HEIGHT 130
#define BOARD_LENGTH 50

/*time delay , looks good :) */
void waitFor(unsigned int secs) {
	time_t retTime = time(0) + secs;     // Get finishing time.
	while (time(0) < retTime);    // Loop until it arrives.
}

/*create a board , full with random numbers , eather 1 or 0*/
int init_board(int * board) {
	int i = 0;
	srand(time(NULL));

	for (; i < BOARD_LENGTH*BOARD_HEIGHT; i++) {
		*board = rand() % 2;
		board++;
	}
}

/* add to x going like the snake game */
int add_x(int i, int a) {
	i += a;
	while (i < 0) {
		i += BOARD_LENGTH;
	}
	while (i >= BOARD_LENGTH) {
		i -= BOARD_LENGTH;
	}
	return i;
}

/* add to y  */
int add_y(int i, int a) {
	i += a;
	while (i < 0) {
		i += BOARD_HEIGHT;
	}
	while (i >= BOARD_HEIGHT) {
		i -= BOARD_HEIGHT;
	}
	return i;
}

/* return the number of on cells next to us */
int next_to(int board[][BOARD_HEIGHT], int x, int y) {
	int	k, l, count;

	count = 0;
	/*going all around*/
	for (k = -1; k <= 1; k++) for (l = -1; l <= 1; l++) {
		/* lets not check not ourself */
		if (k != 0 || l != 0) {
			if (board[add_x(x, k)][add_y(y, l)]) count++;
		}
	}
	return count;
}

void print_board(int board[][BOARD_HEIGHT]) {
	system("cls");
	char string[(BOARD_HEIGHT*BOARD_LENGTH) + BOARD_HEIGHT] = "";
	int x, y;
	for (x = 0; x < BOARD_LENGTH; x++) {
		for (y = 0; y < BOARD_HEIGHT; y++) {
			strcat(string, (board[x][y]) ? "\x2" : " ");
		}
		strcat(string, "\n");
	}
	puts(string);

}

/* do one generation*/
void one_generation(int board[][BOARD_HEIGHT]) {
	int	x, y, a, newboard[BOARD_LENGTH][BOARD_HEIGHT];

	/* for each cell if it has more or less than 3 kill it */

	for (x = 0; x < BOARD_LENGTH; x++) {
		for (y = 0; y < BOARD_HEIGHT; y++) {
			a = next_to(board, x, y);
			if (a == 3) {
				newboard[x][y] = 1;
			}
			else if (a == 2) {
				newboard[x][y] = board[x][y];
			}
			else {
				newboard[x][y] = 0;
			}
		}
	}

	/* copy the new board back into the old board */

	for (x = 0; x < BOARD_LENGTH; x++) {
		for (y = 0; y < BOARD_HEIGHT; y++) {
			board[x][y] = newboard[x][y];
		}
	}
}

int main(int argc, char *argv[]) {
	system("title Game of Life");
	int board[BOARD_LENGTH][BOARD_HEIGHT];
	init_board(&board);
	for (int i = 0; i < 10000; i++) {
		print_board(board);
		one_generation(board);
		/*pause for 1 sec*/
		/*remove it if you want to see it working faster*/
		//waitFor(1);
	}


	return 0;
}