#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "dhafin.h"

void moveCursorUp(Node **cursorNode, int *colIndex);
void moveCursorDown(Node **cursorNode, int *colIndex);
void moveCursorLeft(Node *cursorNode, int *colIndex);
void moveCursorRight(Node *cursorNode, int *colIndex);

void deleteLine(Buffer *buffer, Node **cursorNode);

void backspaceChar(Node *cursorNode, int *colIndex);

