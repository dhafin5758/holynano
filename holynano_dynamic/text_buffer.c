#include <stdlib.h>
#include <string.h>

#include "text_buffer.h"

static char *duplicateTextInfo(const char *info) {
  const char *source = info != NULL ? info : "";
  size_t length = strlen(source);
  char *duplicate = malloc(length + 1);

  if (duplicate == NULL) {
    return NULL;
  }

  memcpy(duplicate, source, length + 1);
  return duplicate;
}

void initTextBuffer(TextBuffer *buffer) {
  if (buffer == NULL) {
    return;
  }

  buffer->head = NULL;
  buffer->tail = NULL;
  buffer->length = 0;
}

TextNode *createTextNode(const char *info) {
  TextNode *node = malloc(sizeof(TextNode));

  if (node == NULL) {
    return NULL;
  }

  node->info = duplicateTextInfo(info);
  if (node->info == NULL) {
    free(node);
    return NULL;
  }

  node->prev = NULL;
  node->next = NULL;
  return node;
}

int appendTextLine(TextBuffer *buffer, const char *info) {
  TextNode *node;

  if (buffer == NULL) {
    return 0;
  }

  node = createTextNode(info);
  if (node == NULL) {
    return 0;
  }

  if (buffer->tail == NULL) {
    buffer->head = node;
    buffer->tail = node;
  } else {
    node->prev = buffer->tail;
    buffer->tail->next = node;
    buffer->tail = node;
  }

  buffer->length++;
  return 1;
}

int insertTextLineAfter(TextBuffer *buffer, TextNode *node, const char *info) {
  TextNode *new_node;

  if (buffer == NULL) {
    return 0;
  }

  if (node == NULL) {
    return appendTextLine(buffer, info);
  }

  new_node = createTextNode(info);
  if (new_node == NULL) {
    return 0;
  }

  new_node->prev = node;
  new_node->next = node->next;

  if (node->next != NULL) {
    node->next->prev = new_node;
  } else {
    buffer->tail = new_node;
  }

  node->next = new_node;
  buffer->length++;
  return 1;
}

void deleteTextNode(TextBuffer *buffer, TextNode *node) {
  if (buffer == NULL || node == NULL) {
    return;
  }

  if (node->prev != NULL) {
    node->prev->next = node->next;
  } else {
    buffer->head = node->next;
  }

  if (node->next != NULL) {
    node->next->prev = node->prev;
  } else {
    buffer->tail = node->prev;
  }

  free(node->info);
  free(node);

  if (buffer->length > 0) {
    buffer->length--;
  }
}

void clearTextBuffer(TextBuffer *buffer) {
  TextNode *node;

  if (buffer == NULL) {
    return;
  }

  node = buffer->head;
  while (node != NULL) {
    TextNode *next = node->next;
    free(node->info);
    free(node);
    node = next;
  }

  initTextBuffer(buffer);
}
