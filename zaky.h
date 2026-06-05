#ifndef ZAKY_H
#define ZAKY_H

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

void deleteLine(Buffer *buffer, Cursor *cursor, char *line, size_t *length);

void backspaceChar(Cursor *cursor, char *line, size_t *length);

#endif