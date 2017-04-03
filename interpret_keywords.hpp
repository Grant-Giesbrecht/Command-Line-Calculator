//
//  interpret_keywords.hpp
//  clc
//
//  Created by Grant Giesbrecht on 3/15/17.
//  Copyright © 2017 Grant Giesbrecht. All rights reserved.
//

#ifndef interpret_keywords_hpp
#define interpret_keywords_hpp

#ifdef _WIN32
#define CLEAR_COMMAND "cls"
#define LIST_COMMAND "dir"
#define SYSTEM_OS "MS Windows"
#else
#define CLEAR_COMMAND "clear"
#define LIST_COMMAND "ls"
#endif

#ifdef __APPLE__
#define SYSTEM_OS "MacOS"
#elif __linux__
#define SYSTEM_OS "LINUX"
#elif __unix__
#define SYSTEM_OS "UNIX"
#endif

#include <stdio.h>
#include <string>
#include <vector>
#include "KMatrix.hpp"
#include "KVar.hpp"

typedef struct{
    char type;
    std::string s;
    KMatrix km;
    bool b;
    double d;
}all_ktype;

typedef struct{
    all_ktype (*function)(std::vector<all_ktype>);
    std::string identifier;
}func_id;

typedef struct{
    std::string command;
    all_ktype output;
}record_entry;

void interpret_with_keywords(std::string input, KVar& kv, all_ktype& result, std::vector<func_id> functions, bool& running, std::vector<record_entry>& record, bool silence_output, int print_precision, int threshold, bool force_scientific, bool force_fixed);

//void interpret_with_keywords(std::string input, KVar& kv, all_ktype& result, std::vector<func_id> functions, bool& running, std::vector<record_entry>& record, bool silence_output, int print_precision, bool scientific_notation);

#endif /* interpret_keywords_hpp */
