#ifndef DANISH_H
#define DANISH_H

#include "dhafin.h"
#include "zaky.h"

void loadFile(Buffer *buffer, const char *filename);
void saveFile(Buffer *buffer, const char *filename);
void redrawScreen(Buffer *buffer, const char *line, Node *cursorNode, size_t cursorColumn);
int insertChar(Cursor *cursor, char **line, size_t *length, size_t *capacity, char c);

#endif