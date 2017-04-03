//
//  main.cpp
//  clc
//
//  Created by Grant Giesbrecht on 3/6/17.
//  Copyright © 2017 Grant Giesbrecht. All rights reserved.
//

#include <iostream>
#include <stdio.h>
#include <string>

#include "KInterp.hpp"
#include "KVar.hpp"
#include "KMatrix.hpp"
#include "string_manip.hpp"
#include "KIFunctions.hpp"
#include "interpret_keywords.hpp"

using namespace std;

string this_filename_1 = "main.cpp";

/*
 + ans variable
 + implied ans (KInterp action)
 + save and read KV files
 - KV2 & KV3 I/O
 - fix strings (in parse, protect whitespace in strings by guaranteeing 2 things: 1.) all whitespace is preserved 2.) all double quotes are first character in their word (if whitespace permits - don't split where otherwise wouldn't).
 + program execution - allow user to run programs and allow startup program to run
 - save record
 - save specific variables to file
 + VIEW command - view contents of file without loading variables
 - precision representation
 + precision in loading
 + semicolon processing
 - update help files for help archives
 - add logos and eggs
 - add comment manipulation
 - add header manipulation and viewing
 - add settings
    - display comments
    - default file format (for storage)
    - directories
 
 */

int main(int argc, const char * argv[]){
    
    KVar kv;
    kv.add_double("ans", 0);
    string command_sequence = "> ";
    all_ktype result;
    
    double print_precision = 5;
    double threshold = 1e5;
    bool force_sci = false;
    bool force_fixed = false;
    
    vector<func_id> functions;
    define_functions(&functions);
    
    vector<record_entry> record;
    
    run_interpret("Resources/startup.clc", kv, result, functions, true, false, "", record, true, print_precision, threshold, force_sci, force_fixed);
    record.clear();
    
    bool running = true;
    string input;
    record_entry temp_rcd;
    all_ktype temp_akt;
    while (running){
        
        //Get input
        cout << command_sequence << flush;
        getline(cin, input);
        
        interpret_with_keywords(input, kv, result, functions, running, record, false, print_precision, threshold, force_sci, force_fixed);

    }
    
    return 0;
}












