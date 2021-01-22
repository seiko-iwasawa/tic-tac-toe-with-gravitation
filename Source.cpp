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

char cur_move_type;
char field[N][M];
vector<pair<int, int>> history;

void init_field() {
  history.clear();
  cur_move_type = 'X';
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

int find_streak(int i, int j, int dx, int dy) {
  int res = 0;
  int x = i, y = j;
  while (field[x][y] == field[i][j]) {
    ++res;
    x = (x + dx + N) % N;
    y = (y + dy + M) % M;
  }
  return res;
}

bool find_win_streak(int i, int j) {
  if (field[i][j] == '.') {
    return false;
  }
  const int C = 4;
  const int DX[4] = {-1, 0, +1, +1};
  const int DY[4] = {+1, +1, +1, 0};
  for (int k = 0; k < 4; ++k) {
    if (find_streak(i, j, DX[k], DY[k]) >= C) {
      return true;
    }
  }
  return false;
}

char find_win_streak() {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < M; ++j) {
      if (find_win_streak(i, j)) {
        return field[i][j];
      }
    }
  }
  return '.';
}

bool is_filled() {
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < M; ++j) {
      if (field[i][j] == '.') {
        return false;
      }
    }
  }
  return true;
}

bool is_end() { return is_filled() || find_win_streak() != '.'; }

void move(int j) {
  int i = N - 1;
  while (field[i][j] != '.') {
    --i;
  }
  field[i][j] = cur_move_type;
  cur_move_type = (cur_move_type == 'X' ? 'O' : 'X');
  history.push_back({i, j});
}

void undo() {
  auto last_move = history.back();
  history.pop_back();
  cur_move_type = (cur_move_type == 'X' ? 'O' : 'X');
  field[last_move.first][last_move.second] = '.';
}

void human_move() {
  int j;
  cin >> j;
  --j;
  move(j);
}

pair<int, pair<int, int>> make_move(int depth, int alpha, int beta) {
  if (is_end()) {
    if (find_win_streak() == '.') {
      return {0, {0, -1}};
    } else {
      return {-1, {0, -1}};
    }
  }
  if (depth == 0 || alpha == beta) {
    return {0, {0, 1}};
  }
  vector<int> options;
  for (int j = 0; j < M; ++j) {
    if (field[0][j] == '.') {
      move(j);
      if (is_end()) {
        undo();
        return {1, {1, j}};
      }
      undo();
      options.push_back(j);
    }
  }
  random_shuffle(options.begin(), options.end());
  pair<int, pair<int, int>> b = {-2, {-1, -1}};
  for (int j : options) {
    move(j);
    auto cur = make_move(depth - 1, alpha, -alpha);
    if (b.first < -cur.first) {
      b = {-cur.first, {cur.second.first + 1, j}};
    } else if (b.first == -cur.first && b.second.first < cur.second.first + 1) {
      b = {-cur.first, {cur.second.first + 1, j}};
    }
    undo();
    alpha = max(alpha, b.first);
    if (b.first >= beta) {
      break;
    }
  }
  return b;
}

int get_good_depth() {
  int cnt = 0;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < M; ++j) {
      cnt += (field[i][j] != '.');
    }
  }
  if (cnt <= 20) {
	  return 20;
  }
  else {
	  return N * M;
  }
}

void computer_move() {
  int j = make_move(get_good_depth(), -1, 1).second.second;
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
  if (find_win_streak() == 'X') {
    cout << "First player won.\n";
  } else if (find_win_streak() == 'O') {
    cout << "Second player won.\n";
  } else {
    cout << "Draw.\n";
  }
}

signed main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  cout.tie(NULL);
  while (true) {
    run();
  }
  return 0;
}
