#include <config.h>
#include <data-base.h>

std::map<std::pair<ull, ull>, std::vector<int>> data_base;

void load_data_position(std::ifstream& fin) {
  ull X_mask, O_mask;
  fin >> X_mask >> O_mask;
  std::vector<int> losses(M);
  for (int i = 0; i < M; ++i) {
    fin >> losses[i];
  }
  data_base[{X_mask, O_mask}] = losses;
}

void load_data_base() {
  std::ifstream fin(DATA_BASE_FILENAME);
  while (!fin.eof()) {
    load_data_position(fin);
  }
  fin.close();
}

void save_data_position(std::ofstream& fout,
                        std::pair<std::pair<ull, ull>, std::vector<int>> data) {
  fout << data.first.first << ' ' << data.first.second << ' ';
  for (auto x : data.second) {
    fout << x << ' ';
  }
  fout << '\n';
}

void save_data_base() {
  std::ofstream fout("new-" + DATA_BASE_FILENAME);
  for (auto e : data_base) {
    save_data_position(fout, e);
  }
  fout.close();
}
