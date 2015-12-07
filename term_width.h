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
