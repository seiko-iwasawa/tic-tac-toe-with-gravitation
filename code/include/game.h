#pragma once

void computer_move();

int get_human_move();

int get_tip();

void human_move();

void print_new_game();

/*
if game_type == FIRST_PLAYER, then the human plays for first player
else if game_type == SECOND_PLAYER, then the human plays for second player
else if game_type == BOTH_PLAYERS, then the human plays for both players
else if game_type == GAME_FOR_LOG, then the computer plays for both players
and this game will be logging
else if game_type == EXIT, then the program will be over
else if game_type == CHANGE_SEED, then you can be able change game seed
else the computer plays for both players
*/
void input_game_type();

void init_game_type();

void print_start_position();

void init_seed();

void change_seed();

void init_game();

void switch_player();

bool is_human_moving();

void mainloop();

void print_result();

void log_game();

void run();
