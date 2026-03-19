#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#define MAX_ROWS 100
#define MAX_COLS 100

char buffer[MAX_ROWS][MAX_COLS];
int cursor_x = 0, cursor_y = 0;
int num_rows = 0;
struct termios orig_termios;

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

void refresh_screen() {
  write(STDOUT_FILENO, "\x1b[2J", 4);    // clear screen
  write(STDOUT_FILENO, "\x1b[H", 3);     // cursor home
  
  // draw each row
  for (int i = 0; i < num_rows; i++) {
    write(STDOUT_FILENO, buffer[i], strlen(buffer[i]));
    if (i < num_rows - 1) {
      write(STDOUT_FILENO, "\r\n", 2);
    }
  }
  
  // place cursor
  char buf[32];
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", cursor_y + 1, cursor_x + 1);
  write(STDOUT_FILENO, buf, strlen(buf));
}
int main() {
  enableRawMode();
  
  // start with one empty line
  buffer[0][0] = '\0';
  num_rows = 1;
  
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1) {
    // Arrow keys are: ESC [ A/B/C/D
    if (c == '\x1b') {
      read(STDIN_FILENO, &c, 1);  // read [
      read(STDIN_FILENO, &c, 1);  // read A/B/C/D
      
      if (c == 'A' && cursor_y > 0) cursor_y--;           // up
      else if (c == 'B' && cursor_y < num_rows - 1) cursor_y++;  // down
      else if (c == 'C') cursor_x++;                       // right
      else if (c == 'D' && cursor_x > 0) cursor_x--;       // left
    }
    else if (c == 24) {  // Ctrl+X
      write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7);
      break;
    }
    else if (c >= 32 && c < 127) {  // printable char
      int len = strlen(buffer[cursor_y]);
      if (len < MAX_COLS - 1) {
        buffer[cursor_y][len] = c;
        buffer[cursor_y][len + 1] = '\0';
        cursor_x++;
      }
    }
    else if (c == '\r' || c == '\n') {  // Enter in raw mode is usually '\r'
      if (num_rows < MAX_ROWS - 1) {
        buffer[num_rows][0] = '\0';
        num_rows++;
        cursor_y++;
        cursor_x = 0;
      }
    }
    else if (c == 127) {  // Backspace
      if (cursor_x > 0) {
        int len = strlen(buffer[cursor_y]);
        if (cursor_x <= len) {
          memmove(&buffer[cursor_y][cursor_x - 1], 
                  &buffer[cursor_y][cursor_x], len - cursor_x + 1);
          cursor_x--;
        }
      }
    }
    
    refresh_screen();
  }
  
  return 0;
}