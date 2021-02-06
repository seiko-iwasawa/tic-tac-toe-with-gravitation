#include <algorithm>
#include <ctime>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>

using namespace std;

typedef unsigned long long ull;

const int N = 6;
const int M = 7;
const int WIN_STREAK = 4;
const int PREGAMES = 0;
const int MAX_REC_TIME = 5000;
const int MANY_LOSSES = 10;  // if the position had at least MANY_LOSSES losses,
                             // position is considered learned
const string DATA_BASE_FILENAME = "data-base.txt";

const int DX[4] = {0, +1, +1, +1};
const int DY[4] = {+1, +1, 0, -1};

map<pair<ull, ull>, vector<int>>
    data_base;  // stores the number of losses for each move in the position

void load_data_position(ifstream &fin) {
  ull X_mask, O_mask;
  fin >> X_mask >> O_mask;
  vector<int> losses(M);
  for (int i = 0; i < M; ++i) {
    fin >> losses[i];
  }
  data_base[{X_mask, O_mask}] = losses;
}

void load_data_base() {
  ifstream fin(DATA_BASE_FILENAME);
  while (!fin.eof()) {
    load_data_position(fin);
  }
  fin.close();
}

void save_data_position(ofstream &fout,
                        pair<pair<ull, ull>, vector<int>> data) {
  fout << data.first.first << ' ' << data.first.second << ' ';
  for (auto x : data.second) {
    fout << x << ' ';
  }
  fout << '\n';
}

void save_data_base() {
  ofstream fout("new-" + DATA_BASE_FILENAME);
  for (auto e : data_base) {
    save_data_position(fout, e);
  }
  fout.close();
}

vector<int> x_streak_sum, o_streak_sum;
int streak_number = 0;
vector<int> streak[N][M];

void build_streak(int i, int j, int dx, int dy) {
  for (int k = 0; k < WIN_STREAK; ++k) {
    streak[(i + k * dx + N) % N][(j + k * dy + M) % M].push_back(streak_number);
  }
  ++streak_number;
}

void build_streak(int i, int j) {
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

void prepare() {
  load_data_base();
  build_streak();
}

char field[N][M];
vector<vector<int>> history;
vector<int> streak_type;
ull X_mask, O_mask;
int rate_delta;
bool have_win_streak;

void init_field() {
  X_mask = O_mask = 0;
  have_win_streak = false;
  x_streak_sum.resize(streak_number, 0);
  o_streak_sum.resize(streak_number, 0);
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
  int i = get_move_place(j);
  field[i][j] = get_move_type();
  if (field[i][j] == 'X') {
    X_mask ^= 1LL << i * M + j;
  } else {
    O_mask ^= 1LL << i * M + j;
  }
  add(i, j, 1, field[i][j]);
  history.push_back({i, j});
}

void undo() {
  int i = history.back()[0];
  int j = history.back()[1];
  history.pop_back();
  if (field[i][j] == 'X') {
    X_mask ^= 1LL << i * M + j;
  } else {
    O_mask ^= 1LL << i * M + j;
  }
  add(i, j, -1, field[i][j]);
  field[i][j] = '.';
}

ull get_mask(char c) {
  ull res = 0;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < M; ++j) {
      res |= (ull)(field[i][j] == c) << (i * M + j);
    }
  }
  return res;
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

map<pair<ull, ull>, int> rec_mem;

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
    if (rec_mem.count({X_mask, O_mask})) {
      alpha = rec_mem[{X_mask, O_mask}];
    }
    int res = alpha;
    for (auto e : options) {
      if (e != options[0] && e.first <= alpha) {
        break;
      }
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
    return rec_mem[{X_mask, O_mask}] = res;
  } else {
    if (rec_mem.count({X_mask, O_mask})) {
      beta = rec_mem[{X_mask, O_mask}];
    }
    int res = beta;
    for (auto e : options) {
      if (e != options[0] && e.first >= beta) {
        break;
      }
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
    return rec_mem[{X_mask, O_mask}] = res;
  }
}

bool flag_break_cycle;

int make_move(int depth) {
  vector<int> options;
  for (int j = 0; j < M; ++j) {
    if (field[0][j] == '.') {
      move(j);
      if (is_end()) {
        undo();
        flag_break_cycle = true;
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
      rec_mem.clear();
      int nxt = make_move(depth, O_WIN, X_WIN);
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
      rec_mem.clear();
      int nxt = make_move(depth, O_WIN, X_WIN);
      undo();
      if (res > nxt) {
        res = nxt;
        bj = j;
      }
    }
  }
  cout << "\tMOVE RATE: " << res << '\n';
  if (res < O_WIN + 100 || res > X_WIN - 100) {
    flag_break_cycle = true;
  }
  return bj;
}

ull get_maskX() { return get_mask('X'); }

ull get_maskO() { return get_mask('O'); }

const int INF = (int)1e9 + 7;

int get_the_best_move() {
  vector<int> &cur = data_base[{get_maskX(), get_maskO()}];
  if (cur.empty()) {
    cur.resize(M);
    for (int j = 0; j < M; ++j) {
      if (field[0][j] != '.') {
        cur[j] = INF;
      }
    }
  }
  int s = 0;
  for (int j = 0; j < M; ++j) {
    if (cur[j] != INF) {
      s += cur[j];
    }
  }
  if (s > MANY_LOSSES) {
    int res = 0;
    for (int j = 0; j < M; ++j) {
      if (cur[res] > cur[j]) {
        res = j;
      } else if (cur[res] == cur[j] && rand() % 2) {
        res = j;
      }
    }
    return res;
  } else {
    int j;
    int depth = 1;
    flag_break_cycle = false;
    int start = clock();
    while (!flag_break_cycle && depth <= 100 &&
           clock() - start <= MAX_REC_TIME) {
      start = clock();
      cout << "CUR IT #" << depth << ":\n";
      j = make_move(depth);
      cout << "\tMOVE: " << j + 1 << '\n';
      cout << "\tTIME: " << clock() - start << '\n';
      ++depth;
    }
    return j;
  }
}

void computer_move() {
  int j = get_the_best_move();
  cout << "Computer move: " << j + 1 << '\n';
  move(j);
}

void human_move() {
  cout << "Human move: ";
  int j;
  cin >> j;
  --j;
  if (j == -1) {
    j = get_the_best_move();
    cout << "Tip: " << j + 1 << '\n';
  }
  move(j);
}

void print_new_game() { cout << "===NEW GAME===\n"; }

const int FIRST_PLAYER = 1;
const int SECOND_PLAYER = 2;
const int BOTH_PLAYERS = 3;
const int GAME_FOR_LOG = 4;
const int EXIT = 5;
int human_player;

/*
if human_player == FRIST_PLAYER, then the human plays for first player
else if human_player == SECOND_PLAYER, then the human plays for second player
else if human_player == BOTH_PLAYERS, then the human plays for both players
else if human_player == GAME_FOR_LOG, then the computer plays for both players
and this game will be logging
else if human_player == EXIT, then the program will be over
else the computer plays for both players
*/
void input_human_player() {
  static int it = 0;
  ++it;
  if (it < PREGAMES) {
    human_player = GAME_FOR_LOG;
  } else {
    cout << "Human player: ";
    cin >> human_player;
    if (human_player == EXIT) {
      save_data_base();
      exit(0);
    }
  }
}

void print_start_position() { print_position(); }

void init_new_game() {
  print_new_game();
  input_human_player();
  init_field();
  print_start_position();
}

int player;

void switch_player() {
  player = (player == FIRST_PLAYER ? SECOND_PLAYER : FIRST_PLAYER);
}

bool is_human_moving() { return player == human_player || human_player == 3; }

void run_game_mainloop() {
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
  if (human_player != GAME_FOR_LOG) {
    return;
  }
  bool flag = (get_streak() == '.');
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < M; ++j) {
      field[i][j] = '.';
    }
  }
  for (int i = 0; i < history.size(); ++i) {
    vector<int> &cur = data_base[{get_maskX(), get_maskO()}];
    cur[history[i][1]] += ((history.size() - i) % 2 == 0 || flag);
    field[history[i][0]][history[i][1]] = (i % 2 == 0 ? 'X' : 'O');
  }
}

void run_game() {
  init_new_game();
  run_game_mainloop();
  print_result();
  log_game();
}

signed main() {
  ios_base::sync_with_stdio(false);
  cin.tie(NULL);
  cout.tie(NULL);
  srand((unsigned int)time(NULL));
  prepare();
  while (true) {
    run_game();
  }
  return 0;
}
