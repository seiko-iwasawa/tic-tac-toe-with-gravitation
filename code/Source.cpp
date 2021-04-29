#include "config.h"

#include "data-base.h"

#include "prepare.h"

#include "field.h"

#include "ai.h"

#include "game.h"

signed main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  cout.tie(NULL);
  // srand((unsigned int)time(NULL));
  prepare();
  while (true) {
    run_game();
  }
  return 0;
}
