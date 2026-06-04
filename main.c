<<<<<<< HEAD:holynano_dynamic/main.c
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
    
    if (buffer.head == NULL) {
        addLine(&buffer, "");
    }
    
    Node *cursor = buffer.head;  
    int colIndex = 0;
    
    /* AKTIFKAN RAW MODE */
    if (!enableRawMode()) {
        return 1;
    }

    refreshScreen(&buffer, cursor, colIndex);

    /* INPUT EDITOR */
    while (read(STDIN_FILENO, &c, 1) == 1) {

        if (c == '\x1b') {
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) == 1 && read(STDIN_FILENO, &seq[1], 1) == 1) {
                if (seq[0] == '[') {
                    switch (seq[1]) {
                      case 'A': 
                        moveCursorUp(&cursor, &colIndex);
                      break;
                      case 'B': 
                        moveCursorDown(&cursor, &colIndex);
                      break;
                      case 'C': 
                        moveCursorRight(cursor, &colIndex);
                      break;
                      case 'D': 
                        moveCursorLeft(cursor, &colIndex);
                      break;
                    }
                }
            }
            refreshScreen(&buffer, cursor, colIndex);
            continue;
        }

        if (c == 20) {
            deleteLine(&buffer, &cursor);
            colIndex = 0;
            continue;
        }
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
=======
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
>>>>>>> 13c565e013847c4452ef284270449744a9fc6b0d:main.c
