#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "dhafin.h"

void refreshScreen(Buffer *buffer, Node *cursor, int colIndex);

void moveCursorUp(Node **cursorNode, int *colIndex);
void moveCursorDown(Node **cursorNode, int *colIndex);
void moveCursorLeft(Node *cursorNode, int *colIndex);
void moveCursorRight(Node *cursorNode, int *colIndex);

void deleteLine(Buffer *buffer, Node **cursorNode);

