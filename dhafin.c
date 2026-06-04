#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#include "dhafin.h"

static struct termios originalTerminal;
static int isRawMode = 0;

char *copyInfo(const char *info) {
  const char *text;
  size_t textLength;
  char *result;

  text = info;

  textLength = strlen(text);
  result = malloc(textLength + 1);

  strcpy(result, text);
  return result;
}

void initBuffer(Buffer *buffer) {
  buffer->head = NULL;
  buffer->tail = NULL;
  buffer->length = 0;
}

Node *makeNode(const char *info) {
  Node *node = malloc(sizeof(Node));

  node->info = copyInfo(info);
  node->prev = NULL;
  node->next = NULL;

  return node;
}

int addLine(Buffer *buffer, const char *info) {
  Node *node;

  node = makeNode(info);

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

  newNode = makeNode(info);

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
  Node *current;

  current = buffer->head;
  while (current != NULL) {
    Node *next = current->next;

    free(current->info);
    free(current);

    current = next;
  }

  initBuffer(buffer);
}

void disableRawMode(void) {
  if (isRawMode) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &originalTerminal);
    isRawMode = 0;
  }
}

int enableRawMode(void) {
  struct termios raw;

  if (!isatty(STDIN_FILENO)) {
    return 1;
  }

  if (tcgetattr(STDIN_FILENO, &originalTerminal) == -1) {
    return 0;
  }

  raw = originalTerminal;
  raw.c_lflag &= ~(ECHO | ICANON);
  raw.c_iflag &= ~(IXON | ICRNL);
  raw.c_cc[VMIN] = 1;
  raw.c_cc[VTIME] = 0;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
    return 0;
  }

  isRawMode = 1;
  atexit(disableRawMode);
  return 1;
}
