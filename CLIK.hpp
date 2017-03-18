#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

#ifndef CLIK_HPP
#define CLIK_HPP

std::string next_string(std::string in, bool& success, int start_idx, int* last_idx=NULL, bool allow_before=false);

std::string next_phrase(std::string in, bool& success, int start_idx, char starter, char ender, int* last_idx=NULL, bool allow_before=false);

bool next_string(std::vector<std::string> in, std::string* value, int start_word, int* last_word);

bool next_phrase(std::vector<std::string> in, std::string* value, char starter, char ender, int start_word, int* last_word);

bool next_double(std::string in, double* value);

bool next_bool(std::string in, bool* value);

#endif
