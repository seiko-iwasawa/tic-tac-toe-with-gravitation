#pragma once

#include "config.h"

const string DATA_BASE_FILENAME = "data-base.txt";

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
