#include <stdio.h>
#include <string.h>

#include "basicFramework.h"
#include "manageFile.h"

static void initEmptyBuffer(void) {
  buffer[0][0] = '\0';
  num_rows = 1;
  cursor_x = 0;
  cursor_y = 0;
}

void createEmptyFile(const char *filename) {
  FILE *file = fopen(filename, "a");
  if (file == NULL) {
    fprintf(stderr, "Failed to create file: %s\n", filename);
    return;
  }

  fclose(file);
}

void loadFile(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    fprintf(stderr, "Failed to load file: %s\n", filename);
    initEmptyBuffer();
    return;
  }

  num_rows = 0;

  char line[MAX_COLS];
  while (num_rows < MAX_ROWS && fgets(line, sizeof(line), file) != NULL) {
    size_t len = strlen(line);

    while (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r')) {
      line[--len] = '\0';
    }

    strncpy(buffer[num_rows], line, MAX_COLS - 1);
    buffer[num_rows][MAX_COLS - 1] = '\0';
    num_rows++;
  }

  fclose(file);

  if (num_rows == 0) {
    buffer[0][0] = '\0';
    num_rows = 1;
  }

  cursor_x = 0;
  cursor_y = 0;
}

void openOrCreateFile(const char *filename) {
  createEmptyFile(filename);
  loadFile(filename);
}
