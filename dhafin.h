<<<<<<< HEAD:holynano_dynamic/dhafin.h
#ifndef DHAFIN_H
#define DHAFIN_H

#include <stddef.h>

typedef struct Node {
  char *info;
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
int enableRawMode(void);
void disableRawMode(void);

#endif
=======
#ifndef DHAFIN_H
#define DHAFIN_H

#include <stddef.h>

typedef struct Node {
  char *info; //point to string
  struct Node *prev;
  struct Node *next;
} Node;

typedef struct Buffer {
  Node *head;
  Node *tail;
  size_t length; //unsigned int (node length) 
} Buffer;

void initBuffer(Buffer *buffer);
Node *makeNode(const char *info);
int addLine(Buffer *buffer, const char *info);
int insertAfter(Buffer *buffer, Node *node, const char *info);
void clearBuffer(Buffer *buffer);
int enableRawMode(void);
void disableRawMode(void);

#endif
>>>>>>> 13c565e013847c4452ef284270449744a9fc6b0d:dhafin.h
