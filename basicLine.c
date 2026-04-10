#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "basicLine.h"

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
  if (clipboard[0] == '\0') return;

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

void startSelection() {
  select_start = cursor_y;
  select_end = -1;
  clipboard_lines = 0;
}

void endSelection() {
  if (select_start < 0) select_start = cursor_y;
  select_end = cursor_y;
  if (select_start > select_end) {
    int temp = select_start;
    select_start = select_end;
    select_end = temp;
  }
  clipboard_lines = 0;
  for (int i = select_start; i <= select_end && clipboard_lines < MAX_ROWS; i++) {
    strncpy(clipboard_block[clipboard_lines], buffer[i], MAX_COLS - 1);
    clipboard_block[clipboard_lines][MAX_COLS - 1] = '\0';
    clipboard_lines++;
  }
}

void pasteSelection() {
  if (clipboard_lines == 0) return;
  if (num_rows + clipboard_lines > MAX_ROWS) return; 
  for (int i = num_rows - 1; i >= cursor_y + 1; i--) {
    strncpy(buffer[i + clipboard_lines], buffer[i], MAX_COLS - 1);
    buffer[i + clipboard_lines][MAX_COLS - 1] = '\0';
  }
  for (int i = 0; i < clipboard_lines; i++) {
    strncpy(buffer[cursor_y + 1 + i], clipboard_block[i], MAX_COLS - 1);
    buffer[cursor_y + 1 + i][MAX_COLS - 1] = '\0';
  }
  num_rows += clipboard_lines;
  cursor_y += clipboard_lines;
  cursor_x = strlen(buffer[cursor_y]);
}

void run_editor_loop(void) {
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
     else if (c == 20) {  // Ctrl+T (delete line)
      deleteLine();
    }
     else if (c == 25) {  // Ctrl+Y (copy line)
      copyLine();
    }
     else if (c == 16) {  // Ctrl+P (paste line)
      pasteLine();
    }
    else if (c == 2) {   // Ctrl+B (start selection)
      startSelection();
    }
    else if (c == 5) {   // Ctrl+E (end selection)
      endSelection();
    }
    else if (c == 15) {  // Ctrl+O (paste selection)
      pasteSelection();
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
}
