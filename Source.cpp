#include <algorithm>
#include <bitset>
#include <cassert>
#include <cmath>
#include <deque>
#include <functional>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <ostream>
#include <queue>
#include <random>
#include <set>
#include <string>
#include <vector>

#include <ctime>

using namespace std;

#define MTEST() \
  int _t;       \
  cin >> _t;    \
  for (int _tn = 0; _tn < _t; ++_tn)
#define ALL(c) c.begin(), c.end()
#define SZ(c) (int)c.size()

typedef unsigned long long ull;

const int N = 6;
const int M = 7;

int rate_delta;
bool have_win_streak;
vector<int> x_streak_sum, o_streak_sum;
vector<int> streak[N][M];

void build_streak(int i, int j, int dx, int dy) {
  const int WIN_STREAK = 4;
  for (int k = 0, x = i, y = j; k < WIN_STREAK;
       ++k, x = (x + dx + N) % N, y = (y + dy + M) % M) {
    streak[x][y].push_back(x_streak_sum.size());
  }
  x_streak_sum.push_back(0);
  o_streak_sum.push_back(0);
}

void build_streak(int i, int j) {
  const int DX[4] = {-1, 0, +1, +1};
  const int DY[4] = {+1, +1, +1, 0};
  for (int k = 0; k < 4; ++k) {
    build_streak(i, j, DX[k], DY[k]);
  }
}

void build_streak() {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < M; ++j) {
      build_streak(i, j);
    }
  }
}

void prepare() { build_streak(); }

char field[N][M];
vector<vector<int>> history;

void init_field() {
  have_win_streak = false;
  fill(x_streak_sum.begin(), x_streak_sum.end(), 0);
  fill(o_streak_sum.begin(), o_streak_sum.end(), 0);
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

bool is_end() { return is_filled() || have_win_streak; }

char get_move_type() { return history.size() & 1 ? 'O' : 'X'; }

int get_streak_impact(char cell) { return cell == 'O' ? -1 : +1; }

void add(int i, int j, int d, char move) {
  if (move == 'X') {
    for (int ind : streak[i][j]) {
      if (x_streak_sum[ind] == 4) {
        have_win_streak = false;
      }
      if (o_streak_sum[ind] == 0) {
        rate_delta -= x_streak_sum[ind] * x_streak_sum[ind];
      }
      if (x_streak_sum[ind] == 0) {
        rate_delta -= -o_streak_sum[ind] * o_streak_sum[ind];
      }
      x_streak_sum[ind] += d;
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
      o_streak_sum[ind] += d;
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
  int i = get_move_place(j);
  field[i][j] = get_move_type();
  add(i, j, get_streak_impact(field[i][j]), field[i][j]);
  history.push_back({i, j});
}

void undo() {
  int i = history.back()[0];
  int j = history.back()[1];
  history.pop_back();
  add(i, j, -get_streak_impact(field[i][j]), field[i][j]);
  field[i][j] = '.';
}

void human_move() {
  cout << "Human move: ";
  int j;
  cin >> j;
  --j;
  move(j);
}

const int O_WIN = -1e9;
const int DRAW = 0;
const int X_WIN = 1e9;

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

int make_move(int depth, int alpha, int beta) {
  if (is_end() || depth == 0) {
    return get_rate();
  }
  vector<pair<int, int>> options;
  for (int j = 0; j < M; ++j) {
    if (field[0][j] == '.') {
      move(j);
      options.push_back({get_rate(), j});
      if (is_end()) {
        undo();
        return options.back().first;
      } else {
        undo();
      }
    }
  }
  sort(options.begin(), options.end());
  if (get_move_type() == 'X') {
    reverse(options.begin(), options.end());
    int res = alpha;
    for (auto e : options) {
      int j = e.second;
      move(j);
      int nxt = make_move(depth - 1, alpha, beta);
      undo();
      res = max(res, nxt);
      alpha = max(alpha, res);
      if (res >= beta) {
        break;
      }
    }
    return res;
  } else {
    int res = beta;
    for (auto e : options) {
      int j = e.second;
      move(j);
      int nxt = make_move(depth - 1, alpha, beta);
      undo();
      res = min(res, nxt);
      beta = min(beta, res);
      if (res <= alpha) {
        break;
      }
    }
    return res;
  }
}

int get_good_depth() { return 5; }

int make_move() {
  if (history.empty()) {
    return 0;
  }
  vector<int> options;
  for (int j = 0; j < M; ++j) {
    if (field[0][j] == '.') {
      move(j);
      if (is_end()) {
        undo();
        return j;
      } else {
        undo();
        options.push_back(j);
      }
    }
  }
  random_shuffle(options.begin(), options.end());
  int start = clock();
  int res, bj;
  if (get_move_type() == 'X') {
    res = O_WIN;
    bj = options[0];
    for (int j : options) {
      move(j);
      int nxt = make_move(get_good_depth(), O_WIN, X_WIN);
      undo();
      if (res < nxt) {
        res = nxt;
        bj = j;
      }
    }
  } else {
    res = X_WIN;
    bj = options[0];
    for (int j : options) {
      move(j);
      int nxt = make_move(get_good_depth(), O_WIN, X_WIN);
      undo();
      if (res > nxt) {
        res = nxt;
        bj = j;
      }
    }
  }
  cout << "MOVE RATE: " << res << '\n';
  cout << "TIME: " << clock() - start << '\n';
  return bj;
}

void computer_move() {
  int j = make_move();
  cout << "Computer move: " << j + 1 << '\n';
  move(j);
}

void run() {
  cout << "===NEW GAME===\n";
  int human_player;
  cout << "Human player: ";
  cin >> human_player;
  --human_player;
  init_field();
  print_position();
  int cur_player = 0;
  while (!is_end()) {
    if (cur_player == human_player || human_player == 2) {
      human_move();
    } else {
      computer_move();
    }
    cur_player ^= true;
    print_position();
  }
  if (!have_win_streak) {
    cout << "Draw.\n";
  } else if (get_move_type() == 'O') {
    cout << "First player won.\n";
  } else {
    cout << "Second player won.\n";
  }
}

signed main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  cout.tie(NULL);
  prepare();
  while (true) {
    run();
  }
  return 0;
}
