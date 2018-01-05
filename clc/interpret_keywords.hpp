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
    std::string description;
    std::string argdesc;
    std::string retdesc;
}func_id;

typedef struct{
    std::string command;
    all_ktype output;
}record_entry;

typedef struct{
    int precision; //Number of decimal places to show
    double threshold; //Threshold to shift from fixed notation to scientific notation
    std::string command_sequence; //Set the command sequence (thing printed to prompt for commands)
    bool force_sci; //Force all output to be in scientific notation
    bool force_fixed; //Force all output to be in fixed form (not scientific notation)
    bool svrcd_lcltm; //Add local time to save records
    bool svrcd_utctm; //Add UTC to saved records
    std::string home_dir; //Path to home directory
    std::string save_dir; //Path to save directory
    bool hide_startup_sequence; //Hide startup sequence from record
    bool print_dir_cd; //Print new directory when cd executed
    bool overwrite_on_load; //Overwrite variables when loaded w/ same name
    bool save_input_history; //Save keyboard input history
    int input_history_length; //Number of entries allowed to be stored in the input history (-1 for unlimited)
}program_settings;

void interpret_with_keywords(std::string input, KVar& kv, all_ktype& result, std::vector<func_id> functions, bool& running, std::vector<record_entry>& record, bool silence_output, program_settings& settings, std::string& in_header, std::string& out_header);

bool save_record(std::string filename, std::vector<record_entry> record, program_settings settings);

bool save_program(std::string filename, std::vector<record_entry> record, program_settings settings);

#endif /* interpret_keywords_hpp */
