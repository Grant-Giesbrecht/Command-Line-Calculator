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
#include "string_manip.hpp"
#include "settings_menu.hpp"
#include <unistd.h>

#define IFPRINT if (!silence_output) cout

using namespace std;

string this_filename_0 = "interpret_keywords.cpp";

void interpret_with_keywords(std::string input, KVar& kv, all_ktype& result, std::vector<func_id> functions, bool& running, vector<record_entry>& record, bool silence_output, program_settings& settings, string& in_header, string& out_header){
    
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
        if (words.size() < 1){
            continue;
        }
        
        //Scan for keywords - if non found, interpret as expression
        if (to_uppercase(words[0]) == "EXIT"){ //Exit
            running = false;
        }else if(to_uppercase(words[0]) == "HELP"){
            
            for (int i = 1 ; i < words.size() ; i++){
                if (!print_file(string(RESOURCE_DIR) + "Resources/clc_" + to_lowercase(words[i]) + "_help.txt", 1)){
                    IFPRINT << indent_line(1) << "ERROR: Failed to recognize help subtopic '" + words[i] + "'" << endl;
                }
            }
            if (words.size() == 1){
                if (!print_file(string(RESOURCE_DIR) + "Resources/clc_help.txt", 1)){
                    IFPRINT << indent_line(1) << "SOFTWARE ERROR: Failed to locate file: 'clc_help.txt'" << endl;
                    cout << string(RESOURCE_DIR) + "Resources/clc_help.txt" << endl;
                }
            }
        }else if(to_uppercase(words[0]) == "LOGO"){
            print_file(string(RESOURCE_DIR) + "Resources/CLC_logo.txt", 1);
        }else if(to_uppercase(words[0]) == "LSVAR"){
            
            bool print_comments = true;
            for (int i = 1 ; i < words.size() ; i++ ){
                if (to_uppercase(words[i]) == "-C"){
                    print_comments = true;
                }else if (to_uppercase(words[i]) == "-NC"){
                    print_comments = false;
                }else{
                    IFPRINT << indent_line(1) << "ERROR: Failed to recognize flag '" + words[i] + "'." << endl;
                }
            }
            
            if (print_comments){
                kv.set(KV_PRINT_COMMENT, true);
            }else{
                kv.set(KV_PRINT_COMMENT, false);
            }
            
            kv.print(1);
        }else if(to_uppercase(words[0]) == "CLVAR"){
            kv.clear();
            IFPRINT << indent_line(1) << "All variables cleared" << endl;
        }else if(to_uppercase(words[0]) == "CLEAR" || to_uppercase(words[0]) == "CLS"){
            system(CLEAR_COMMAND);
        }else if(to_uppercase(words[0]) == "LS" || to_uppercase(words[0]) == "DIR"){
            system(string(string(LIST_COMMAND) + " " + cat_tokens(words, 1, " ")).c_str());
        }else if(to_uppercase(words[0]) == "CD"){
            chdir(cat_tokens(words, 1, " ").c_str());
        }else if(to_uppercase(words[0]) == "HOME"){
            chdir(settings.home_dir.c_str());
        }else if(to_uppercase(words[0]) == "SAVE_DIR"){
            chdir(settings.save_dir.c_str());
        }else if(to_uppercase(words[0]) == "PWD"){
            system("pwd");
        }else if(to_uppercase(words[0]) == "SETCOM"){
            
            if (words.size() > 1){
                
                string cmt;
                IFPRINT << indent_line(1) << "NEW COMMENT: ";
                getline(cin, cmt);
                
                for (int i = 1 ; i < words.size() ; i++){
                    if (!kv.set_comment(words[i], cmt)){
                        IFPRINT << indent_line(1) << "ERROR: Failed to set comment of variable '" + words[i] + "'" << endl;
                    }
                }
            }else{
                IFPRINT << indent_line(1) << "VARIABLE: ";
                string usr_input;
                getline(cin, usr_input);
                string cmt;
                IFPRINT << indent_line(1) << "NEW COMMENT: ";
                getline(cin, cmt);
                vector<string> del_vars = parse(usr_input, " ");
                for (int i = 0 ; i < del_vars.size() ; i++){
                    if (!kv.set_comment(del_vars[i], cmt)){
                        IFPRINT << indent_line(1) << "ERROR: Failed to set comment of variable '" + words[i] + "'" << endl;
                    }
                }
            }
            
        }else if(to_uppercase(words[0]) == "ABE"){
            print_file(string(RESOURCE_DIR) + "Resources/Abe_Lincoln.txt", 0);
        }else if(to_uppercase(words[0]) == "VIEW"){
            KVar viewer;
            viewer.set(KV_OVERWRITE_ON_LOAD, true);
            viewer.set(KV_PRINT_THRESHOLD, settings.threshold);
            viewer.set(KV_PRINT_PRECISION, (double)settings.precision);
            
            string fn = "";
            bool read_comment = true;
            bool verbatim = false;
            int file_type = 1; //1 = KV1, 2 = KV2, 3 = KV3
            for (int i = 1 ; i < words.size() ; i++ ){
                if (to_uppercase(words[i]) == "-KV1"){
                    file_type = 1;
                }else if(to_uppercase(words[i]) == "-KV2"){
                    file_type = 2;
                }else if(to_uppercase(words[i]) == "-KV3"){
                    file_type = 3;
                }else if(to_uppercase(words[i]) == "-V"){
                    verbatim = true;
                }else if(to_uppercase(words[i]) == "-NC"){
                    read_comment = false;
                }else if(to_uppercase(words[i]) == "-C"){
                    read_comment = true;
                }else if(to_uppercase(words[i]) == "-NV"){
                    verbatim = false;
                }else if(words[i][0] != '-'){
                    fn = fn + words[i];
                }else{
                    IFPRINT << indent_line(1) << "ERROR: Failed to recognize flag '" + words[i] + "'." << endl;
                }
            }
            
            for (int i = 0 ; i < fn.length() ; i++){
                if (fn[i] == ' ' || fn[i] == '\t'){
                    fn = fn.substr(0, i) + fn.substr(i+1);
                }
            }
            
            if (fn.length() == 0){
                IFPRINT << indent_line(1) << "FILE: ";
                getline(cin, fn);
            }
            
            if (read_comment){
                viewer.set(KV_READ_COMMENTS, true);
            }else{
                viewer.set(KV_READ_COMMENTS, false);
            }
            
            //Add KV1, 2, or 3 extension if !verbatim and no extension
            string extension = "";
            if (!verbatim && fn.find('.') == string::npos){
                KVar kv_test;
                if (kv_test.load_KV1(fn+".KV1")){
                    extension = ".KV1";
                    fn = fn + extension;
                }else if (kv_test.load_KV1(fn+".KV2")){
                    extension = ".KV2";
                    fn = fn + extension;
                }else if(kv_test.load_KV1(fn+".KV3")){
                    extension = ".KV3";
                    fn = fn + extension;
                }
            }
            
            long fail_line;
            switch (file_type) {
                case 1:
                    if (!viewer.load_KV1(fn, &fail_line)){
                        IFPRINT << indent_line(1) << "ERROR: Failed to load file: '" + fn + "'.\n" << indent_line(2) << "Failed on line " << to_string(fail_line) << '.' << endl;
                    }else if(extension != ""){
                        IFPRINT << indent_line(1) << "File '" + fn + "' read successfully." << endl;
                    }
                    break;
                case 2:
                    if (!viewer.load_KV1(fn, &fail_line)){
                        IFPRINT << indent_line(1) << "ERROR: Failed to load file: '" + fn + "'.\n" << indent_line(2) << "Failed on line " << to_string(fail_line) << '.' << endl;
                    }else if(extension != ""){
                        IFPRINT << indent_line(1) << "File '" + fn + "' read successfully." << endl;
                    }
                    break;
                case 3:
                    if (!viewer.load_KV1(fn, &fail_line)){
                        IFPRINT << indent_line(1) << "ERROR: Failed to load file: '" + fn + "'.\n" << indent_line(2) << "Failed on line " << to_string(fail_line) << '.' << endl;
                    }else if(extension != ""){
                        IFPRINT << indent_line(1) << "File '" + fn + "' read successfully." << endl;
                    }
                    break;
                default:
                    IFPRINT << indent_line(1) << "SOFTWARE ERROR: Unrecognized filetype detected." << endl;
                    break;
            }
            string header = viewer.get_header();
            if (header.length() > 0) IFPRINT << indent_line(1) << "HEADER: \n" << indent_in_string(header, 2) << endl;
            viewer.print(1);
            
        }else if(to_uppercase(words[0]) == "LOAD"){
            
            kv.set(KV_OVERWRITE_ON_LOAD, settings.overwrite_on_load);
            
            string fn = "";
            bool load_comments = true;
            bool verbatim = false;
            int file_type = 1; //1 = KV1, 2 = KV2, 3 = KV3
            for (int i = 1 ; i < words.size() ; i++ ){
                if (to_uppercase(words[i]) == "-KV1"){
                    file_type = 1;
                }else if(to_uppercase(words[i]) == "-KV2"){
                    file_type = 2;
                }else if(to_uppercase(words[i]) == "-KV3"){
                    file_type = 3;
                }else if(words[i][0] != '-'){
                    fn = fn + words[i];
                }else if(to_uppercase(words[i]) == "-V"){
                    verbatim = true;
                }else if(to_uppercase(words[i]) == "-NV"){
                    verbatim = false;
                }else if(to_uppercase(words[i]) == "-NC"){
                    load_comments = false;
                }else if(to_uppercase(words[i]) == "-C"){
                    load_comments = true;
                }else if(to_uppercase(words[i]) == "-SCI" || to_uppercase(words[i]) == "-SCIENTIFIC"){
                    fn = string(RESOURCE_DIR) + "Resources/scientific.kv1";
                }else if(to_uppercase(words[i]) == "-STD" || to_uppercase(words[i]) == "-STANDARD"){
                    fn = string(RESOURCE_DIR) + "Resources/standard.kv1";
                }else{
                    IFPRINT << indent_line(1) << "ERROR: Failed to recognize flag '" + words[i] + "'." << endl;
                }
            }
            
            for (int i = 0 ; i < fn.length() ; i++){
                if (fn[i] == ' ' || fn[i] == '\t'){
                    fn = fn.substr(0, i) + fn.substr(i+1);
                }
            }
            
            if (fn.length() == 0){
                IFPRINT << indent_line(1) << "FILE: ";
                getline(cin, fn);
            }
            
            if (load_comments){
                kv.set(KV_READ_COMMENTS, true);
            }else{
                kv.set(KV_READ_COMMENTS, false);
            }
            
            //Add KV1, 2, or 3 extension if !verbatim and no extension
            string extension = "";
            if (!verbatim && fn.find('.') == string::npos){
                KVar kv_test;
                if (kv_test.load_KV1(fn+".KV1")){
                    extension = ".KV1";
                    fn = fn + extension;
                }else if (kv_test.load_KV1(fn+".KV2")){
                    extension = ".KV2";
                    fn = fn + extension;
                }else if(kv_test.load_KV1(fn+".KV3")){
                    extension = ".KV3";
                    fn = fn + extension;
                }
            }
            
            long fail_line;
            switch (file_type) {
                case 1:
                    if (!kv.load_KV1(fn, &fail_line)){
                        IFPRINT << indent_line(1) << "ERROR: Failed to load file: '" + fn + "'.\n" << indent_line(2) << "Failed on line " << to_string(fail_line) << '.' << endl;
                    }else if(extension != ""){
                        in_header = kv.get_header();
                        IFPRINT << indent_line(1) << "File '" + fn + "' read successfully." << endl;
                    }else{
                        in_header = kv.get_header();
                    }
                    break;
                case 2:
                    if (!kv.load_KV1(fn, &fail_line)){
                        IFPRINT << indent_line(1) << "ERROR: Failed to load file: '" + fn + "'.\n" << indent_line(2) << "Failed on line " << to_string(fail_line) << '.' << endl;
                    }else if(extension != ""){
                        in_header = kv.get_header();
                        IFPRINT << indent_line(1) << "File '" + fn + "' read successfully." << endl;
                    }else{
                        in_header = kv.get_header();
                    }
                    break;
                case 3:
                    if (!kv.load_KV1(fn, &fail_line)){
                        IFPRINT << indent_line(1) << "ERROR: Failed to load file: '" + fn + "'.\n" << indent_line(2) << "Failed on line " << to_string(fail_line) << '.' << endl;
                    }else if(extension != ""){
                        in_header = kv.get_header();
                        IFPRINT << indent_line(1) << "File '" + fn + "' read successfully." << endl;
                    }else{
                        in_header = kv.get_header();
                    }
                    break;
                default:
                    IFPRINT << indent_line(1) << "SOFTWARE ERROR: Unrecognized filetype detected." << endl;
                    break;
            }
            
        }else if(to_uppercase(words[0]) == "SAVE"){
            
            string fn = "";
            bool save_comments = true;
            bool append_save_dir = false;
            bool append_home_dir = false;
            int file_type = 1; //1 = KV1, 2 = KV2, 3 = KV3
            kv.set_header(out_header);
            for (int i = 1 ; i < words.size() ; i++ ){
                if (to_uppercase(words[i]) == "-KV1"){
                    file_type = 1;
                }else if(to_uppercase(words[i]) == "-KV2"){
                    file_type = 2;
                }else if(to_uppercase(words[i]) == "-KV3"){
                    file_type = 3;
                }else if(to_uppercase(words[i]) == "-SD" || to_uppercase(words[i]) == "-SAVE_DIR"){
                    append_save_dir = true;
                    append_home_dir = false;
                }else if(to_uppercase(words[i]) == "-H" || to_uppercase(words[i]) == "-HOME_DIR" || to_uppercase(words[i]) == "-HOME" ){
                    append_save_dir = false;
                    append_home_dir = true;
                }else if(to_uppercase(words[i]) == "-C"){
                    save_comments = true;
                }else if(to_uppercase(words[i]) == "-NC"){
                    save_comments = false;
                }else if(words[i][0] != '-'){
                    fn = fn + words[i];
                }else{
                    IFPRINT << indent_line(1) << "ERROR: Failed to recognize flag '" + words[i] + "'." << endl;
                }
            }
            
            for (int i = 0 ; i < fn.length() ; i++){
                if (fn[i] == ' ' || fn[i] == '\t'){
                    fn = fn.substr(0, i) + fn.substr(i+1);
                }
            }
            
            if (fn.length() == 0){
                IFPRINT << indent_line(1) << "FILE: ";
                getline(cin, fn);
            }
            
            if (append_save_dir){
                fn = settings.save_dir + fn;
            }else if(append_home_dir){
                fn = settings.home_dir + fn;
            }
            
            if (save_comments){
                kv.set(KV_SAVE_COMMENTS, true);
            }else{
                kv.set(KV_SAVE_COMMENTS, false);
            }
            
            switch(file_type){
                case 1:
                    if (kv.write_KV1(fn)){
                        IFPRINT << indent_line(1) << "File '" + fn + "' write successful." << endl;
                    }else{
                        IFPRINT << indent_line(1) << "ERROR: File '" + fn + "' write failed!" << endl;
                    }
                    break;
                case 2:
                    if (kv.write_KV2(fn)){
                        IFPRINT << indent_line(1) << "File '" + fn + "' write successful." << endl;
                    }else{
                        IFPRINT << indent_line(1) << "ERROR: File '" + fn + "' write failed!" << endl;
                    }
                    break;
                case 3:
                    if (kv.write_KV3(fn)){
                        IFPRINT << indent_line(1) << "File '" + fn + "' write successful." << endl;
                    }else{
                        IFPRINT << indent_line(1) << "ERROR: File '" + fn + "' write failed!" << endl;
                    }
                    break;
                default:
                    IFPRINT << indent_line(1) << "SOFTWARE ERROR: Unrecognized filetype detected." << endl;
                    break;
            }
        }else if(to_uppercase(words[0]) == "VHDR"){
            if (in_header != ""){
                IFPRINT << indent_line(1) << "READ HEADER: " << endl;
                IFPRINT << indent_in_string(in_header, 2) << endl;
            }
            if (out_header != ""){
                IFPRINT << indent_line(1) << "WRITE HEADER: " << endl;
                IFPRINT << indent_in_string(out_header, 2) << endl;
            }
        }else if(to_uppercase(words[0]) == "SHDR"){
            
            string hdr = "";
            bool copy_in_header = false;
            for (int i = 1 ; i < words.size() ; i++ ){
                if (to_uppercase(words[i]) == "-IN"){
                    copy_in_header = true;
//                }else if(words[i][0] != '-'){
//                    hdr = hdr + words[i];
                }else{
                    IFPRINT << indent_line(1) << "ERROR: Failed to recognize flag '" + words[i] + "'." << endl;
                }
            }
            
            if (copy_in_header){
                out_header = in_header;
            }
            
            if (hdr.length() == 0){
                IFPRINT << indent_line(1) << "HEADER: ";
                getline(cin, hdr);
            }
            
            out_header = hdr;
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
                    fn = fn + words[i];
                }else{
                    IFPRINT << "ERROR: Unrecognized flag '" + words[i] + "'";
                }
            }
            
            for (int i = 0 ; i < fn.length() ; i++){
                if (fn[i] == ' ' || fn[i] == '\t'){
                    fn = fn.substr(0, i) + fn.substr(i+1);
                }
            }
            
            if (fn == ""){
                IFPRINT << indent_line(1) << "FILE: ";
                getline(cin, fn);
            }
            
            if (!run_interpret(fn, kv, result, functions, persist, (!silence), indent_line(1), record, true, settings, in_header, out_header)){
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
            bool print_errors = false;
            for (int i = 1 ; i < words.size() ; i++){
                if (to_uppercase(words[i]) == "-B"){
                    count_start_beginning = true;
                }else if(to_uppercase(words[i]) == "-NB"){
                    count_start_beginning = false;
                }else if(to_uppercase(words[i]) == "-A"){
                    print_errors = true;
                }else if(to_uppercase(words[i]) == "-NA"){
                    print_errors = false;
                }else if(words[i][0] != '-'){
                    if (!str_to_double(words[i], count)){
                        IFPRINT << indent_line(1) << "ERROR: Failed to interpret '" + words[i] + "' as number";
                        return;
                    }
                    count = (int)count;
                }else{
                    
                }
            }
            
//            unsigned long start = 0;
//            unsigned long end = record.size();
//            if (count != -1){
//                if (count_start_beginning){
//                    start = 0;
//                    end = count;
//                }else{
//                    start = record.size() - count;
//                    end = record.size();
//                }
//                if (end > record.size()){
//                    end = record.size();
//                }
//            }
            
            int start = 0;
            int end = (int)record.size()-1;
            int ticker = 0;
            if (count != -1){
                if (print_errors){
                    if (count_start_beginning){
                        start = 0;
                        end = count -1;
                    }else{
                        start = record.size()-count;
                        end = (int)record.size()-1;
                    }
                    if (start < 0){
                        start = 0;
                    }
                    if (end > record.size()-1){
                        end = (int)record.size()-1;
                    }
                }else{
                    if (count_start_beginning){
                        for (int i = 0 ; i < record.size() ; i++){
                            if (record[i].output.type != 'e' ){
                                ticker++;
                                if (ticker == count){
                                    end = i;
                                    break;
                                }
                            }
                        }
                    }else{
                        for (int i = (int)record.size()-1 ; i >= 0 ; i--){
                            if (record[i].output.type != 'e'){
                                ticker++;
                                if (ticker == count){
                                    start = i;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            
            int num_printed = 0;
            for (int i = start ; i <= end ; i++){
//                if (count != -1 && num_printed >= count){
//                    break;
//                }
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
                    num_printed++;
                }
            }
        
        }else if(to_uppercase(words[0]) == "SET"){
            run_settings_menu(settings);
            kv.set(KV_PRINT_THRESHOLD, settings.threshold);
            kv.set(KV_PRINT_PRECISION, (double)settings.precision);
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
            string filename = "record.rcd";
            for (int i = 1 ; i < words.size() ; i++){
                if (to_uppercase(words[i]) == "-F"){
                    if (words.size() - 1 == i) continue;
                    filename = words[i+1];
                }else if(to_uppercase(words[i]) == "-H"){
                    if (filename.length() < settings.home_dir.length() || filename.substr(0, settings.home_dir.length()) != settings.home_dir){
                        if (settings.home_dir[settings.home_dir.length()-1] != '/'){
                            filename = settings.home_dir + '/' + filename;
                        }else{
                            filename = settings.home_dir + filename;
                        }
                    }
                }else if(to_uppercase(words[i]) == "-S"){
                    if (filename.length() < settings.save_dir.length() || filename.substr(0, settings.save_dir.length()) != settings.save_dir){
                        if (settings.save_dir[settings.save_dir.length()-1] != '/'){
                            filename = settings.save_dir + '/' + filename;
                        }else{
                            filename = settings.save_dir + filename;
                        }
                    }
                }else{
                    
                }
            }
            save_record(filename, record, settings);
        }else{  //If not keyword, evaluate expression
            //Evaluate expression
            string feed_string = cat_tokens(words, 0, " ");
//            ensure_whitespace(feed_string, "-");
            if (!interpret(feed_string, kv, result, functions, true)){
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
                IFPRINT << indent_line(1) << akt_tostring(result, settings.precision, settings.threshold) << endl;
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

bool save_record(std::string filename, std::vector<record_entry> record, program_settings settings){
    
    fstream file;
    file.open(filename, std::ofstream::out | std::ofstream::trunc);
    if (!file.is_open()){
        return false;
    }
    
    //Save date and time if asked
    if (settings.svrcd_lcltm){
        time_t now = time(0);
        char* dt = ctime(&now);
        file << "LOCAL: " << dt;
    }
    if (settings.svrcd_utctm){
        time_t now = time(0);
        tm *gmtm = gmtime(&now);
        char* dt = asctime(gmtm);
        file << "UTC: "<< dt;
    }
    if (settings.svrcd_utctm || settings.svrcd_lcltm){
        file << endl;
    }
    
    for (int i = 0 ; i < record.size() ; i++){
        file << record[i].command << endl;
        file << "\t" << akt_tostring(record[i].output, settings.precision, settings.threshold) << endl;
    }
    
    file.close();
    
    return true;
}

bool save_program(std::string filename, std::vector<record_entry> record, program_settings settings){
    
    fstream file;
    file.open(filename, std::ofstream::out | std::ofstream::trunc);
    if (!file.is_open()){
        return false;
    }
    
    return true;
}











