#pragma once

#include <algorithm>
#include <cassert>
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
const int DELTA_LOSSES = 0;

const int DX[4] = {0, +1, +1, +1};
const int DY[4] = {+1, +1, 0, -1};

const int O_WIN = (int)-1e9;
const int DRAW = 0;
const int X_WIN = (int)1e9;

const int FIRST_PLAYER = 1;
const int SECOND_PLAYER = 2;
const int BOTH_PLAYERS = 3;
const int GAME_FOR_LOG = 4;
const int EXIT = 5;

const int TIP = 0;
