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

#define MS_PER_UPDATE 4

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
void drawBoard(int);
int drawMovables();
void buildWall(int, int, int, int);
void drawSquare(int, int, int, int, int);
int initMap();
int killMap();
void processInput();
void step();
typedef struct {
	int row;
	int col;
	int old_row;
	int old_col;
	int direction;
	int turn_dir;
	int ticks_per_move;
	char avatar;
	double ticks_since_last_move;
} player;

player player1;
int** screen_map;
int input, max_x, max_y, row, col = 0;
double previous;
