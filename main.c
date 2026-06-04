#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dhafin.h"
#include "danish.h"
#include "zaky.h"

int main(int argc, char *argv[]) {

    Buffer buffer;
    char *line = NULL;
    size_t length = 0;
    size_t capacity = 0;
    char c;

    char *filename = "output.txt";

    initBuffer(&buffer);

    /* LOAD FILE DARI TERMINAL */
    if (argc > 1) {
        filename = argv[1];
        loadFile(&buffer, filename);
    }

    /* AKTIFKAN RAW MODE */
    if (!enableRawMode()) {
        return 1;
    }

    /* INPUT EDITOR */
    while (read(STDIN_FILENO, &c, 1) == 1) {

        /* CTRL + S */
        if (c == 19) {
            if (length > 0) {
                line[length] = '\0';
                addLine(&buffer, line);
                saveFile(&buffer, filename);
                deleteNode(&buffer, buffer.tail);
            } else {
                saveFile(&buffer, filename);
            }
            continue;
        }

        /* CTRL + X */
        if (c == 24) {
        break;
        }

        /* ENTER */
        if (c == '\r' || c == '\n') {
            if (line == NULL) {
                addLine(&buffer, "");
            } else {
                line[length] = '\0';
                addLine(&buffer, line);
            }

            length = 0;
            write(STDOUT_FILENO, "\r\n", 2);
        }

        else if (c >= 32 && c < 127) {
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