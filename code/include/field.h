#pragma once

#include <config.h>

extern char field[N][M];
extern std::vector<std::vector<int>> history;
extern std::vector<int> x_streak_sum, o_streak_sum;
extern ull X_mask, O_mask;
extern int rate_delta;
extern bool have_win_streak;

void init_field();

void print_numeration();

void print_field();

void print_position();

bool is_filled();

char get_streak();

bool is_end();

char get_turn_order();

const int KEK = 5;

void add(int i, int j, int d, char move);

int get_move_place(int j);

void move(int j);

void undo();

int get_rate();

char get_result();

bool is_draw();

bool is_X();

bool is_O();
