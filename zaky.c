#include "zaky.h"

char *clipboard = NULL;

static size_t getLineLength(Node *node) {
    if (node == NULL || node->info == NULL) {
        return 0;
    }

    return strlen(node->info);
}

static void limitCursorColumn(Cursor *cursor) {
    size_t lineLength;

    if (cursor == NULL || cursor->node == NULL) {
        return;
    }

    lineLength = getLineLength(cursor->node);
    if (cursor->column > lineLength) {
        cursor->column = lineLength;
    }
}

void initCursor(Cursor *cursor, Buffer *buffer) {
    if (cursor == NULL) {
        return;
    }

    cursor->node = NULL;
    cursor->column = 0;

    if (buffer != NULL) {
        cursor->node = buffer->head;
        limitCursorColumn(cursor);
    }
}

void setCursor(Cursor *cursor, Node *node) {
    if (cursor == NULL) {
        return;
    }

    cursor->node = node;
    cursor->column = getLineLength(node);
}

void moveCursorLeft(Cursor *cursor) {
    if (cursor == NULL || cursor->node == NULL) {
        return;
    }

    if (cursor->column > 0) {
        cursor->column--;
    } else if (cursor->node->prev != NULL) {
        cursor->node = cursor->node->prev;
        cursor->column = getLineLength(cursor->node);
    }
}

void moveCursorRight(Cursor *cursor) {
    size_t lineLength;

    if (cursor == NULL || cursor->node == NULL) {
        return;
    }

    lineLength = getLineLength(cursor->node);
    if (cursor->column < lineLength) {
        cursor->column++;
    } else if (cursor->node->next != NULL) {
        cursor->node = cursor->node->next;
        cursor->column = 0;
    }
}

void moveCursorUp(Cursor *cursor) {
    if (cursor == NULL || cursor->node == NULL) {
        return;
    }

    if (cursor->node->prev != NULL) {
        cursor->node = cursor->node->prev;
        limitCursorColumn(cursor);
    }
}

void moveCursorDown(Cursor *cursor) {
    if (cursor == NULL || cursor->node == NULL) {
        return;
    }

    if (cursor->node->next != NULL) {
        cursor->node = cursor->node->next;
        limitCursorColumn(cursor);
    }
}

void deleteLine(Buffer *buffer, Cursor *cursor){
    Node *toDelete;

    if (buffer == NULL ||cursor == NULL ||cursor->node == NULL){
        return;
    }

    toDelete = cursor->node;

    if (toDelete->next != NULL){
        cursor->node = toDelete->next;
    }
    else if (toDelete->prev != NULL){
        cursor->node = toDelete->prev;
    }
    else{
        cursor->node = NULL;
    }
    cursor->column = 0;
    deleteNode(buffer, toDelete);
}

void backspaceChar(Cursor *cursor, char *line, size_t *length) {

    if (cursor->node != NULL) {
        size_t nodeLength = getLineLength(cursor->node);

        if (cursor->column > 0 && cursor->column <= nodeLength) {
            memmove(&cursor->node->info[cursor->column - 1],
                    &cursor->node->info[cursor->column],
                    nodeLength - cursor->column + 1);
            cursor->column--;
        }

        return;
    }

    if (line != NULL && length != NULL &&
        cursor->column > 0 && cursor->column <= *length) {
        memmove(&line[cursor->column - 1],
                &line[cursor->column],
                *length - cursor->column + 1);
        cursor->column--;
        (*length)--;
    }
}

