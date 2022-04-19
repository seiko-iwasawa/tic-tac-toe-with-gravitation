#pragma once

#include <config.h>

const std::string DATA_BASE_FILENAME = "data-base.txt";

extern std::map<std::pair<ull, ull>, std::vector<int>>
    data_base;  // stores the number of losses for each move in the position

void load_data_position(std::ifstream& fin);

void load_data_base();

void save_data_position(std::ofstream& fout,
                        std::pair<std::pair<ull, ull>, std::vector<int>> data);

void save_data_base();
