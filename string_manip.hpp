#include <iostream> //Standard includes
#include <stdio.h>
#include <string>
#include <vector>

#ifndef STRING_MANIP_HPP
#define STRING_MANIP_HPP

std::vector<std::string> parse(std::string input, std::string delin);

void ensure_whitespace(std::string& in, std::string targets);

void ensure_whitespace_full(std::string& in, std::string multichar_target);

void remove_end_whitespace(std::string& io);

bool isnum(std::string s);

void remove_comments(std::string& s, std::string commentor);

void remove_from_end(std::string& s, std::string targets);

std::string cat_tokens(std::vector<std::string> t, int idx, std::string joint);

std::string to_uppercase(std::string s);

std::string to_lowercase(std::string s);

std::string bool_to_str(bool x, bool uppercase=false);

std::string format_newline(std::string input, std::string prefix);

std::string hp_string(double input, int precision=15, bool scientific=false);

double strtod(std::string input, bool* success=NULL);

void select_notation(double num, int precision, int threshold, bool force_sci, bool force_fix);

char select_notation(double num, int threshold);

#endif
