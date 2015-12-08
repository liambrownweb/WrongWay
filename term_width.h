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

#ifndef TERM_WIDTH
#define TERM_WIDTH
#include <sys/ioctl.h>
#include <unistd.h>
struct winsize term_width() {
	int output = 0;
	struct winsize w;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);	
	return w;
}
#endif
