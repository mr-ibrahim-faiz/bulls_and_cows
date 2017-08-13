#pragma once

#include "network.h"

#include<vector>
using std::vector;

// Bulls_and_cows functions

// converts char to size_t
size_t char_to_size_t(const char&);

// checks if a vector has unique elements
template<typename T>
bool has_unique_elements(T);

// gets a random vector of size_t with n unique elements
vector<size_t> get_random_vector(const size_t&);

// counts bulls and cows
void count_bulls_and_cows(const vector<size_t>&,
	const vector<size_t>&, size_t&, size_t&);

// display main menu
void display_glbl_menu();

// display network menu
void display_ntwk_menu();

// ask player to enter a number of digits to guess
void asks_number_of_digits();

// gets number of digits to guess
size_t gets_number_of_digits();


// Data transmission functions

// send size_t number
void send_size_t_number(const SOCKET&, const size_t&);

// recieve size_t number
size_t recieve_size_t_number(const SOCKET&);

// sends vector of size_t
void send_size_t_vector(const SOCKET&, const vector<size_t>&);

// recieves vector of size_t
vector<size_t> recieve_size_t_vector(const SOCKET&);


// Main functions

// core of game
int game_core(const vector<size_t>&, size_t&, size_t&);

// command line version of bulls and cows game offline
int bullsandcowsc_offline();

// core of game network
int network_game_core(const SOCKET&, const vector<size_t>&);

// command line version of bulls and cows game network
int bullsandcowsc_network();