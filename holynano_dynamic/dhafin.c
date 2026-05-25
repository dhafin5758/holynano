#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "dhafin.h"

static struct termios oldTerminal;
static int rawModeOn = 0;

static char *copyInfo(const char *info) {
  const char *source;
  size_t length;
  char *copy;

  if (info != NULL) {
    source = info;
  } else {
    source = "";
  }

  length = strlen(source);
  copy = malloc(length + 1);

  if (copy == NULL) {
    return NULL;
  }

  memcpy(copy, source, length + 1);
  return copy;
}

void initBuffer(Buffer *buffer) {
  if (buffer == NULL) {
    return;
  }

  buffer->head = NULL;
  buffer->tail = NULL;
  buffer->length = 0;
}

Node *makeNode(const char *info) {
  Node *node = malloc(sizeof(Node));

  if (node == NULL) {
    return NULL;
  }

  node->info = copyInfo(info);
  if (node->info == NULL) {
    free(node);
    return NULL;
  }

  node->prev = NULL;
  node->next = NULL;

  return node;
}

int addLine(Buffer *buffer, const char *info) {
  Node *node;

  if (buffer == NULL) {
    return 0;
  }

  node = makeNode(info);
  if (node == NULL) {
    return 0;
  }

  if (buffer->head == NULL) {
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

int insertAfter(Buffer *buffer, Node *node, const char *info) {
  Node *newNode;

  if (buffer == NULL) {
    return 0;
  }

  if (node == NULL) {
    return addLine(buffer, info);
  }

  newNode = makeNode(info);
  if (newNode == NULL) {
    return 0;
  }

  newNode->prev = node;
  newNode->next = node->next;

  if (node->next != NULL) {
    node->next->prev = newNode;
  } else {
    buffer->tail = newNode;
  }

  node->next = newNode;
  buffer->length++;

  return 1;
}

void deleteNode(Buffer *buffer, Node *node) {
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

void clearBuffer(Buffer *buffer) {
  Node *node;

  if (buffer == NULL) {
    return;
  }

  node = buffer->head;
  while (node != NULL) {
    Node *next = node->next;

    free(node->info);
    free(node);

    node = next;
  }

  initBuffer(buffer);
}

void disableRawMode(void) {
  if (rawModeOn) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &oldTerminal);
    rawModeOn = 0;
  }
}

int enableRawMode(void) {
  struct termios raw;

  if (!isatty(STDIN_FILENO)) {
    return 1;
  }

  if (tcgetattr(STDIN_FILENO, &oldTerminal) == -1) {
    return 0;
  }

  raw = oldTerminal;
  raw.c_lflag &= ~(ECHO | ICANON);
  raw.c_iflag &= ~(IXON | ICRNL);
  raw.c_cc[VMIN] = 1;
  raw.c_cc[VTIME] = 0;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
    return 0;
  }

  rawModeOn = 1;
  atexit(disableRawMode);
  return 1;
}
