#pragma once

void computer_move() {
  int j = get_the_best_move();
  cout << "Computer move: " << j + 1 << '\n';
  move(j);
}

int get_human_move() {
  cout << "Human move: ";
  int j;
  cin >> j;
  return j - 1;
}

int get_tip() {
  int j = get_the_best_move();
  cout << "Tip: " << j + 1 << '\n';
  return j;
}

void human_move() {
  int j = get_human_move();
  if (j == TIP - 1) {
    j = get_tip();
  }
  move(j);
}

void print_new_game() { cout << "===NEW GAME===\n"; }

int game_type;

/*
if game_type == FRIST_PLAYER, then the human plays for first player
else if game_type == SECOND_PLAYER, then the human plays for second player
else if game_type == BOTH_PLAYERS, then the human plays for both players
else if game_type == GAME_FOR_LOG, then the computer plays for both players
and this game will be logging
else if game_type == EXIT, then the program will be over
else if game_type == CHANGE_SEED, then you can be able change game seed
else the computer plays for both players
*/
void input_game_type() {
  cout << "Game type: ";
  cin >> game_type;
  if (game_type == EXIT) {
    save_data_base();
    exit(0);
  }
}

void init_game_type() {
  static int it = 0;
  ++it;
  if (it < PREGAMES) {
    game_type = GAME_FOR_LOG;
  } else {
    input_game_type();
  }
}

void print_start_position() { print_position(); }

void init_seed() {
  unsigned int seed = time(NULL);
  cout << "Seed: " << seed << '\n';
  srand(seed);
}

void change_seed() {
  cout << "Input your seed: ";
  int seed;
  cin >> seed;
  srand(seed);
}

void init_game() {
  print_new_game();
  init_seed();
  init_game_type();
  while (game_type == CHANGE_SEED) {
    change_seed();
    init_game_type();
  }
  init_field();
  print_start_position();
}

int player;

void switch_player() {
  player = (player == FIRST_PLAYER ? SECOND_PLAYER : FIRST_PLAYER);
}

bool is_human_moving() { return player == game_type || game_type == 3; }

void mainloop() {
  player = FIRST_PLAYER;
  while (!is_end()) {
    if (is_human_moving()) {
      human_move();
    } else {
      computer_move();
    }
    switch_player();
    print_position();
  }
}

void print_result() {
  if (is_draw()) {
    cout << "Draw.\n";
  } else if (is_X()) {
    cout << "First player won.\n";
  } else {
    cout << "Second player won.\n";
  }
}

void log_game() {
  if (game_type != GAME_FOR_LOG) {
    return;
  }
  bool flag = (is_draw());
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < M; ++j) {
      field[i][j] = '.';
    }
  }
  for (int i = 0; i < (int)history.size(); ++i) {
    int shift = get_shift();
    vector<int> &cur = data_base[{get_maskX(shift), get_maskO(shift)}];
    cur[(history[i][1] - shift + M) % M] +=
        ((history.size() - i) % 2 == 0 || flag);
    field[history[i][0]][history[i][1]] = (i % 2 == 0 ? 'X' : 'O');
  }
}

void run() {
  init_game();
  mainloop();
  print_result();
  log_game();
}
