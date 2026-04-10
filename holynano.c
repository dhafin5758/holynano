#include <string.h>

#include "basicFramework.h"
#include "basicLine.h"
#include "manageFile.h"

static void initEmptyEditor(void) {
  buffer[0][0] = '\0';
  num_rows = 1;
  cursor_x = 0;
  cursor_y = 0;
}

int main(int argc, char *argv[]) {
  enableRawMode();

  if (argc > 1 && (strcmp(argv[1], "-v") == 0 || strcmp(argv[1], "--view") == 0)) {
    setViewOnlyMode(1);
    if (argc > 2) {
      loadFile(argv[2]);
    } else {
      initEmptyEditor();
    }
  } else if (argc > 1) {
    openOrCreateFile(argv[1]);
  } else {
    initEmptyEditor();
  }

  refresh_screen();

  run_editor_loop();

  return 0;
}