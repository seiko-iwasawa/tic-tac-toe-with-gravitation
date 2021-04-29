#pragma once

struct Game {
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

  int game_type;

  /*
  if game_type == FRIST_PLAYER, then the human plays for first player
  else if game_type == SECOND_PLAYER, then the human plays for second player
  else if game_type == BOTH_PLAYERS, then the human plays for both players
  else if game_type == GAME_FOR_LOG, then the computer plays for both players
  and this game will be logging
  else if game_type == EXIT, then the program will be over
  else the computer plays for both players
  */
  void input_game_type() {
    static int it = 0;
    ++it;
    if (it < PREGAMES) {
      game_type = GAME_FOR_LOG;
    } else {
      cout << "Game type: ";
      cin >> game_type;
      if (game_type == EXIT) {
        save_data_base();
        exit(0);
      }
    }
  }

  void print_start_position() { print_position(); }

  void init_game() {
    input_game_type();
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
    if (get_streak() == '.') {
      cout << "Draw.\n";
    } else if (get_move_type() == 'O') {
      cout << "First player won.\n";
    } else {
      cout << "Second player won.\n";
    }
  }

  void log_game() {
    if (game_type != GAME_FOR_LOG) {
      return;
    }
    bool flag = (get_streak() == '.');
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

  Game() {
    init_game();
    mainloop();
    print_result();
    log_game();
  }
};
