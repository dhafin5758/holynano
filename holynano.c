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
  const char *help = "Ctrl+T Delete Line | Ctrl+X Exit";
  write(STDOUT_FILENO, "\r\n", 2);
  write(STDOUT_FILENO, "\x1b[7m", 4);
  write(STDOUT_FILENO, help, strlen(help));
  write(STDOUT_FILENO, "\x1b[K", 3);
  write(STDOUT_FILENO, "\x1b[m", 3);
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

  display_help();

  
  // place cursor
  char buf[32];
  snprintf(buf, sizeof(buf), "\x1b[%d;%dH", cursor_y + 1, cursor_x + 1);
  write(STDOUT_FILENO, buf, strlen(buf));
} 

void deleteLine() {
  if (num_rows == 0) return;


  if (num_rows == 1) {
    buffer[0][0] = '\0';
    cursor_x = 0;
    cursor_y = 0;
    return;
  }


  for (int i = cursor_y; i < num_rows - 1; i++) {
    strcpy(buffer[i], buffer[i + 1]);
  }


  num_rows--;
  buffer[num_rows][0] = '\0';


  if (cursor_y >= num_rows) cursor_y = num_rows - 1;


  int len = strlen(buffer[cursor_y]);
  if (cursor_x > len) cursor_x = len;
}

void copyLine() {
  if (num_rows == 0) {
    clipboard[0] = '\0';
    return;
  }
  strncpy(clipboard, buffer[cursor_y], MAX_COLS - 1);
  clipboard[MAX_COLS - 1] = '\0';
}

void pasteLine() {
  if (num_rows >= MAX_ROWS) return;
  if (clipboard[0] == '\0') return; // nothing to paste


  for (int i = num_rows; i > cursor_y + 1; i--) {
    strncpy(buffer[i], buffer[i - 1], MAX_COLS - 1);
    buffer[i][MAX_COLS - 1] = '\0';
  }


  strncpy(buffer[cursor_y + 1], clipboard, MAX_COLS - 1);
  buffer[cursor_y + 1][MAX_COLS - 1] = '\0';
  num_rows++;


  cursor_y++;
  cursor_x = strlen(buffer[cursor_y]);
}

int main() {
  enableRawMode();
  
  // start with one empty line
  buffer[0][0] = '\0';
  num_rows = 1;
  refresh_screen();
  
  char c;
  while (read(STDIN_FILENO, &c, 1) == 1) {
    // Arrow keys are: ESC [ A/B/C/D
    if (c == '\x1b') {
      read(STDIN_FILENO, &c, 1);  // read [
      read(STDIN_FILENO, &c, 1);  // read A/B/C/D
      
     if (c == 'A' && cursor_y > 0) {                      // up
        cursor_y--;
        int len = strlen(buffer[cursor_y]);
        if (cursor_x > len) cursor_x = len;
      }
      else if (c == 'B' && cursor_y < num_rows - 1) {      // down
        cursor_y++;
        int len = strlen(buffer[cursor_y]);
        if (cursor_x > len) cursor_x = len;
      }
      else if (c == 'C') {                                  // right
        int len = strlen(buffer[cursor_y]);
        if (cursor_x < len) cursor_x++;
      }
      else if (c == 'D' && cursor_x > 0) cursor_x--;       // left
    }
    else if (c == 24) {  // Ctrl+X
      write(STDOUT_FILENO, "\x1b[2J\x1b[H", 7);
      break;
    }
     else if (c == 20) {  // Ctrl+k (delete line)
      deleteLine();
    }
     else if (c == 25) {  // Ctrl+Y (copy line)
      copyLine();
      
    }
     else if (c == 16) {  // Ctrl+P (paste line)
      pasteLine();
    }
    else if (c >= 32 && c < 127) {  // printable char
      int len = strlen(buffer[cursor_y]);
      if (len < MAX_COLS - 1) {
        if (cursor_x > len) cursor_x = len;
        memmove(&buffer[cursor_y][cursor_x + 1],
                &buffer[cursor_y][cursor_x],
                len - cursor_x + 1);
        buffer[cursor_y][cursor_x] = c;
        cursor_x++;
      }
    }
    else if (c == '\r' || c == '\n') {  // Enter in raw mode is usually '\r'
      if (num_rows < MAX_ROWS - 1) {
        int len = strlen(buffer[cursor_y]);
        if (cursor_x > len) cursor_x = len;

        memmove(buffer[cursor_y + 1], &buffer[cursor_y][cursor_x], len - cursor_x + 1);
        buffer[cursor_y][cursor_x] = '\0';
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