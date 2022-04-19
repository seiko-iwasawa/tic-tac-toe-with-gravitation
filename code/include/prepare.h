#pragma once

#include <config.h>

extern int streak_number;
extern std::vector<int> streak[N][M];

void build_streak(int i, int j, int dx, int dy);

void build_streak(int i, int j);

void build_streak();

extern int mask_leader[1 << M];

int shift_mask(int mask, int j);

void build_mask_leader();

void prepare();
