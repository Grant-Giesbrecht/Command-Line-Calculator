#include <iostream>
#include <stdio.h>
#include <vector>
#include <string>

#include "KMatrix.hpp"
#include "KVar.hpp"

#ifndef KC_AUXILLIARY_HPP
#define KC_AUXILLIARY_HPP

typedef struct{
	char type;
	std::string s;
	bool b;
	KMatrix m;
	double d;
}token;

std::vector<std::string> fill_keywords();

bool add_matrix(std::vector<std::string> words, KVar& kv);

bool add_bool(std::vector<std::string> words, KVar& kv );

bool add_string(std::vector<std::string> words, KVar& kv );

bool add_double(std::vector<std::string> words, KVar& kv );

// void print_variables(vlist vars);

bool is_operator(std::string in, std::string* op);

bool valid_varname(std::string name, KVar& kv, std::string* error=NULL);

bool is_varname(std::string name, KVar& kv, std::string* error=NULL);

bool interpret(std::string in, KVar& kv, char* return_type, double* double_return, bool* bool_return, std::string* string_return, KMatrix* matrix_return, std::string* error);

#endif
