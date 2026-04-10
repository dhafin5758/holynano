#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>

#include "basicFramework.h"
#include "manageFile.h"

char buffer[MAX_ROWS][MAX_COLS];
int cursor_x = 0, cursor_y = 0;
int num_rows = 0;
struct termios orig_termios;
char clipboard[MAX_COLS];
char status_message[256];

void setStatusMessage(const char *message) {
	if (message == NULL) {
		status_message[0] = '\0';
		return;
	}

	strncpy(status_message, message, sizeof(status_message) - 1);
	status_message[sizeof(status_message) - 1] = '\0';
}

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

int getCurrentLine(void) {
	return cursor_y + 1;
}

int getCurrentColumn(void) {
	return cursor_x + 1;
}

void display_help() {
	const char *help = isViewOnlyMode()
		? "VIEW ONLY | Ctrl+X Exit|  "
		: "Ctrl+S Save | Ctrl+T Delete Line | Ctrl+X Exit | Ctrl+P paste line | Ctrl+Y copy line| | Ctrl+K cut line|";
	const char *text = status_message[0] != '\0' ? status_message : help;
	char info_line[64];
	char pos[32];
	struct winsize ws;
	int rows = 24;
	int cols = 80;
	int help_len = strlen(text);
	int info_len;
	int help_row;
	int info_row;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0) {
		if (ws.ws_row > 0) rows = ws.ws_row;
		if (ws.ws_col > 0) cols = ws.ws_col;
	}

	help_row = rows - 1;
	info_row = rows;
	if (help_row < 1) help_row = 1;
	if (info_row < 1) info_row = 1;

	snprintf(info_line, sizeof(info_line), "Ln %d, Col %d", getCurrentLine(), getCurrentColumn());
	info_len = strlen(info_line);
	if (help_len > cols) help_len = cols;
	if (info_len > cols) info_len = cols;

	snprintf(pos, sizeof(pos), "\x1b[%d;1H", help_row);
	write(STDOUT_FILENO, pos, strlen(pos));
	write(STDOUT_FILENO, "\x1b[7m", 4);
	write(STDOUT_FILENO, text, help_len);
	write(STDOUT_FILENO, "\x1b[K", 3);
	write(STDOUT_FILENO, "\x1b[m", 3);

	snprintf(pos, sizeof(pos), "\x1b[%d;1H", info_row);
	write(STDOUT_FILENO, pos, strlen(pos));
	write(STDOUT_FILENO, "\x1b[7m", 4);
	write(STDOUT_FILENO, info_line, info_len);
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
