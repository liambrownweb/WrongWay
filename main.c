#include "term_width.h"
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

/* Define cell types */
#define EMPTY 0
#define WALL 1
#define TURN 2
#define CAR 3
#define SPEEDUP 4
#define SLOWDOWN 5

/* Define directions */
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3

/* Define turns */
#define CW 0
#define CCW 1

/* for main.h */
void addTurn(int, int);
int main();
int moveChar();
int drawMovables();
void buildWall(int, int, int, int);
void drawSquare(int, int, int, int, int);
int initMap();
int killMap();
void step();
typedef struct {
	int row;
	int col;
	int old_row;
	int old_col;
	int direction;
	int turn_dir;
	int velocity;
	char avatar;
} player;

player player1;
int** screen_map;
int input, max_x, max_y, row, col = 0;
int checkCell(int c_row, int c_col) {
	return screen_map[c_row][c_col];
}

void addTurn(int row, int col) {
	screen_map[row][col] = TURN;
}

/* Creates an invisible wall on the screen map; cursor then can't pass through it.
 * Note: this does NOT check that your wall is within limits. It will segfault if
 * the wall goes out of range.
 **/
void buildWall(int orig_y, int orig_x, int length, int direction){
	int i, w_row, w_col;
	for (i = 0; i < length; i++) {
		if (direction == UP) {
			w_row = orig_y - i;
			w_col = orig_x;
		} else if (direction == DOWN) {
			w_row = orig_y + i;
			w_col = orig_x;
		} else if (direction == LEFT) {
			w_row = orig_y;
			w_col = orig_x - i;
		} else if (direction == RIGHT) {
			w_row = orig_y;
			w_col = orig_x + i;
		}
		screen_map[w_row][w_col] = WALL;
	}
}
void drawSquare(int orig_x, int orig_y, int width, int height, int line_type) {
	mvhline(orig_y, orig_x, 0, width-orig_x);
	buildWall(orig_y, orig_x, width-orig_x, RIGHT);
    mvhline(height, orig_x, 0, width-orig_x);
	buildWall(height, orig_x, width-orig_x, RIGHT);
    mvvline(orig_y, orig_x, 0, height-orig_y);
	buildWall(orig_y, orig_x, height-orig_y, DOWN);
    mvvline(orig_y, width, 0, height-orig_y);
	buildWall(orig_y, width, height-orig_y, DOWN);
    mvaddch(orig_y, orig_x, ACS_ULCORNER);
    mvaddch(height, orig_x, ACS_LLCORNER);
    mvaddch(orig_y, width, ACS_URCORNER);
    mvaddch(height, width, ACS_LRCORNER);	
	addTurn(orig_y + 1, orig_x + 1);
	addTurn(height - 1, orig_x + 1);
	addTurn(height - 1, width - 1);
	addTurn(orig_y + 1, width - 1);
}
int initMap(){
	int i, j;
	screen_map = (int**) malloc(sizeof(int*) * max_y);
	for (i = 0; i < max_y; i++) {
		screen_map[i] = (int*) malloc(sizeof(int) * max_x);
		for (j = 0; j < max_x; j++) {
			screen_map[i][j] = EMPTY;
		}
	}
	return 0;
}
int killMap(){
	int i;
	for (i = 0; i < max_y; i++){
		free(screen_map[i]);
	}
	free(screen_map);
	return 0;
}
int main() {
	struct winsize width = term_width();
	initscr();			/* Start curses mode 		  */
	curs_set(0);
	getmaxyx(stdscr, max_y, max_x);
	initMap();
	raw();				/* Disable line buffering */
	keypad(stdscr, TRUE); /* Get interactive functionality like function and arrow keys */
	row = max_y >> 1;
	col = max_x >> 1;
	mvaddch(row, col, '*');
	drawSquare(30, 5, max_x - 31, max_y - 5, 0);
	drawSquare(0, 0, max_x - 1, max_y - 1, 0);
	player1.row = max_y - 2;
	player1.col = max_x >> 1;
	player1.direction = RIGHT;
	player1.velocity = 1;
	player1.avatar = '%';
	player1.turn_dir = CCW;
	refresh();			/* Print it on to the real screen */
	while (input != 3) {
		input = getch();			/* Wait for user input */
		player1.old_col = player1.col;
		player1.old_row = player1.row;
		if (input == KEY_LEFT && player1.col > 1 && checkCell(player1.row, player1.col-1) != WALL) {
			player1.col -= 1;
		} else if (input == KEY_RIGHT && player1.col < max_x - 1 && checkCell(player1.row, player1.col+1) != WALL) {
			player1.col += 1;
		} else if (input == KEY_UP && player1.row > 1 && checkCell(player1.row-1, player1.col) != WALL) {
			player1.row -= 1;
		} else if (input == KEY_DOWN && player1.row < max_y - 1 && checkCell(player1.row+1, player1.col) != WALL) {
			player1.row += 1;
		} else step();
		drawMovables();
		refresh();
	}
	endwin();			/* End curses mode		  */
	killMap();
	curs_set(2);
	return 0;
}
int drawMovables(){
	mvaddch(player1.old_row, player1.old_col, ' ');
	mvaddch(player1.row, player1.col, player1.avatar);
	refresh();
}

int moveChar(player* current) {
	current->old_col = current->col;
	current->old_row = current->row;
	if (checkCell(current->row, current->col) == TURN) {
		turnChar(current);
	}
	if (current->direction == LEFT && current->col > 1 && checkCell(current->row, current->col-1) != WALL) {
		current->col -= 1;
	} else if (current->direction == RIGHT && current->col < max_x - 1 && checkCell(current->row, current->col+1) != WALL) {
		current->col += 1;
	} else if (current->direction == UP && current->row > 1 && checkCell(current->row-1, current->col) != WALL) {
		current->row -= 1;
	} else if (current->direction == DOWN && current->row < max_y - 1 && checkCell(current->row+1, current->col) != WALL) {
		current->row += 1;
	}

}

int turnChar (player* current) {
	if (current->turn_dir == CW) {
		if (current->direction == LEFT) {
			current->direction = UP;
		} else {
			current->direction += 1;
		}
	} else {
		if (current->direction == UP) {
			current->direction = LEFT;
		} else {
			current->direction -= 1;
		}
	}
	return 0;
}

void step() {
	moveChar(&player1);
}
