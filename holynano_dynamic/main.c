
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "text_buffer.h"

int main() {
  TextBuffer buffer;
  char *input = NULL;
  size_t capacity = 0;

  initTextBuffer(&buffer);

  while (getline(&input, &capacity, stdin) != -1) {
    size_t length = strlen(input);

    if (length > 0 && input[length - 1] == '\n') {
      input[length - 1] = '\0';
    }

    if (!appendTextLine(&buffer, input)) {
      free(input);
      clearTextBuffer(&buffer);
      return 1;
    }
  }

  free(input);
  clearTextBuffer(&buffer);
  return 0;
}
