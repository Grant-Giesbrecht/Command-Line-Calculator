#ifndef KINTERPAUX_HPP
#define KINTERPAUX_HPP

#include <string>
#include <vector>
#include "KInterp.hpp"
#include "KMatrix.hpp"

bool detect_variable(std::vector<std::string> words, int begin_idx, int end_idx , KVar& kv, std::string& varname, char& type, std::string* error);

std::string char_to_typestr(char in);
#endif
