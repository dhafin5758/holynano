#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "basicFramework.h"
#include "basicLine.h"

int main() {
  enableRawMode();
  
  // start with one empty line
  buffer[0][0] = '\0';
  num_rows = 1;
  refresh_screen();

  run_editor_loop();

  return 0;
}