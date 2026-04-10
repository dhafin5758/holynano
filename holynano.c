#include "basicFramework.h"
#include "basicLine.h"
#include "manageFile.h"

int main(int argc, char *argv[]) {
  enableRawMode();

  if (argc > 1) {
    openOrCreateFile(argv[1]);
  } else {
    buffer[0][0] = '\0';
    num_rows = 1;
    cursor_x = 0;
    cursor_y = 0;
  }

  refresh_screen();

  run_editor_loop();

  return 0;
}