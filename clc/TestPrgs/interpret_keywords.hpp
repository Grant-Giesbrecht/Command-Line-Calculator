//
//  interpret_keywords.hpp
//  clc
//
//  Created by Grant Giesbrecht on 3/15/17.
//  Copyright © 2017 Grant Giesbrecht. All rights reserved.
//

#ifndef interpret_keywords_hpp
#define interpret_keywords_hpp

#include "configuration.hpp"

#include <stdio.h>
#include <string>
#include <vector>
#include "KMatrix.hpp"
#include "KVar.hpp"
#include <ctime>

typedef struct{
    char type; //'d' - double, 'm' - matrix, 'b' - bool, 's' - string, 'e' - error (message in string 's')
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

typedef struct{
    int precision;
    double threshold;
    std::string command_sequence;
    bool force_sci;
    bool force_fixed;
    bool svrcd_lcltm;
    bool svrcd_utctm;
    std::string home_dir;
    std::string save_dir;
    bool hide_startup_sequence;
    bool print_dir_cd;
    bool overwrite_on_load;
}program_settings;

void interpret_with_keywords(std::string input, KVar& kv, all_ktype& result, std::vector<func_id> functions, bool& running, std::vector<record_entry>& record, bool silence_output, program_settings& settings, std::string& in_header, std::string& out_header);

bool save_record(std::string filename, std::vector<record_entry> record, program_settings settings);

bool save_program(std::string filename, std::vector<record_entry> record, program_settings settings);

#endif /* interpret_keywords_hpp */
