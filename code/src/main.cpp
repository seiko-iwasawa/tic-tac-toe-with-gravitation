#include "config.h"
#include "game.h"
#include "prepare.h"

signed main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(NULL);
  std::cout.tie(NULL);
  // srand((unsigned int)time(NULL));
  prepare();
  while (true) {
    run();
  }
  return 0;
}
