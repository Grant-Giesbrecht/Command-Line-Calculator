#ifndef KINTERPAUX_HPP
#define KINTERPAUX_HPP

#include <string>
#include <vector>
#include "KInterp.hpp"
#include "KMatrix.hpp"

bool detect_variable(std::vector<std::string> words, int begin_idx, int end_idx , KVar& kv, std::string& varname, char& type, std::string* error);

std::string char_to_typestr(char in);

bool str_to_kmatrix(std::string input, KMatrix& output);
bool str_to_string(std::string input, std::string& output);
//bool str_to_double(std::string input, double& output);
bool str_to_double(std::string input, double& output, std::string previous_word="");
bool str_to_double(std::string input, double& output,  std::string& error, std::string previous_word="");
bool str_to_bool(std::string input, bool& output);
bool str_to_op(std::string input);
#endif
