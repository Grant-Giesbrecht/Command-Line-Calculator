//
//  settings_menu.cpp
//  clc
//
//  Created by Grant Giesbrecht on 4/3/17.
//  Copyright © 2017 Grant Giesbrecht. All rights reserved.
//

#include "settings_menu.hpp"
#include <iostream>
#include <stdio.h>
#include "KInterpAux.hpp"

#define IFPRINTERR if(true)

using namespace std;

void run_settings_menu(program_settings& settings){
    
    cout << "\t=========================================================" << endl;
    cout << "\t=================== CLC SETTINGS MENU ===================" << endl;
    cout << "\t=========================================================" << endl << endl;
    show_settings(settings);
    cout << endl << indent_line(1) << "Enter settings number to adjust. Type 'Exit' to quit" << endl;
    
    string input;
    while (true){
        
        cout << indent_line(1) << "> " << flush;
        getline(cin, input);
        vector<string> words = parse(input, " ");
        if (words.size() == 0) continue; //Skip if just carriage return
        string upin = to_uppercase(words[0]);
        
        if (upin == "EXIT"){
            break;
        }else if(upin == "VIEW"){
            show_settings(settings);
        }else if(upin == "HELP"){
            print_file(string(RESOURCE_DIR) + "Resources/settings_menu_help.txt", 1);
        }else if(upin == "1" || upin == "PRECISION"){
            
            //Get new value
            if (words.size() > 1){
                input = words[1];
            }else{
                cout << indent_line(2) << "New value (integer): " << flush;
                getline(cin, input);
            }
            
            //Interpret, check, and apply new value
            try{
                double nv = stod(input);
                if (nv <= 0 || (int)nv != nv ){
                    cout << indent_line(2) << "ERROR: New value must be an integer greater than zero" << endl;
                }
                settings.precision = nv;
            }catch(...){
                cout << indent_line(2) << "ERROR: New value must be an integer greater than zero" << endl;
            }
            
        }else if(upin == "2" || upin == "THRESHOLD"){
            
            //Get new value
            if (words.size() > 1){
                input = words[1];
            }else{
                cout << indent_line(2) << "New value (double): " << flush;
                getline(cin, input);
            }
            
            //Interpret, check, and apply new value
            try{
                double nv = stod(input);
                if (nv < 0){
                    cout << indent_line(2) << "ERROR: New value must be a positive number" << endl;
                }
                settings.threshold = nv;
            }catch(...){
                cout << indent_line(2) << "ERROR: New value must be a positive number" << endl;
            }
            
        }else if(upin == "3" || upin == "COMMAND_SEQUENCE"){
            
            //Get new value
            if (words.size() > 1){
                bool success;
                int ignore;
                input = next_string(input, success, 0, &ignore, true);
                if (!success){
                    cout << "ERROR: New value must be a string (surrounded by double quotes)" << endl;
                }
            }else{
                cout << indent_line(2) << "New value (string): " << flush;
                getline(cin, input);
                bool success;
                input = next_string(input, success, 0);
                if (!success){
                    cout << "ERROR: New value must be a string (surrounded by double quotes)" << endl;
                }
            }
            
            //Interpret, check, and apply new value
            settings.command_sequence = input;
        }else if(upin == "8" || upin == "HOME_DIR"){
            
            //Get new value
            if (words.size() > 1){
                bool success;
                int ignore;
                input = next_string(input, success, 0, &ignore, true);
                if (!success){
                    cout << "ERROR: New value must be a string (surrounded by double quotes)" << endl;
                }
            }else{
                cout << indent_line(2) << "New value (string): " << flush;
                getline(cin, input);
                bool success;
                input = next_string(input, success, 0);
                if (!success){
                    cout << "ERROR: New value must be a string (surrounded by double quotes)" << endl;
                }
            }
            
            //Add '/' to directory if not there
            if (input[input.length()-1] != '\\' && input[input.length()-1] != '/'){
                input = input + '/';
            }
            
            //Interpret, check, and apply new value
            settings.home_dir = input;
            
        }else if(upin == "9" || upin == "SAVE_DIR"){
            
            //Get new value
            if (words.size() > 1){
                bool success;
                int ignore;
                input = next_string(input, success, 0, &ignore, true);
                if (!success){
                    cout << "ERROR: New value must be a string (surrounded by double quotes)" << endl;
                }
            }else{
                cout << indent_line(2) << "New value (string): " << flush;
                getline(cin, input);
                bool success;
                input = next_string(input, success, 0);
                if (!success){
                    cout << "ERROR: New value must be a string (surrounded by double quotes)" << endl;
                }
            }
            
            //Add '/' to directory if not there
            if (input[input.length()-1] != '\\' && input[input.length()-1] != '/'){
                input = input + '/';
            }
            
            //Interpret, check, and apply new value
            settings.save_dir = input;
            
        }else if(upin == "4" || upin == "FORCE_SCI"){
            bool out;
            if (words.size() > 1){
                str_to_bool(words[1], out);
            }else{
                cout << indent_line(2) << "New value (bool): " << flush;
                getline(cin, input);
                str_to_bool(input, out);
            }
            settings.force_sci = out;
        }else if(upin == "5" || upin == "FORCE_FIX"){
            bool out;
            if (words.size() > 1){
                str_to_bool(words[1], out);
            }else{
                cout << indent_line(2) << "New value (bool): " << flush;
                getline(cin, input);
                str_to_bool(input, out);
            }
            settings.force_fixed = out;
        }else if(upin == "6" || upin == "RCD_LOCAL"){
            bool out;
            if (words.size() > 1){
                str_to_bool(words[1], out);
            }else{
                cout << indent_line(2) << "New value (bool): " << flush;
                getline(cin, input);
                str_to_bool(input, out);
            }
            settings.svrcd_lcltm = out;
        }else if(upin == "7" || upin == "RCD_UTC"){
            bool out;
            if (words.size() > 1){
                str_to_bool(words[1], out);
            }else{
                cout << indent_line(2) << "New value (bool): " << flush;
                getline(cin, input);
                str_to_bool(input, out);
            }
            settings.svrcd_utctm = out;
        }else if(upin == "10" || upin == "HIDE_STARTUP"){
            bool out;
            if (words.size() > 1){
                str_to_bool(words[1], out);
            }else{
                cout << indent_line(2) << "New value (bool): " << flush;
                getline(cin, input);
                str_to_bool(input, out);
            }
            settings.hide_startup_sequence = out;
        }else if(upin == "11" || upin == "PRINT_DIR_CD"){
            bool out;
            if (words.size() > 1){
                str_to_bool(words[1], out);
            }else{
                cout << indent_line(2) << "New value (bool): " << flush;
                getline(cin, input);
                str_to_bool(input, out);
            }
            settings.print_dir_cd = out;
        }else if(upin == "PRINT_CODES"){
            
        }else if(upin == "SAVE"){
            save_settings(string(RESOURCE_DIR) + "Resources/program_settings.txt", settings);
        }else{
            cout << indent_line(2) << "Unrecognized setting. Enter 'help' for instructions." << endl;
        }
        
        
    }
    
    
    
    
}

void show_settings(program_settings settings){
    cout << "\t Settings: " << endl;
    cout << indent_line(2) << "1. Precision: \t" << settings.precision << endl;
    cout << indent_line(2) << "2. Threshold: \t" << settings.threshold << endl;
    cout << indent_line(2) << "3. Command Sequence: \t'" << settings.command_sequence << "'" << endl;
    cout << indent_line(2) << "4. Force Scientific Notation: \t" << bool_to_str(settings.force_sci) << endl;
    cout << indent_line(2) << "5. Force Fixed Point Notation: \t" << bool_to_str(settings.force_fixed) << endl;
    cout << indent_line(2) << "6. Save Local Time in Record: \t" << bool_to_str(settings.svrcd_lcltm) << endl;
    cout << indent_line(2) << "7. Save UTC (Time) in Record: \t" << bool_to_str(settings.svrcd_utctm) << endl;
    cout << indent_line(2) << "8. Home Directory: \t'" << settings.home_dir << "'" << endl;
    cout << indent_line(2) << "9. Save Directory: \t'" << settings.save_dir << "'" << endl;
    cout << indent_line(2) << "10. Hide Startup Sequence: \t" << bool_to_str(settings.hide_startup_sequence) << endl;
    cout << indent_line(2) << "11. Print directory after cd: \t" << bool_to_str(settings.print_dir_cd) << endl;
}

bool load_settings(std::string filename, program_settings& settings){
        
    ifstream filev(filename);
    if (!filev.is_open()){
        return false;
    }
    
    string s;
    vector<string> words;
    while (filev.good()){
        getline(filev, s);
        words = parse(s, " ");
        
        if (words.size() < 1){
            continue;
        }
        
        if (words[0] == "PRECISION"){
            try{
                settings.precision = stod(words[1]);
            }catch(...){
                IFPRINTERR cout << "SOFTWARE ERROR: Failed to interpret value for setting '" << words[0] << "'" << endl;
            }
        }else if(words[0] == "THRESHOLD"){
            try{
                settings.threshold = stod(words[1]);
            }catch(...){
                IFPRINTERR cout << "SOFTWARE ERROR: Failed to interpret value for setting '" << words[0] << "'" << endl;
            }
        }else if(words[0] == "COMMAND_SEQUENCE"){
            if (s.length() < 18){
                continue;
            }
            settings.command_sequence = s.substr(17);
        }else if(words[0] == "HOME_DIR"){
            if (s.length() < 10){
                continue;
            }
            settings.home_dir = s.substr(9);
        }else if(words[0] == "SAVE_DIR"){
            if (s.length() < 10){
                continue;
            }
            settings.save_dir = s.substr(9);
        }else if(words[0] == "FORCE_SCI"){
            bool out;
            if (!str_to_bool(words[1], out)){
                IFPRINTERR cout << "SOFTWARE ERROR: Failed to interpret value for setting '" << words[0] << "'" << endl;
            }
            settings.force_sci = out;
        }else if(words[0] == "FORCE_FIX"){
            bool out;
            if (!str_to_bool(words[1], out)){
                IFPRINTERR cout << "SOFTWARE ERROR: Failed to interpret value for setting '" << words[0] << "'" << endl;
            }
            settings.force_fixed = out;
        }else if(words[0] == "RCD_LOCAL"){
            bool out;
            if (!str_to_bool(words[1], out)){
                IFPRINTERR cout << "SOFTWARE ERROR: Failed to interpret value for setting '" << words[0] << "'" << endl;
            }
            settings.svrcd_lcltm = out;
        }else if(words[0] == "RCD_UTC"){
            bool out;
            if (!str_to_bool(words[1], out)){
                IFPRINTERR cout << "SOFTWARE ERROR: Failed to interpret value for setting '" << words[0] << "'" << endl;
            }
            settings.svrcd_utctm = out;
        }else if(words[0] == "HIDE_STARTUP"){
            bool out;
            if (!str_to_bool(words[1], out)){
                IFPRINTERR cout << "SOFTWARE ERROR: Failed to interpret value for setting '" << words[0] << "'" << endl;
            }
            settings.hide_startup_sequence = out;
        }else if(words[0] == "PRINT_DIR_CD"){
            bool out;
            if (!str_to_bool(words[1], out)){
                IFPRINTERR cout << "SOFTWARE ERROR: Failed to interpret value for setting '" << words[0] << "'" << endl;
            }
            settings.print_dir_cd = out;
        }else{
            /*IFPRINTERR*/ cout << "SOFTWARE ERROR: Failed to recognize setting '" << words[0] << "'" << endl;
        }
        
    }
    
    filev.close();
    
    return true;
}

bool save_settings(std::string filename, program_settings& settings){
    
    fstream file;
    file.open(filename, std::ofstream::out | std::ofstream::trunc);
    if (!file.is_open()){
        return false;
    }
    
    file << "PRECISION " << hp_string(settings.precision, 15, true) << endl;
    file << "THRESHOLD " << hp_string(settings.threshold, 15, true) << endl;
    file << "COMMAND_SEQUENCE " << settings.command_sequence << endl;
    file << "FORCE_SCI " << bool_to_str(settings.force_sci) << endl;
    file << "FORCE_FIX " << bool_to_str(settings.force_fixed) << endl;
    file << "RCD_LOCAL " << bool_to_str(settings.svrcd_lcltm) << endl;
    file << "RCD_UTC " << bool_to_str(settings.svrcd_utctm) << endl;
    file << "HOME_DIR " << settings.home_dir << endl;
    file << "SAVE_DIR " << settings.save_dir << endl;
    file << "HIDE_STARTUP " << bool_to_str(settings.hide_startup_sequence) << endl;
    file << "PRINT_DIR_CD " << bool_to_str(settings.print_dir_cd) << endl;
    
    file.close();
    
    return true;
}



