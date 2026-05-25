#include <stdlib.h>
#include <unistd.h>

#include "dhafin.h"
#include "zaky.h"

int main(void) {
  Buffer buffer;
  char *line = NULL;
  size_t length = 0;
  size_t capacity = 0;
  char c;

  initBuffer(&buffer);

  if (!enableRawMode()) {
    return 1;
  }

  while (read(STDIN_FILENO, &c, 1) == 1) {
    if (c == '\r' || c == '\n') {
      if (line == NULL) {
        addLine(&buffer, "");
      } else {
        line[length] = '\0';
        addLine(&buffer, line);
      }

      length = 0;
      write(STDOUT_FILENO, "\r\n", 2);
    } else if (c >= 32 && c < 127) {
      if (length + 1 >= capacity) {
        char *newLine;

        if (capacity == 0) {
          capacity = 16;
        } else {
          capacity = capacity * 2;
        }

        newLine = realloc(line, capacity);
        if (newLine == NULL) {
          free(line);
          clearBuffer(&buffer);
          return 1;
        }

        line = newLine;
      }

      line[length] = c;
      length++;
      line[length] = '\0';
      write(STDOUT_FILENO, &c, 1);
    }
  }

  if (length > 0) {
    line[length] = '\0';
    addLine(&buffer, line);
  }

  free(line);
  clearBuffer(&buffer);
  return 0;
}
