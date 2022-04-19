#include <config.h>
#include <field.h>
#include <prepare.h>

char field[N][M];
std::vector<std::vector<int>> history;
std::vector<int> x_streak_sum, o_streak_sum;
ull X_mask, O_mask;
int rate_delta;
bool have_win_streak;

void init_field() {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < M; ++j) {
      field[i][j] = '.';
    }
  }
  history.clear();
  x_streak_sum.assign(streak_number, 0);
  o_streak_sum.assign(streak_number, 0);
  X_mask = O_mask = 0;
  rate_delta = 0;
  have_win_streak = false;
}

void print_numeration() {
  for (int j = 0; j < M; ++j) {
    std::cout << j + 1;
  }
  std::cout << '\n';
}

void print_field() {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < M; ++j) {
      std::cout << field[i][j];
    }
    std::cout << '\n';
  }
}

void print_position() {
  print_numeration();
  print_field();
}

bool is_filled() {
  return history.size() == N * M;
}

char get_streak() {
  if (*max_element(x_streak_sum.begin(), x_streak_sum.end()) == WIN_STREAK) {
    return 'X';
  } else if (*max_element(o_streak_sum.begin(), o_streak_sum.end()) ==
             WIN_STREAK) {
    return 'O';
  } else {
    return '.';
  }
}

bool is_end() {
  return is_filled() || have_win_streak;
}

char get_turn_order() {
  return history.size() & 1 ? 'O' : 'X';
}

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
  field[i][j] = get_turn_order();
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
      if (get_turn_order() == 'O') {
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

char get_result() {
  return get_streak();
}

bool is_draw() {
  return get_result() == '.';
}

bool is_X() {
  return get_result() == 'X';
}

bool is_O() {
  return get_result() == 'O';
}
