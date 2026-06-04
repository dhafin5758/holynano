#include "zaky.h"

char *clipboard = NULL;

void moveCursorUp(Node **cursorNode, int *colIndex) {
    if (cursorNode == NULL || *cursorNode == NULL) return;
    
    if ((*cursorNode)->prev != NULL) {
        *cursorNode = (*cursorNode)->prev;
        int len = strlen((*cursorNode)->info);
        if (*colIndex > len) {
            *colIndex = len;
        }
    }
}

void moveCursorDown(Node **cursorNode, int *colIndex) {
    if (cursorNode == NULL || *cursorNode == NULL) return;
    
    if ((*cursorNode)->next != NULL) {
        *cursorNode = (*cursorNode)->next;
        int len = strlen((*cursorNode)->info);
        if (*colIndex > len) {
            *colIndex = len;
        }
    }
}

void moveCursorLeft(Node *cursorNode, int *colIndex) {
    if (cursorNode == NULL || *colIndex <= 0) return;
    (*colIndex)--;
}

void moveCursorRight(Node *cursorNode, int *colIndex) {
    if (cursorNode == NULL || colIndex == NULL) return;
    int len = strlen(cursorNode->info);
    if (*colIndex < len) {
        (*colIndex)++;
    }
}

void deleteLine(Buffer *buffer, Node **cursorNode) {
    if (buffer == NULL || cursorNode == NULL || *cursorNode == NULL) return;

    Node *toDelete = *cursorNode;

    if (toDelete->next != NULL) {
        *cursorNode = toDelete->next; 
    } else if (toDelete->prev != NULL) {
        *cursorNode = toDelete->prev; 
    } else {
        *cursorNode = NULL; 
    }

    deleteNode(buffer, toDelete);
}

void backspaceChar(Node *cursorNode, int *colIndex)
{
    int len;

    if (cursorNode == NULL)
        return;

    if (colIndex == NULL)
        return;

    if (*colIndex <= 0)
        return;

    len = strlen(cursorNode->info);

    memmove(
        &cursorNode->info[*colIndex - 1],
        &cursorNode->info[*colIndex],
        len - *colIndex + 1
    );

    (*colIndex)--;
}