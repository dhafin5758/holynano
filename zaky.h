#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "dhafin.h"

typedef struct Cursor {
    Node *node;
    size_t column;
} Cursor;

void initCursor(Cursor *cursor, Buffer *buffer);
void setCursor(Cursor *cursor, Node *node) ;

void moveCursorUp(Cursor *cursor);
void moveCursorDown(Cursor *cursor);
void moveCursorLeft(Cursor *cursor);
void moveCursorRight(Cursor *cursor);

void deleteLine(Buffer *buffer, Cursor *cursor);

void backspaceChar(Node *cursorNode, int *colIndex);