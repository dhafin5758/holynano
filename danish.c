#include <stdio.h>
#include <string.h>

#include "danish.h"

void loadFile(Buffer *buffer, const char *filename) {

    FILE *file;
    char line[1024];
    Node *current;

    if (buffer == NULL || filename == NULL) {
        return;
    }

    file = fopen(filename, "r");

    if (file == NULL) {
        return;
    }

    while (fgets(line, sizeof(line), file) != NULL) {

        size_t len = strlen(line);

        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        addLine(buffer, line);
    }

    fclose(file);

    /* TAMPILKAN ISI BUFFER */
    current = buffer->head;

    while (current != NULL) {
        printf("%s\n", current->info);
        current = current->next;
    }
}

void saveFile(Buffer *buffer, const char *filename) {

    FILE *file;
    Node *current;

    if (buffer == NULL || filename == NULL) {
        return;
    }

    file = fopen(filename, "w");

    if (file == NULL) {

        printf("Gagal menyimpan file: %s\n", filename);

        return;
    }

    current = buffer->head;

    while (current != NULL) {

        fprintf(file, "%s\n", current->info);

        current = current->next;
    }

    fclose(file);
}