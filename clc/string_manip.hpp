#include <iostream> //Standard includes
#include <stdio.h>
#include <string>
#include <vector>

#ifndef STRING_MANIP_HPP
#define STRING_MANIP_HPP

std::vector<std::string> parse(std::string input, std::string delin);

void ensure_whitespace(std::string& in, std::string targets);

void ensure_whitespace_protected(std::string& in, std::string targets, std::vector<std::string> protected_targets);

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

char select_notation(double num, int precision, int threshold, bool force_sci, bool force_fix);

char select_notation(double num, int threshold);

std::string add_space_indicators(std::string);

std::string add_space_indicators(std::string, std::string indentation);

//From KInterpAux

bool str_to_string(std::string input, std::string& output);
//bool str_to_double(std::string input, double& output);
bool str_to_double(std::string input, double& output, std::string previous_word="");
bool str_to_double(std::string input, double& output,  std::string& error, std::string previous_word="");
bool str_to_bool(std::string input, bool& output);
bool str_to_op(std::string input);

//From CLIK

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

std::string vdtos(std::vector<double> vin, std::string delim);

#endif
