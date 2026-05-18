#ifndef TEXT_BUFFER_H
#define TEXT_BUFFER_H

#include <stddef.h>

typedef struct TextNode {
  char *info;
  struct TextNode *prev;
  struct TextNode *next;
} TextNode;

typedef struct TextBuffer {
  TextNode *head;
  TextNode *tail;
  size_t length;
} TextBuffer;

void initTextBuffer(TextBuffer *buffer);
TextNode *createTextNode(const char *info);
int appendTextLine(TextBuffer *buffer, const char *info);
int insertTextLineAfter(TextBuffer *buffer, TextNode *node, const char *info);
void deleteTextNode(TextBuffer *buffer, TextNode *node);
void clearTextBuffer(TextBuffer *buffer);

#endif
