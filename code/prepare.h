#pragma once

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

int mask_leader[1 << M];

int shift_mask(int mask, int j) {
  int res = 0;
  for (int i = 0; i < M; ++i) {
    res |= (mask >> i & 1) << (i + j) % M;
  }
  return res;
}

void build_mask_leader() {
  for (int mask = 1; mask < (1 << N); ++mask) {
    if (mask_leader[mask] == 0) {
      for (int j = 0; j < M; ++j) {
        mask_leader[shift_mask(mask, j)] = j;
      }
    }
  }
}

void prepare() {
  // load_data_base();
  build_streak();
  build_mask_leader();
}
