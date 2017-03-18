//
//  interpret_keywords.cpp
//  clc
//
//  Created by Grant Giesbrecht on 3/15/17.
//  Copyright © 2017 Grant Giesbrecht. All rights reserved.
//

#include "interpret_keywords.hpp"
#include <stdio.h>
#include <iostream>
#include "KInterp.hpp"
#include "KInterpAux.hpp"

#define IFPRINT if (!silence_output) cout

using namespace std;

string this_filename_0 = "interpret_keywords.cpp";



void interpret_with_keywords(std::string input, KVar& kv, all_ktype& result, std::vector<func_id> functions, bool& running, vector<record_entry>& record, bool print_errors, bool silence_output){
    
    record_entry temp_rcd;
    
    //Skip blank lines (KInterp will not break, just return error stating no content. Unneccesary though).
    if (input.length() == 0){
        return;
    }
    
    //Parse words - not evaluation quality parse, just to determine if keyword active or if interpretation is required
    vector<string> words_complete = space_and_parse(input);
    vector<string> words;
    vector<vector<string> > sentences = form_sentences(words_complete);
    
    for (int j = 0 ; j < sentences.size() ; j++){
    
        words = sentences[j];
        
        //Scan for keywords - if non found, interpret as expression
        if (to_uppercase(words[0]) == "EXIT"){ //Exit
            running = false;
        }else if(to_uppercase(words[0]) == "HELP"){
            
            for (int i = 1 ; i < words.size() ; i++){
                if (!print_file("Resources/clc_" + to_lowercase(words[i]) + "_help.txt", 1)){
                    IFPRINT << indent_line(1) << "ERROR: Failed to recognize help subtopic '" + words[i] + "'" << endl;
                }
            }
            if (words.size() == 1){
                if (!print_file("Resources/clc_help.txt", 1)){
                    IFPRINT << indent_line(1) << "SOFTWARE ERROR: Failed to locate file: 'clc_help.txt'" << endl;
                }
            }
        }else if(to_uppercase(words[0]) == "LSVAR"){
            kv.print(1);
        }else if(to_uppercase(words[0]) == "CLVAR"){
            kv.clear();
            IFPRINT << indent_line(1) << "All variables cleared" << endl;
        }else if((words[0]) == "clear" || (words[0]) == "cls"){
            system(CLEAR_COMMAND);
        }else if((words[0]) == "ls" || words[0] == "dir"){
            system(LIST_COMMAND);
        }else if(to_uppercase(words[0]) == "LOAD"){
            IFPRINT << indent_line(1) << "FILE: ";
            string fn;
            getline(cin, fn);
            if (!kv.load_file(fn)){
                IFPRINT << indent_line(1) << "ERROR: Failed to load file: '" + fn + "'";
            }else{
                IFPRINT << indent_line(1) << "File '" + fn + "' read successfully." << endl;
            }
        }else if(to_uppercase(words[0]) == "SAVE"){
            IFPRINT << "FILE: ";
            string fn;
            getline(cin, fn);
            if (kv.write_file(fn)){
                IFPRINT << "File '" + fn + "' write successful." << endl;
            }
        }else if(to_uppercase(words[0]) == "RUN"){
            bool silence = true;
            string fn = "";
            bool persist = false;
            
            for (int i = 1 ; i < words.size() ; i++){
                if (to_uppercase(words[i]) == "-S"){
                    silence = true;
                }else if(to_uppercase(words[i]) == "-NS"){
                    silence = false;
                }else if(to_uppercase(words[i]) == "-P"){
                    persist = true;
                }else if(to_uppercase(words[i]) == "-NP"){
                    persist = false;
                }else if(words[i][0] != '-'){
                    fn = words[i];
                }else{
                    IFPRINT << "ERROR: Unrecognized flag '" + words[i] + "'";
                }
            }
            
            if (fn == ""){
                IFPRINT << indent_line(1) << "FILE: ";
                getline(cin, fn);
            }
            
            if (!run_interpret(fn, kv, result, functions, persist, (!silence), indent_line(1), record, print_errors, true)){
                if (result.type == 'e'){
                    IFPRINT << indent_line(1) << result.s << endl;
                }else{
                    IFPRINT << indent_line(1) << "SOFTWARE ERROR: Undefined type returned by interpret(). Problem detected in interpret() function. Check file 'KInterp.cpp'" << endl;
                    IFPRINT << indent_line(1) << "\t";
                    try{
                        IFPRINT << result.s << endl;
                    }catch(...){}
                }
            }else{
                kv.delete_var("ans");
                switch(result.type){
                    case 'd':
                        kv.add_double("ans", result.d);
                        break;
                    case 'm':
                        kv.add_matrix("ans", result.km);
                        break;
                    case 'b':
                        kv.add_bool("ans", result.b);
                        break;
                    case 's':
                        kv.add_string("ans", result.s);
                        break;
                    case 'e':
                        IFPRINT << indent_line(1) << "SOFTWARE ERROR: Failed interpret was not caught." << result.s << endl;
                        break;
                    default:
                        IFPRINT << "SOFTWARE ERROR: Failed to identify type when updating variable 'ans'. Error in file '" + this_filename_0 + "'" << endl;
                        kv.add_double("ans", -1);
                        break;
                }
            }
        }else if(to_uppercase(words[0]) == "SVPRG"){
        }else if(to_uppercase(words[0]) == "DELETE"){
            if (words.size() > 1){
                for (int i = 1 ; i < words.size() ; i++){
                    if (!kv.delete_var(words[i])){
                        IFPRINT << indent_line(1) << "ERROR: Failed to delete variable '" + words[i] + "'" << endl;
                    }
                }
            }else{
                IFPRINT << indent_line(1) << "VARIABLE: ";
                string usr_input;
                getline(cin, usr_input);
                vector<string> del_vars = parse(usr_input, " ");
                for (int i = 0 ; i < del_vars.size() ; i++){
                    if (!kv.delete_var(del_vars[i])){
                        IFPRINT << indent_line(1) << "ERROR: Failed to delete variable '" + words[i] + "'" << endl;
                    }
                }
            }
        }else if(to_uppercase(words[0]) == "PRCD"){
            
            bool count_start_beginning = false;
            double count = -1;
            for (int i = 1 ; i < words.size() ; i++){
                if (to_uppercase(words[i]) == "-B"){
                    count_start_beginning = true;
                }else if(words[i][0] != '-'){
                    if (!str_to_double(words[i], count)){
                        IFPRINT << indent_line(1) << "ERROR: Failed to interpret '" + words[i] + "' as number";
                        return;
                    }
                    count = (int)count;
                }else{
                    
                }
            }
            
            unsigned long start = 0;
            unsigned long end = record.size();
            if (count != -1){
                if (count_start_beginning){
                    start = 0;
                    end = count;
                }else{
                    start = record.size() - count;
                    end = record.size();
                }
                if (end > record.size()){
                    end = record.size();
                }
            }
            
            for (unsigned long i = start ; i < end ; i++){
                if (print_errors || record[i].output.type != 'e'){
                    IFPRINT << indent_line(1) << '[' << to_string(i) << "] " << record[i].command << endl;
                    switch (record[i].output.type) {
                        case 'd':
                            IFPRINT << indent_line(2) << to_string(record[i].output.d) << endl;
                            break;
                        case 'b':
                            IFPRINT << indent_line(2) << bool_to_str(record[i].output.b) << endl;
                            break;
                        case 'm':
                            IFPRINT << indent_line(2) << record[i].output.km.get_string() << endl;
                            break;
                        case 's':
                            IFPRINT << indent_line(2) << record[i].output.s << endl;
                            break;
                        case 'e':
                            IFPRINT << indent_line(2) << record[i].output.s << endl;
                            break;
                        default:
                            IFPRINT << "SOFTWARE ERROR: Unrecognized retrun type" << endl;
                            break;
                    }
                }
            }
        }else if(to_uppercase(words[0]) == "RELIGION"){
            kv.add_bool("religion", false);
            IFPRINT << indent_line(1) << bool_to_str(false) << endl;
        }else if(to_uppercase(words[0]) == "MATLAB"){
            kv.add_string("MATLAB", "Spawn of satan");
            IFPRINT << indent_line(1) << "Spawn of satan" << endl;
        }else if(to_uppercase(words[0]) == "CLRCD"){
            record.clear();
            IFPRINT << indent_line(1) << "Record cleared." << endl;
        }else if(to_uppercase(words[0]) == "SVRCD"){
            //Save record as text file
        }else{  //If not keyword, evaluate expression
            //Evaluate expression
            if (!interpret(cat_tokens(words, 0, " "), kv, result, functions, true)){
                if (result.type == 'e'){
                    IFPRINT << indent_line(1) << result.s << endl;
                }else{
                    IFPRINT << indent_line(1) << "SOFTWARE ERROR: Undefined type returned by interpret(). Problem detected in interpret() function. Check file 'KInterp.cpp'" << endl;
                    IFPRINT << indent_line(1) << "\t";
                    try{
                        IFPRINT << result.s << endl;
                    }catch(...){}
                }
            }else{
                IFPRINT << indent_line(1) << akt_tostring(result, true) << endl;
                kv.delete_var("ans");
                switch(result.type){
                    case 'd':
                        kv.add_double("ans", result.d);
                        break;
                    case 'm':
                        kv.add_matrix("ans", result.km);
                        break;
                    case 'b':
                        kv.add_bool("ans", result.b);
                        break;
                    case 's':
                        kv.add_string("ans", result.s);
                        break;
                    case 'e':
                        IFPRINT << indent_line(1) << "SOFTWARE ERROR: Failed interpret was not caught." << result.s << endl;
                        break;
                    default:
                        IFPRINT << "SOFTWARE ERROR: Failed to identify type when updating variable 'ans'. Error in file '" + this_filename_0 + "'" << endl;
                        kv.add_double("ans", -1);
                        break;
                }
            }
            temp_rcd.command = input;
            temp_rcd.output = result;
            record.push_back(temp_rcd);
        }
    }
}

