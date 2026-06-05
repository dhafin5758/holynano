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
    Cursor cursor;

    char *filename = "output.txt";

    initBuffer(&buffer);

    /* LOAD FILE DARI TERMINAL */
    if (argc > 1) {
        filename = argv[1];
        loadFile(&buffer, filename);
    }

    initCursor(&cursor, &buffer);

    /* AKTIFKAN RAW MODE */
    if (!enableRawMode()) {
        return 1;
    }
    clearScreen();

    /* INPUT EDITOR */
    while (read(STDIN_FILENO, &c, 1) == 1) {

        /* ARROW KEY: buang escape sequence agar tidak tercetak sebagai [A/[B/[C/[D */
        if (c == '\x1b') {  
            char seq[2];

            read(STDIN_FILENO, &seq[0], 1);
            read(STDIN_FILENO, &seq[1], 1);

            if (seq[0] == '[') {

                if (seq[1] == 'A') {
                    if (cursor.node == NULL && buffer.tail != NULL) {
                        cursor.node = buffer.tail;

                        if (cursor.column > strlen(cursor.node->info)) {
                            cursor.column = strlen(cursor.node->info);
                        }
                    } else {
                    moveCursorUp(&cursorNode, &colIndex);
                }

                else if (seq[1] == 'B') {
                    if (cursor.node != NULL && cursor.node->next == NULL) {
                        cursor.node = NULL;

                        if (cursor.column > length) {
                            cursor.column = length;
                        }
                    } else {
                    moveCursorDown(&cursorNode, &colIndex);
                }

                else if (seq[1] == 'C') {
                    if (cursor.node == NULL && cursor.column < length) {
                        cursor.column++;
                    } else if (cursor.node != NULL &&
                               cursor.node->next == NULL &&
                               cursor.column >= strlen(cursor.node->info)) {
                        cursor.node = NULL;

                        if (cursor.column > length) {
                            cursor.column = length;
                        }
                    } else {
                    moveCursorRight(cursorNode, &colIndex);
                }

                else if (seq[1] == 'D') {
                    if (cursor.node == NULL && cursor.column > 0) {
                        cursor.column--;
                    } else if (cursor.node == NULL && buffer.tail != NULL) {
                        cursor.node = buffer.tail;
                        cursor.column = strlen(cursor.node->info);
                    } else {
                    moveCursorLeft(cursorNode, &colIndex);
                }
            }

            redrawScreen(&buffer, line, cursor.node, cursor.column);

            continue;
        }

        if (c == 20) {
            deleteLine(&buffer, &cursor);
            redrawScreen(&buffer,line,cursor.node,cursor.column);
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
        
        if (c == 127 || c == 8) {
            backspaceChar(&cursor, line, &length);
            redrawScreen(&buffer, line, cursor.node, cursor.column);
            continue;
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

        else if (c >= 32 && c < 127) {  //A-Z, a-z, angka, spasi, tanda baca
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
