#pragma once
struct Field {
  char field[N][M];
  vector<vector<int>> history;
  vector<int> streak_type;
  ull X_mask, O_mask;
  int rate_delta;
  bool have_win_streak;

  void init_field() {
    X_mask = O_mask = 0;
    have_win_streak = false;
    x_streak_sum.assign(streak_number, 0);
    o_streak_sum.assign(streak_number, 0);
    history.clear();
    rate_delta = 0;
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < M; ++j) {
        field[i][j] = '.';
      }
    }
  }

  void print_numeration() {
    for (int j = 0; j < M; ++j) {
      cout << j + 1;
    }
    cout << '\n';
  }

  void print_field() {
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < M; ++j) {
        cout << field[i][j];
      }
      cout << '\n';
    }
  }

  void print_position() {
    print_numeration();
    print_field();
  }

  bool is_filled() { return history.size() == N * M; }

  char get_streak(int i, int j, int dx, int dy) {
    for (int k = 0; k < WIN_STREAK; ++k) {
      if (field[(i + k * dx + N) % N][(j + k * dy + M) % M] != field[i][j]) {
        return '.';
      }
    }
    return field[i][j];
  }

  char get_streak(int i, int j) {
    for (int k = 0; k < 4; ++k) {
      int streak = get_streak(i, j, DX[k], DY[k]);
      if (streak != '.') {
        return streak;
      }
    }
    return '.';
  }

  char get_streak() {
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < M; ++j) {
        char streak = get_streak(i, j);
        if (streak != '.') {
          return streak;
        }
      }
    }
    return '.';
  }

  bool is_end() { return is_filled() || have_win_streak; }

  char get_move_type() { return history.size() & 1 ? 'O' : 'X'; }

  const int KEK = 5;

  void add(int i, int j, int d, char move) {
    if (move == 'X') {
      for (int ind : streak[i][j]) {
        if (x_streak_sum[ind] == 4) {
          have_win_streak = false;
        }
        rate_delta -= (x_streak_sum[ind] == 3 && o_streak_sum[ind] == 0) * KEK;
        rate_delta -= (o_streak_sum[ind] == 3 && x_streak_sum[ind] == 0) * -KEK;
        if (o_streak_sum[ind] == 0) {
          rate_delta -= x_streak_sum[ind] * x_streak_sum[ind];
        }
        if (x_streak_sum[ind] == 0) {
          rate_delta -= -o_streak_sum[ind] * o_streak_sum[ind];
        }
        x_streak_sum[ind] += d;
        rate_delta += (x_streak_sum[ind] == 3 && o_streak_sum[ind] == 0) * KEK;
        rate_delta += (o_streak_sum[ind] == 3 && x_streak_sum[ind] == 0) * -KEK;
        if (x_streak_sum[ind] == 0) {
          rate_delta += -o_streak_sum[ind] * o_streak_sum[ind];
        }
        if (o_streak_sum[ind] == 0) {
          rate_delta += x_streak_sum[ind] * x_streak_sum[ind];
        }
        if (x_streak_sum[ind] == 4) {
          have_win_streak = true;
        }
      }
    } else {
      for (int ind : streak[i][j]) {
        if (o_streak_sum[ind] == 4) {
          have_win_streak = false;
        }
        if (x_streak_sum[ind] == 0) {
          rate_delta -= -o_streak_sum[ind] * o_streak_sum[ind];
        }
        if (o_streak_sum[ind] == 0) {
          rate_delta -= x_streak_sum[ind] * x_streak_sum[ind];
        }
        rate_delta -= (x_streak_sum[ind] == 3 && o_streak_sum[ind] == 0) * KEK;
        rate_delta -= (o_streak_sum[ind] == 3 && x_streak_sum[ind] == 0) * -KEK;
        o_streak_sum[ind] += d;
        rate_delta += (x_streak_sum[ind] == 3 && o_streak_sum[ind] == 0) * KEK;
        rate_delta += (o_streak_sum[ind] == 3 && x_streak_sum[ind] == 0) * -KEK;
        if (o_streak_sum[ind] == 0) {
          rate_delta += x_streak_sum[ind] * x_streak_sum[ind];
        }
        if (x_streak_sum[ind] == 0) {
          rate_delta += -o_streak_sum[ind] * o_streak_sum[ind];
        }
        if (o_streak_sum[ind] == 4) {
          have_win_streak = true;
        }
      }
    }
  }

  int get_move_place(int j) {
    int i = N - 1;
    while (field[i][j] != '.') {
      --i;
    }
    return i;
  }

  void move(int j) {
    assert(0 <= j && j <= M - 1);
    int i = get_move_place(j);
    field[i][j] = get_move_type();
    if (field[i][j] == 'X') {
      X_mask ^= 1LL << (i * M + j);
    } else {
      O_mask ^= 1LL << (i * M + j);
    }
    add(i, j, 1, field[i][j]);
    history.push_back({i, j});
  }

  void undo() {
    int i = history.back()[0];
    int j = history.back()[1];
    history.pop_back();
    if (field[i][j] == 'X') {
      X_mask ^= 1LL << (i * M + j);
    } else {
      O_mask ^= 1LL << (i * M + j);
    }
    add(i, j, -1, field[i][j]);
    field[i][j] = '.';
  }

  int get_rate() {
    if (is_end()) {
      if (have_win_streak) {
        if (get_move_type() == 'O') {
          return X_WIN - history.size();
        } else {
          return O_WIN + history.size();
        }
      } else {
        return DRAW;
      }
    } else {
      return DRAW + rate_delta;
    }
  }

  Field() { init_field(); }

  auto& operator[](int i) { return field[i]; }
  Field& operator=(Field& f) {
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < M; ++j) {
        field[i][j] = f[i][j];
      }
    }
    history = f.history;
    streak_type = f.streak_type;
    X_mask = f.X_mask, O_mask = f.O_mask;
    rate_delta = f.rate_delta;
    have_win_streak = f.have_win_streak;
    return *this;
  }
};
