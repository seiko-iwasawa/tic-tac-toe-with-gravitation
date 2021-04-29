#pragma once

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
  if (get_turn_order() == 'X') {
    reverse(options.begin(), options.end());
    if (rec_mem.count({X_mask, O_mask})) {
      alpha = rec_mem[{X_mask, O_mask}];
    }
    if (alpha >= beta) {
      return alpha;
    }
    int res = alpha;
    for (auto e : options) {
      if (e != options[0] && e.first <= alpha) {
        // break;
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
    if (alpha >= beta) {
      return beta;
    }
    int res = beta;
    for (auto e : options) {
      if (e != options[0] && e.first >= beta) {
        // break;
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

int make_move(int depth, vector<int> &good_first_moves) {
  vector<int> options;
  for (int j : good_first_moves) {
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
  if (get_turn_order() == 'X') {
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

ull get_mask(char c, int shift) {
  ull res = 0;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < M; ++j) {
      res |= (ull)(field[i][j] == c) << (i * M + (j - shift + M) % M);
    }
  }
  return res;
}

ull get_maskX(int shift) { return get_mask('X', shift); }

ull get_maskO(int shift) { return get_mask('O', shift); }

const int INF = (int)1e9 + 7;

int get_shift() {
  int mask = 0;
  for (int j = 0; j < M; ++j) {
    mask |= (field[N - 1][j] == 'X') << j;
  }
  return mask_leader[mask];
}

int get_the_best_move() {
  int shift = get_shift();
  vector<int> &cur = data_base[{get_maskX(shift), get_maskO(shift)}];
  if (cur.empty()) {
    cur.resize(M);
    for (int j = 0; j < M; ++j) {
      if (field[0][j] != '.') {
        cur[(j - shift + M) % M] = INF;
      }
    }
  }
  int min_n_losses = INF;
  for (int j = 0; j < M; ++j) {
    if (cur[j] != INF) {
      min_n_losses = min(min_n_losses, cur[j]);
    }
  }
  vector<int> good_first_moves;
  for (int j = 0; j < M; ++j) {
    if (cur[j] <= min_n_losses + DELTA_LOSSES) {
      good_first_moves.push_back((j + shift) % M);
    }
  }
  int j;
  int depth = 6;
  flag_break_cycle = false;
  int start = clock();
  while (!flag_break_cycle && depth <= N * M - (int)history.size() &&
         clock() - start <= MAX_REC_TIME) {
    start = clock();
    cout << "CUR IT #" << depth << ":\n";
    j = make_move(depth, good_first_moves);
    cout << "\tMOVE: " << j + 1 << '\n';
    cout << "\tTIME: " << clock() - start << '\n';
    ++depth;
  }
  return j;
}
