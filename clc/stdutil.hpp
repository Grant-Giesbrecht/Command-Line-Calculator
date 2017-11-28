#include <iostream> //Standard includes
#include <stdio.h>
#include <string>
#include <vector>

#ifndef STDUTIL_HPP
#define STDUTIL_HPP

void choose_scientific(double in, double threshold, double precision);

bool print_file(std::string filename, int tabs);

void print_vector(std::vector<std::vector<double> > v, int indent=0, bool spaces=false);

std::string indent_line(int indentation, bool use_spaces=false, bool indent_in_function=false);

std::string indent_in_string(std::string, int indentation, bool use_spaces=false);

int strvec_contains(std::vector<std::string> vec, std::string target);

#endif
