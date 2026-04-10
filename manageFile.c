#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "basicFramework.h"
#include "manageFile.h"

#define MAX_FILE_PATH 512

static char activeFilePath[MAX_FILE_PATH];

static void setActiveFilePath(const char *filename) {
  if (filename == NULL) {
    activeFilePath[0] = '\0';
    return;
  }

  strncpy(activeFilePath, filename, MAX_FILE_PATH - 1);
  activeFilePath[MAX_FILE_PATH - 1] = '\0';
}

static int requestFileNameForSave(void) {
  char inputPath[MAX_FILE_PATH];
  int len = 0;
  char c;
  char prompt[256];

  inputPath[0] = '\0';

  setStatusMessage("Save as:  (Ctrl+C cancel)");
  refresh_screen();

  while (read(STDIN_FILENO, &c, 1) == 1) {
    if (c == '\r' || c == '\n') {
      break;
    }

    if (c == 3) {
      setStatusMessage("");
      return 0;
    }

    if (c == 127 || c == 8) {
      if (len > 0) {
        len--;
        inputPath[len] = '\0';
      }
    } else if (c >= 32 && c < 127 && len < MAX_FILE_PATH - 1) {
      inputPath[len++] = c;
      inputPath[len] = '\0';
    }

    snprintf(prompt, sizeof(prompt), "Save as: %s  (Ctrl+C cancel)", inputPath);
    setStatusMessage(prompt);
    refresh_screen();
  }

  if (len == 0) {
    setStatusMessage("");
    return 0;
  }

  setActiveFilePath(inputPath);
  setStatusMessage("");
  return 1;
}

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

  setActiveFilePath(filename);

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

void saveFile(void) {
  if (activeFilePath[0] == '\0') {
    if (!requestFileNameForSave()) {
      return;
    }
  }

  FILE *file = fopen(activeFilePath, "w");
  if (file == NULL) {
    setStatusMessage("");
    return;
  }

  for (int i = 0; i < num_rows; i++) {
    fputs(buffer[i], file);
    if (i < num_rows - 1) {
      fputc('\n', file);
    }
  }

  fclose(file);
  setStatusMessage("");
}
