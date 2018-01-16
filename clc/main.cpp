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
#include <unistd.h>

#include "configuration.hpp"
#include "KInterp.hpp"
#include "KVar.hpp"
#include "KMatrix.hpp"
#include "string_manip.hpp"
#include "KIFunctions.hpp"
#include "interpret_keywords.hpp"
#include "settings_menu.hpp"

#include <readline/readline.h> //rl
#include <readline/history.h> //rl

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
    all_ktype result;
    
    string in_header = "";
    string out_header = "";
    
    std::string user_home_path = getenv("HOME"); //UNIX only
    if (user_home_path.length() > 0 && (user_home_path[user_home_path.length()-1] != '=')){
        user_home_path = user_home_path + "/";
    }
    
    //Load program setting from settings file
    program_settings settings{5, 1e5, "> ", false, false, true, true, user_home_path, user_home_path, true, false, true, true, -1};
    if (!load_settings(user_home_path + ".clc_settings.conf", settings)){
        cout << "STARTUP ERROR: Failed to locate settings file.";
        if (save_settings(user_home_path + ".clc_settings.conf", settings)){
            cout << " New settings file created." << endl;
        }else{
            cout << " Failed to create new settings file." << endl;
        }
        
    }
    kv.set(KV_PRINT_THRESHOLD, settings.threshold);
    kv.set(KV_PRINT_PRECISION, (double)settings.precision);
    
    //Change directory to home directory
    chdir(settings.home_dir.c_str());
    
    //Load functions
    vector<func_id> functions;
    define_functions(&functions);
    
    //Initialize the record
    vector<record_entry> record;
    
    //Load keyboard history if specified to do so
    if (settings.save_input_history) read_history(string(user_home_path + ".clc_input_history.txt").c_str()); //Read history (returns 0 on success)
    
    //Run the startup script to load any user preferences
    run_interpret(string(RESOURCE_DIR) + "Resources/startup.clc", kv, result, functions, true, false, "", record, true, settings, in_header, out_header);
    if (settings.hide_startup_sequence){
        record.clear();
        in_header = "";
        out_header = "";
    }
    
    //Begin the main loop
    bool running = true;
    string input;
    string last_input;
    record_entry temp_rcd;
    all_ktype temp_akt;
    char* in_buf = nullptr;
    while (running){
        
        in_buf = readline(settings.command_sequence.c_str()); //rl
        input = string(in_buf); //rl
        //Get input
//        cout << settings.command_sequence << flush; //!rl
//        getline(cin, input); //!rl
        for (int i = 0 ; i < input.length() ; i++){
            if (input[i] != ' ' && input[i] != '\t'){
                if (last_input != input && to_uppercase(input) != "EXIT"){
                    add_history(in_buf); //Add to history if not repeat
                }
                break;
            }
        }
        
        interpret_with_keywords(input, kv, result, functions, running, record, false, settings, in_header, out_header);

        last_input = input;
    }
    
    //Deallocate 'in_buf'
    free(in_buf);
    
    //Save keyboard history if requested
    if (settings.save_input_history) write_history(string(user_home_path + ".clc_input_history.txt").c_str());
    
    //Save settings to file
    save_settings(user_home_path + ".clc_settings.conf", settings);
    
    return 0;
}












