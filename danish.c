#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "danish.h"

void loadFile(Buffer *buffer, const char *filename) {

    FILE *file;
    char line[1024];
    Node *current;

    if (buffer == NULL || filename == NULL) {
        return;
    }

    file = fopen(filename, "r");

    if (file == NULL) {
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {

        size_t len = strlen(line);

        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        addLine(buffer, line);
    }

    fclose(file);

    /* TAMPILKAN ISI BUFFER */
    current = buffer->head;

    while (current != NULL) {
        printf("%s\n", current->info);
        current = current->next;
    }
}

void saveFile(Buffer *buffer, const char *filename) {

    FILE *file;
    Node *current;

    if (buffer == NULL || filename == NULL) {
        return;
    }

    file = fopen(filename, "w");

    if (file == NULL) {

        printf("Gagal menyimpan file: %s\n", filename);

        return;
    }

    current = buffer->head;

    while (current != NULL) {

        fprintf(file, "%s\n", current->info);

        current = current->next;
    }

    fclose(file);
}

int insertChar(Cursor *cursor, char **line, size_t *length, size_t *capacity, char c) {

    if (cursor->node != NULL) {
        size_t nodeLength = strlen(cursor->node->info);
        char *newInfo;

        if (cursor->column > nodeLength) {
            cursor->column = nodeLength;
        }

        newInfo = realloc(cursor->node->info, nodeLength + 2);
        if (newInfo == NULL) {
            return 0;
        }

        cursor->node->info = newInfo;
        memmove(&cursor->node->info[cursor->column + 1],
                &cursor->node->info[cursor->column],
                nodeLength - cursor->column + 1);
        cursor->node->info[cursor->column] = c;
        cursor->column++;

        return 1;
    }

    if (*length + 1 >= *capacity) {
        char *newLine;

        if (*capacity == 0) {
            *capacity = 16;
        } else {
            *capacity = *capacity * 2;
        }

        newLine = realloc(*line, *capacity);
        if (newLine == NULL) {
            return 0;
        }

        *line = newLine;
    }

    if (cursor->column > *length) {
        cursor->column = *length;
    }

    memmove(&(*line)[cursor->column + 1],
            &(*line)[cursor->column],
            *length - cursor->column + 1);
    (*line)[cursor->column] = c;
    (*length)++;
    cursor->column++;

    return 1;
}

void redrawScreen(Buffer *buffer, const char *line, Node *cursorNode, size_t cursorColumn) {

    Node *current;
    size_t row = 1;
    size_t cursorRow = 1;

    clearScreen();

    current = buffer->head;
    while (current != NULL) {

        if (current == cursorNode) {
            cursorRow = row;
        }

        printf("%s\r\n", current->info);
        current = current->next;
        row++;
    }

    if (line != NULL) {
        printf("%s", line);

        if (cursorNode == NULL) {
            cursorRow = row;
        }
    }

    printf("\x1b[%zu;%zuH", cursorRow, cursorColumn + 1);
    fflush(stdout);
}