#ifndef BASICFRAMEWORK_H
#define BASICFRAMEWORK_H

#include <termios.h>

#define MAX_ROWS 100
#define MAX_COLS 100

extern char buffer[MAX_ROWS][MAX_COLS];
extern int cursor_x, cursor_y;
extern int num_rows;
extern struct termios orig_termios;
extern char clipboard[MAX_COLS];

void disableRawMode(void);
void enableRawMode(void);
void display_help(void);
void refresh_screen(void);

#endif
