#include "zaky.h"

void moveCursorUp(Node **cursorNode) {
    if (cursorNode == NULL || *cursorNode == NULL) return;
    
    if ((*cursorNode)->prev != NULL) {
        *cursorNode = (*cursorNode)->prev;
    }
}

void moveCursorDown(Node **cursorNode) {
    if (cursorNode == NULL || *cursorNode == NULL) return;
    
    if ((*cursorNode)->next != NULL) {
        *cursorNode = (*cursorNode)->next;
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
