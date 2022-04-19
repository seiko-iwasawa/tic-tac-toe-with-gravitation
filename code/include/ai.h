#pragma once

#include <config.h>
#include <vector>

int make_move(int depth, int alpha, int beta);

int make_move(int depth, std::vector<int>& good_first_moves);

ull get_mask(char c, int shift);

ull get_maskX(int shift);

ull get_maskO(int shift);

int get_shift();

int get_the_best_move();