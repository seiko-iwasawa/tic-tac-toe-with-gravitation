#include "config.h"
#include "game.h"
#include "prepare.h"

int main() {
  // enables automatic flushing of the output stream after any output operation.
  std::cout << std::unitbuf;

  // generates different seeds for each program run
  // https://stackoverflow.com/questions/7748071/same-random-numbers-every-time-i-run-the-program
  // it has a problem; if we run programs at the same time we will have the same
  // seeds anyway
  // https://stackoverflow.com/a/13446015https://stackoverflow.com/a/13446015
  srand((unsigned int)time(NULL));

  prepare();
  while (true) {
    run();
  }

  return 0;
}
