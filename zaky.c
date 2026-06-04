#include "zaky.h"

char *clipboard = NULL;

void refreshScreen(Buffer *buffer, Node *cursor, int colIndex) {
    if (buffer == NULL) return;

    write(STDOUT_FILENO, "\x1b[2J", 4);
    write(STDOUT_FILENO, "\x1b[H", 3);

    Node *current = buffer->head;
    int rowNum = 1;
    int cursorRow = 1;

    while (current != NULL) {
        if (current == cursor) {
            cursorRow = rowNum;
            printf("> %s\r\n", current->info);
        } else {
            printf("  %s\r\n", current->info);
        }
        current = current->next;
        rowNum++;
    }
    
    if (cursorRow < 1) cursorRow = 1;
    int targetCol = colIndex + 3; 
    if (targetCol < 1) targetCol = 1;

    char gotoSeq[32];
    sprintf(gotoSeq, "\x1b[%d;%dH", cursorRow, colIndex + 3);
    write(STDOUT_FILENO, gotoSeq, strlen(gotoSeq));

    fflush(stdout);
}

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
