#ifndef DHAFIN_H
#define DHAFIN_H

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
typedef struct Node {
  char *info; //point ke first char
  struct Node *prev;
  struct Node *next;
} Node;

typedef struct Buffer {
  Node *head;
  Node *tail;
  size_t length;
} Buffer;

void initBuffer(Buffer *buffer);
Node *makeNode(const char *info);
int addLine(Buffer *buffer, const char *info);
int insertAfter(Buffer *buffer, Node *node, const char *info);
void deleteNode(Buffer *buffer, Node *node);
void clearBuffer(Buffer *buffer);
void clearScreen(void);
int enableRawMode(void);
void disableRawMode(void);

#endif

