#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "basicFramework.h"

char buffer[MAX_ROWS][MAX_COLS];
int cursor_x = 0, cursor_y = 0;
int num_rows = 0;
struct termios orig_termios;
char clipboard[MAX_COLS];

void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disableRawMode);

	struct termios raw = orig_termios;
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag |= (CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;

	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void display_help() {
	const char *help = "Ctrl+T Delete Line | Ctrl+X Exit | Ctrl+T delete line | Ctrl+Y copy line | Ctrl+P paste line|";
	write(STDOUT_FILENO, "\r\n", 2);
	write(STDOUT_FILENO, "\x1b[7m", 4);
	write(STDOUT_FILENO, help, strlen(help));
	write(STDOUT_FILENO, "\x1b[K", 3);
	write(STDOUT_FILENO, "\x1b[m", 3);
}

void refresh_screen() {
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);

	for (int i = 0; i < num_rows; i++) {
		write(STDOUT_FILENO, buffer[i], strlen(buffer[i]));
		if (i < num_rows - 1) {
			write(STDOUT_FILENO, "\r\n", 2);
		}
	}

	display_help();

	char buf[32];
	snprintf(buf, sizeof(buf), "\x1b[%d;%dH", cursor_y + 1, cursor_x + 1);
	write(STDOUT_FILENO, buf, strlen(buf));
}
