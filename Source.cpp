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

using namespace std;

#define MTEST() \
  int _t;       \
  cin >> _t;    \
  for (int _tn = 0; _tn < _t; ++_tn)
#define ALL(c) c.begin(), c.end()
#define SZ(c) (int)c.size()

const int N = 6;
const int M = 7;

bool have_win_streak;
vector<int> streak_sum;
vector<int> streak[N][M];

void build_streak(int i, int j, int dx, int dy) {
  const int WIN_STREAK = 4;
  for (int k = 0, x = i, y = j; k < WIN_STREAK;
       ++k, x = (x + dx + N) % N, y = (y + dy + M) % M) {
    streak[x][y].push_back(streak_sum.size());
  }
  streak_sum.push_back(0);
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
  fill(streak_sum.begin(), streak_sum.end(), 0);
  history.clear();
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

void add(int i, int j, int d) {
  for (int ind : streak[i][j]) {
    if (streak_sum[ind] == -4 || streak_sum[ind] == 4) {
      have_win_streak = false;
    }
    streak_sum[ind] += d;
    if (streak_sum[ind] == -4 || streak_sum[ind] == 4) {
      have_win_streak = true;
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
  add(i, j, get_streak_impact(field[i][j]));
  history.push_back({i, j});
}

void undo() {
  int i = history.back()[0];
  int j = history.back()[1];
  history.pop_back();
  add(i, j, -get_streak_impact(field[i][j]));
  field[i][j] = '.';
}

void human_move() {
  int j;
  cin >> j;
  --j;
  move(j);
}

const int O_WIN = -1000;
const int DRAW = 0;
const int X_WIN = 1000;

int get_x_win() { return X_WIN - history.size(); }
int get_o_win() { return O_WIN + history.size(); }

int make_move(int depth, int alpha, int beta) {
  if (is_end()) {
    if (have_win_streak) {
      return get_move_type() == 'O' ? get_x_win() : get_o_win();
    } else {
      return DRAW;
    }
  }
  if (depth == 0) {
    return DRAW;
  }
  vector<int> options;
  for (int j = 0; j < M; ++j) {
    if (field[0][j] == '.') {
      move(j);
      if (is_end()) {
        undo();
        return get_move_type() == 'X' ? get_x_win() : get_o_win();
      } else {
        undo();
        options.push_back(j);
      }
    }
  }
  random_shuffle(options.begin(), options.end());
  if (get_move_type() == 'X') {
    int res = alpha;
    for (int j : options) {
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
    for (int j : options) {
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

int get_good_depth() {
  if (history.size() <= 6) {
    return 6;
  } else if (history.size() <= 8){
	  return 7;
  } else if (history.size() <= 10){
	  return 8;
  } else if (history.size() <= 15){
	  return 10;
  } else {
    return N * M;
  }
}

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
      if (res > DRAW) {
        break;
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
      if (res < DRAW) {
        break;
      }
    }
  }
  cout << "MOVE RATE: " << res << '\n';
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
