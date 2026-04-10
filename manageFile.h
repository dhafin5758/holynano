#ifndef MANAGEFILE_H
#define MANAGEFILE_H

void createEmptyFile(const char *filename);
void loadFile(const char *filename);
void openOrCreateFile(const char *filename);
void saveFile(void);
void setViewOnlyMode(int enabled);
int isViewOnlyMode(void);

#endif
