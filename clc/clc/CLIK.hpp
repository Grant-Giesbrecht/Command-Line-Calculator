#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

#ifndef CLIK_HPP
#define CLIK_HPP

//index_from_word_idx() definitions
#define POSITION_WORD_START 0
#define POSITION_WORD_END 1

std::string next_string(std::string in, bool& success, int start_idx, int* last_idx=NULL, bool allow_before=false);

std::string next_phrase(std::string in, bool& success, int start_idx, char starter, char ender, int* last_idx=NULL, bool allow_before=false);

bool next_string(std::vector<std::string> in, std::string* value, int start_word, int* last_word);

bool next_phrase(std::vector<std::string> in, std::string in_full, std::string* value, char starter, char ender, int start_word, int* last_word, int* last_index, bool allow_before=false);

bool next_phrase(std::vector<std::string> in, std::string* value, char starter, char ender, int start_word, int* last_word);

bool next_double(std::string in, double* value);

bool next_bool(std::string in, bool* value);

int idx_from_word_idx(std::string str_in, std::vector<std::string> words_in, int word_idx, int position_on_word, int minimum_idx=0);

int word_idx_from_idx(std::string str_in, std::vector<std::string> words_in, int str_idx, int start_idx=0);

#endif
