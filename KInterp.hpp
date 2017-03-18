#ifndef KINTERP_HPP
#define KINTERP_HPP

#include <string>
#include <stdio.h>

#include "KMatrix.hpp"
#include "KVar.hpp"
#include "interpret_keywords.hpp"

bool interpret(std::string input, KVar& vars, all_ktype& out, std::vector<func_id> interp_functions, bool allow_print=false);

std::vector<std::string> space_and_parse(std::string input);

void inject_words_into_strvec(std::vector<std::string>& words, std::string inject, int begin, int end);

void inject_akt_into_aktvec(std::vector<all_ktype>& words, all_ktype inject, int begin, int end);

std::string akt_tostring(all_ktype akt, bool formal=false);

bool run_interpret(std::string filename, KVar& vars, all_ktype& out, std::vector<func_id> interp_functions, bool persist, bool print_results, std::string indentation, std::vector<record_entry>& record, bool print_error, bool delete_comments=false);

std::vector<std::vector<std::string> > form_sentences(std::vector<std::string> input);

#endif
