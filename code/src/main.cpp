#include "config.h"
#include "game.h"
#include "prepare.h"

signed main() {
  std::cout << std::unitbuf;  // Enables automatic flushing of the output stream
                              // after any output operation.
  // srand((unsigned int)time(NULL));
  prepare();
  while (true) {
    run();
  }
  return 0;
}
