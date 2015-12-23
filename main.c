/**
 * WrongWay, Copyright 2015 Liam Brown <liambrownweb@gmail.com>.
 *
 * This file is part of WrongWay.
 * 
 * WrongWay is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * WrongWay is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with WrongWay.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "term_width.h"
#include "main.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int checkCell(int c_row, int c_col) {
	return screen_map[c_row][c_col];
}

void addNPC() {
	player* new_npc = malloc(sizeof(player));
	int new_coords[2], row, col;
	findEmpty(new_coords);
	row = new_coords[0];
	col = new_coords[1];
	new_npc->row = row;
	new_npc->col = col;
	new_npc->direction = RIGHT;
	new_npc->ticks_per_move = 30;
	new_npc->ticks_since_last_move = 0;
	new_npc->avatar = '@';
	new_npc->turn_dir = CW;
	npcs[npc_count++] = *new_npc;
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

void clearSquare(int orig_x, int orig_y, int width, int height) {
	int x, y, row, col = 0;
	for (y = 0; y < height; y += 1) {
		for (x = 0; x < width; x += 1) {
			col = x + orig_x;
			row = y + orig_y;
			mvaddch(row, col, ' ');
			screen_map[row][col] = EMPTY;
		}
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

void findEmpty(int* row_col) {
	int row = 1;
	int col = 10;
	while (checkCell(row, col) != EMPTY) {
		//generate new row and column
		row = 1;
		col = 10;
	}
	row_col[0] = row;
	row_col[1] = col;
}

int initMap(){
	int i, j;
	npc_count = 0;
	screen_map = (int**) malloc(sizeof(int*) * max_y);
	for (i = 0; i < max_y; i++) {
		screen_map[i] = (int*) malloc(sizeof(int) * max_x);
		for (j = 0; j < max_x; j++) {
			screen_map[i][j] = EMPTY;
		}
	}
	npcs = (player*) malloc(sizeof(player*) * 10);
	addNPC();
	return 0;
}

int killMap(){
	int i;
	for (i = 0; i < max_y; i++){
		free(screen_map[i]);
	}
	free(screen_map);
	free(npcs);
	return 0;
}

void drawBoard(int lanes) {
	int i = 0;
	drawSquare(0, 0, max_x - 1, max_y - 1, 0);
	for (; i < lanes; i += 1) {
		drawSquare(i << 1, i << 1, max_x - (1 + (i << 1)), max_y - (1 + (i << 1)), 0);
	}
	clearSquare(1, max_y / 3, (i<<1)-4, max_y / 3);
	clearSquare(max_x + 2 - (i << 1), max_y / 3, (i << 1) - 4, max_y / 3);
}

int main() {
	struct winsize width = term_width();
	initscr();			/* Start curses mode 		  */
	curs_set(0);
	noecho();
	cbreak();
	timeout(0);
	getmaxyx(stdscr, max_y, max_x);
	if (max_y < 40 || max_x < 40) {
		endwin();
		printf("Console dimensions are too small to run this game!");
	} else {
		initMap();
		keypad(stdscr, TRUE); /* Get interactive functionality like function and arrow keys */
		row = max_y >> 1;
		col = max_x >> 1;
		drawBoard(6);
		player1.row = max_y - 2;
		player1.col = max_x >> 1;
		player1.direction = RIGHT;
		player1.ticks_per_move = 30;
		player1.ticks_since_last_move = 0;
		player1.avatar = '%';
		player1.turn_dir = CCW;
		refresh();			/* Print it on to the real screen */
		previous = clock();
		clock_t current;
		float lag = 0.0;
		while (true && input != 3 && input != 'q') {
			previous = current;
			current = clock();
			float elapsed = ((current - previous) * 1000) / CLOCKS_PER_SEC; 
			lag += elapsed;
			if ((input = getch()) != ERR){
				processInput(input);
			}

			while ((int)lag >= MS_PER_UPDATE)
			{
				step();
				lag = 0;
			}
			drawMovables();
		}
		killMap();
		endwin();			/* End curses mode		  */
	}
	curs_set(2);
	echo();
	return 0;
}
void processInput(int my_input) {
	player1.old_col = player1.col;
	player1.old_row = player1.row;
	if (my_input == KEY_LEFT && player1.col > 1 && checkCell(player1.row, player1.col-1) != WALL) {
		player1.col -= 1;
	} else if (my_input == KEY_RIGHT && player1.col < max_x - 1 && checkCell(player1.row, player1.col+1) != WALL) {
		player1.col += 1;
	} else if (my_input == KEY_UP && player1.row > 1 && checkCell(player1.row-1, player1.col) != WALL) {
		player1.row -= 1;
	} else if (my_input == KEY_DOWN && player1.row < max_y - 1 && checkCell(player1.row+1, player1.col) != WALL) {
		player1.row += 1;
	}
}
int drawMovables(){
	int i;
	mvaddch(player1.old_row, player1.old_col, ' ');
	mvaddch(player1.row, player1.col, player1.avatar);
	for (i = 0; i < npc_count; i++) {
		mvaddch(npcs[i].old_row, npcs[i].old_col, ' ');
		mvaddch(npcs[i].row, npcs[i].col, npcs[i].avatar);
	}
	refresh();
	return 0;
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
	int i;
	player1.ticks_since_last_move += 1;
	if (player1.ticks_since_last_move >= player1.ticks_per_move){
		moveChar(&player1);
		player1.ticks_since_last_move = 0;
	}
	for (i = 0; i < npc_count; i++){
		npcs[i].ticks_since_last_move += 1;
		if (npcs[i].ticks_since_last_move >= npcs[i].ticks_per_move){
			moveChar(&npcs[i]);
			npcs[i].ticks_since_last_move = 0;
		}
	}
}
