#ifndef KINTERP_HPP
#define KINTERP_HPP

#include <string>
#include <stdio.h>

#include "KMatrix.hpp"
#include "KVar.hpp"
#include "interpret_keywords.hpp"

bool interpret(std::string input, KVar& vars, all_ktype& out, std::vector<func_id> interp_functions, bool allow_print=false);

std::vector<std::string> space_and_parse(std::string input);

std::vector<std::string> space_and_parse_protected(std::string input);

std::vector<std::string> space_and_parse_negatives(std::string input);

bool is_negative_restricted_character(char c); //Just intended for use in space_and_parse for sytaxtual clarity

void inject_words_into_strvec(std::vector<std::string>& words, std::string inject, int begin, int end);

void inject_akt_into_aktvec(std::vector<all_ktype>& words, all_ktype inject, int begin, int end);

std::string akt_tostring(all_ktype akt, int precision, int threshold);

std::string akt_tostring(all_ktype akt, int precision, int threshold, char notation);

std::string akt_tostring(all_ktype akt, bool formal=false);

//bool run_interpret(std::string filename, KVar& vars, all_ktype& out, std::vector<func_id> interp_functions, bool persist, bool print_results, std::string indentation, std::vector<record_entry>& record, bool delete_comments=false, int print_precision=15, bool scientific_notation=false);

bool run_interpret(std::string filename, KVar& vars, all_ktype& out, std::vector<func_id> interp_functions, bool persist, bool print_results, std::string indentation, std::vector<record_entry>& record, bool delete_comments, program_settings settings, std::string& in_header, std::string& out_header, std::vector<int>& fail_lines, std::vector<std::string>& fail_messages);

bool run_interpret(std::string filename, KVar& vars, all_ktype& out, std::vector<func_id> interp_functions, bool persist, bool print_results, std::string indentation, std::vector<record_entry>& record, bool delete_comments, program_settings settings, std::string& in_header, std::string& out_header);

std::vector<std::vector<std::string> > form_sentences(std::vector<std::string> input);

#endif
