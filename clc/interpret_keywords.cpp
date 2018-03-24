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
#include <readline/history.h>

#define IFPRINT if (!silence_output) cout

//Help page flags
#define VERBOSE_FLAG 1
#define INTRO_FLAG 2
#define COMMAND_FLAG 4
#define FUNCTION_FLAG 8
#define TOPIC_FLAG 16
#define LCOMMANDS_FLAG 32
#define LFUNCTIONS_FLAG 64
#define LTOPICS_FLAG 128

using namespace std;

string this_filename_0 = "interpret_keywords.cpp";

void interpret_with_keywords(std::string input, KVar& kv, all_ktype& result, std::vector<func_id> functions, bool& running, vector<record_entry>& record, bool silence_output, program_settings& settings, string& in_header, string& out_header, bool case_sensitive_functions){

    record_entry temp_rcd;

    //Skip blank lines (KInterp will not break, just return error stating no content. Unneccesary though).
    if (input.length() == 0){
        return;
    }

    //Parse words - not evaluation quality parse, just to determine if keyword active or if interpretation is required
    vector<string> words_complete = space_and_parse_protected(input);
    vector<string> words;
    vector<vector<string> > sentences = form_sentences(words_complete);
    vector<string> words_uc;

//    for (int s = 0 ; s < sentences.size() ; s++){
//        cout << "Sentence[" << s << "]: " << endl;
//        for (int w = 0 ; w < sentences[s].size() ; w++){
//            cout << "\t" << sentences[s][w] << endl;
//        }
//    }

    for (int j = 0 ; j < sentences.size() ; j++){

        words = sentences[j];
        if (words.size() < 1){
            continue;
        }

        words_uc.clear();
        for (int wuc = 0; wuc < words.size() ; wuc++){
            words_uc.push_back(to_uppercase(words[wuc]));
        }

        //Scan for keywords - if non found, interpret as expression
        if (words_uc[0] == "EXIT"){ //Exit
            running = false;
        }else if(words_uc[0] == "HELP"){

            //Read command line arguments
            vector<string> pages;
            unsigned char help_flags = 0;
            unsigned char page_handler = COMMAND_FLAG | FUNCTION_FLAG | TOPIC_FLAG;
            for (int i = 1 ; i < words.size() ; i++){
                if (words_uc[i] == "-V" || words_uc[i] == "-VERBOSE"){
                    help_flags |= VERBOSE_FLAG;
                }else if(words_uc[i] == "-I" || words_uc[i] == "-INTRO"){
                    help_flags |= INTRO_FLAG;
                }else if(words_uc[i] == "-T" || words_uc[i] == "-TOPIC"){
                    page_handler = TOPIC_FLAG;
                    help_flags |= TOPIC_FLAG;
                }else if(words_uc[i] == "-C" || words_uc[i] == "-COMMAND"){
                    page_handler = COMMAND_FLAG;
                    help_flags |= COMMAND_FLAG;
                }else if(words_uc[i] == "-F" || words_uc[i] == "-FUNCTION"){
                    page_handler = FUNCTION_FLAG;
                    help_flags |= FUNCTION_FLAG;
                }else if(words_uc[i] == "-LC" || words_uc[i] == "-LISTCOMMANDS"){
                    help_flags |= LCOMMANDS_FLAG;
                }else if(words_uc[i] == "-LF" || words_uc[i] == "-LISTFUNCTIONS"){
                    help_flags |= LFUNCTIONS_FLAG;
                }else if(words_uc[i] == "-LT" || words_uc[i] == "-LISTTOPICS"){
                    help_flags |= LTOPICS_FLAG;
                }else{
                    pages.push_back(words[i]);
                }
            }
            if (help_flags == 0 && pages.size() == 0){
                help_flags = INTRO_FLAG;
            }

            //Service request
            if (help_flags & INTRO_FLAG){ //Intro page
                if (!print_file(string(RESOURCE_DIR) + "doc/clc_intro_help.txt", 1)){
                    IFPRINT << indent_line(1) << "RESOURCE ERROR: Failed to locate file: 'clc_intro_help.txt'" << endl;
                }
            }
            if (help_flags & VERBOSE_FLAG){ //Verbose page
                if (!print_file(string(RESOURCE_DIR) + "doc/clc_verbose_help.txt", 1)){
                    IFPRINT << indent_line(1) << "RESOURCE ERROR: Failed to locate file: 'clc_verbose_help.txt'" << endl;
                }
            }
            if (help_flags & LCOMMANDS_FLAG){ //Verbose page
                if (!print_file(string(RESOURCE_DIR) + "doc/clc_list_commands_help.txt", 1)){
                    IFPRINT << indent_line(1) << "RESOURCE ERROR: Failed to locate file: 'clc_list_commands_help.txt'" << endl;
                }
            }
            if (help_flags & LTOPICS_FLAG){ //Verbose page
                if (!print_file(string(RESOURCE_DIR) + "doc/clc_list_topics_help.txt", 1)){
                    IFPRINT << indent_line(1) << "RESOURCE ERROR: Failed to locate file: 'clc_list_topics_help.txt'" << endl;
                }
            }
            if (help_flags & LFUNCTIONS_FLAG){ //Verbose page
                cout << indent_line(1) << "---------------------------------------------------------" << endl;
                cout << indent_line(1) << "--------------------- CLC FUNCTIONS ---------------------" << endl;
                for (int f = 0 ; f < functions.size() ; f++){
                    cout << indent_line(1, 4) << functions[f].identifier << endl;
                }
            }
            if (page_handler & COMMAND_FLAG){ //Service help page requests
                for (int k = 0 ; k < pages.size() ; k++){
                    if (print_file(string(RESOURCE_DIR) + "doc/clc_command_" + to_lowercase(pages[k]) + "_help.txt", 1)){
                        pages.erase(pages.begin() + k); //If successful, delete page from requests
                        k--;
                    }
                }
            }
            if (page_handler & TOPIC_FLAG){ //Service help page requests
                for (int k = 0 ; k < pages.size() ; k++){
                    if (print_file(string(RESOURCE_DIR) + "doc/clc_topic_" + to_lowercase(pages[k]) + "_help.txt", 1)){
                        pages.erase(pages.begin() + k); //If successful, delete page from requests
                        k--;
                    }
                }
            }
            if (page_handler & FUNCTION_FLAG){ //Search functions
//                cout << functions.size() << endl;
                string identifier_uc = "";
                for (int k = 0 ; k < pages.size() ; k++){
                    for (int f = 0 ; f < functions.size() ; f++){
                        identifier_uc = to_uppercase(functions[f].identifier);
                        if (identifier_uc == to_uppercase(pages[k])){
                            cout << indent_line(1) << "---------------------------------------------------------" << endl << indent_line(1);
                            for (int l = 0 ; l < floor((44-functions[f].identifier.length())/2.0) ; l++){
                                cout << "-";
                            }
                            cout << " CLC " << identifier_uc << "() HELP ";
                            for (int l = 0 ; l < ceil((44-functions[f].identifier.length())/2.0) ; l++){
                                cout << "-";
                            }
                            cout << "\n\n" << indent_line(1) << "Description:\n" << indent_line(2) << tabulate_newline(functions[f].description, 1, 4) << endl;
                            cout << "\n" << indent_line(1) << "Arguments: " << tabulate_newline(functions[f].argdesc, 1, 4) << endl;
                            cout << "\n" << indent_line(1) << "Return Value:\n" << indent_line(2) << tabulate_newline(functions[f].retdesc, 1, 4) << endl;
                            pages.erase(pages.begin() + k); //If successful, delete page from requests
                            k--;
                            break;
                        }
                    }
                }
            }
            if (pages.size() > 0){
                cout << "\tIHS ERROR: Failed to locate help pages for " << pages.size() << " request";
                if (pages.size() > 1) cout << 's';
                cout << ":\n";
                for (int k = 0 ; k < pages.size() ; k++){
                    cout << "\t\t" << pages[k] << endl;
                }
            }


            //Origional help interface
            /*for (int i = 1 ; i < words.size() ; i++){
                if (!print_file(string(RESOURCE_DIR) + "Resources/clc_" + to_lowercase(words[i]) + "_help.txt", 1)){
                    IFPRINT << indent_line(1) << "ERROR: Failed to recognize help subtopic '" + words[i] + "'" << endl;
                }
            }
            if (words.size() == 1){
                if (!print_file(string(RESOURCE_DIR) + "Resources/clc_help.txt", 1)){
                    IFPRINT << indent_line(1) << "SOFTWARE ERROR: Failed to locate file: 'clc_help.txt'" << endl;
                    cout << string(RESOURCE_DIR) + "Resources/clc_help.txt" << endl;
                }
            }*/

        }else if(words_uc[0] == "LOGO"){
            print_file(string(RESOURCE_DIR) + "doc/CLC_logo.txt", 1);
        }else if(words_uc[0] == "LSVAR"){

            bool print_comments = true;
            for (int i = 1 ; i < words.size() ; i++ ){
                if (words_uc[i] == "-C"){
                    print_comments = true;
                }else if (words_uc[i] == "-NC"){
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
        }else if(words_uc[0] == "CLVAR"){
            kv.clear();
            IFPRINT << indent_line(1) << "All variables cleared" << endl;
        }else if(words_uc[0] == "CLEAR" || words_uc[0] == "CLS"){
            system(CLEAR_COMMAND);
        }else if(words_uc[0] == "LS" || words_uc[0] == "DIR"){
            system(string(string(LIST_COMMAND) + " " + cat_tokens(words, 1, " ")).c_str());
        }else if(words_uc[0] == "CD"){
            chdir(cat_tokens(words, 1, " ").c_str());
        }else if(words_uc[0] == "HOME"){
            chdir(settings.home_dir.c_str());
        }else if(words_uc[0] == "SAVE_DIR"){
            chdir(settings.save_dir.c_str());
        }else if(words_uc[0] == "PWD"){
            system("pwd");
        }else if(words_uc[0] == "SETCOM"){

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

        }else if(words_uc[0] == "ABE"){
            print_file(string(RESOURCE_DIR) + "doc/Abe_Lincoln.txt", 0);
        }else if(words_uc[0] == "VIEW"){
            KVar viewer;
            viewer.set(KV_OVERWRITE_ON_LOAD, true);
            viewer.set(KV_PRINT_THRESHOLD, settings.threshold);
            viewer.set(KV_PRINT_PRECISION, (double)settings.precision);

            string fn = "";
            bool read_comment = true;
            bool verbatim = false;
            int file_type = 1; //1 = KV1, 2 = KV2, 3 = KV3
            char append_dir = 'n'; //n = none, h = home, s = save_dir
            for (int i = 1 ; i < words.size() ; i++ ){
                if (words_uc[i] == "-KV1"){
                    file_type = 1;
                }else if(words_uc[i] == "-KV2"){
                    file_type = 2;
                }else if(words_uc[i] == "-KV3"){
                    file_type = 3;
                }else if(words[i][0] != '-'){
                    fn = fn + words[i];
                }else if(words_uc[i] == "-V"){
                    verbatim = true;
                }else if(words_uc[i] == "-NV"){
                    verbatim = false;
                }else if(words_uc[i] == "-NC"){
                    read_comment = false;
                }else if(words_uc[i] == "-C"){
                    read_comment = true;
                }else if(words_uc[i] == "-SD" || words_uc[i] == "-SAVE_DIR"){
                    append_dir = 's';
                }else if(words_uc[i] == "-H" || words_uc[i] == "-HOME"){
                    append_dir = 'h';
                }else if(words_uc[i] == "-SCI" || words_uc[i] == "-SCIENTIFIC"){
                    fn = string(RESOURCE_DIR) + "variables/scientific.kv1";
                }else if(words_uc[i] == "-STD" || words_uc[i] == "-STANDARD"){
                    fn = string(RESOURCE_DIR) + "variables/standard.kv1";
                }else if(words_uc[i] == "-CNV" || words_uc[i] == "-CONVERSION"){
                    fn = string(RESOURCE_DIR) + "variables/conversion.kv1";
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

            //Append directory if requested
            if (append_dir != 'n'){
                switch(append_dir){
                    case('h'):
                        if (settings.home_dir[settings.home_dir.length()-1] == '/'){
                            fn = settings.home_dir + fn;
                        }else{
                            fn = settings.home_dir + '/' + fn;
                        }
                        break;
                    case('s'):
                        if (settings.save_dir[settings.save_dir.length()-1] == '/'){
                            fn = settings.save_dir + fn;
                        }else{
                            fn = settings.save_dir + '/' + fn;
                        }
                        break;
                }
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

            long fail_line = 0;
            switch (file_type) {
                case 1:
                    if (!viewer.load_KV1(fn, fail_line)){
                        if (fail_line == -1){
                            IFPRINT << indent_line(1) << "ERROR: File \"" << fn << "\" not found." << endl;
                        }else{
                            IFPRINT << indent_line(1) << "ERROR: Failed to load file: '" + fn + "'.\n" << indent_line(2) << "Failed on line " << to_string(fail_line) << '.' << endl;
                        }
                    }else if(extension != ""){
                        IFPRINT << indent_line(1) << "File '" + fn + "' read successfully." << endl;
                    }
                    break;
                case 2:
                    if (!viewer.load_KV1(fn, fail_line)){
                        if (fail_line == -1){
                            IFPRINT << indent_line(1) << "ERROR: File \"" << fn << "\" not found." << endl;
                        }else{
                            IFPRINT << indent_line(1) << "ERROR: Failed to load file: '" + fn + "'.\n" << indent_line(2) << "Failed on line " << to_string(fail_line) << '.' << endl;
                        }
                    }else if(extension != ""){
                        IFPRINT << indent_line(1) << "File '" + fn + "' read successfully." << endl;
                    }
                    break;
                case 3:
                    if (!viewer.load_KV1(fn, fail_line)){
                        if (fail_line == -1){
                            IFPRINT << indent_line(1) << "ERROR: File \"" << fn << "\" not found." << endl;
                        }else{
                            IFPRINT << indent_line(1) << "ERROR: Failed to load file: '" + fn + "'.\n" << indent_line(2) << "Failed on line " << to_string(fail_line) << '.' << endl;
                        }
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

        }else if(words_uc[0] == "LOAD"){

            kv.set(KV_OVERWRITE_ON_LOAD, settings.overwrite_on_load);

            string fn = "";
            bool load_comments = true;
            bool verbatim = false;
            int file_type = 1; //1 = KV1, 2 = KV2, 3 = KV3
            char append_dir = 'n'; //n = none, h = home, s = save_dir
            for (int i = 1 ; i < words.size() ; i++ ){
                if (words_uc[i] == "-KV1"){
                    file_type = 1;
                }else if(words_uc[i] == "-KV2"){
                    file_type = 2;
                }else if(words_uc[i] == "-KV3"){
                    file_type = 3;
                }else if(words[i][0] != '-'){
                    fn = fn + words[i];
                }else if(words_uc[i] == "-V"){
                    verbatim = true;
                }else if(words_uc[i] == "-NV"){
                    verbatim = false;
                }else if(words_uc[i] == "-NC"){
                    load_comments = false;
                }else if(words_uc[i] == "-C"){
                    load_comments = true;
                }else if(words_uc[i] == "-C"){
                    load_comments = true;
                }else if(words_uc[i] == "-SD" || words_uc[i] == "-SAVE_DIR"){
                    append_dir = 's';
                }else if(words_uc[i] == "-H" || words_uc[i] == "-HOME"){
                    append_dir = 'h';
                }else if(words_uc[i] == "-SCI" || words_uc[i] == "-SCIENTIFIC"){
                    fn = string(RESOURCE_DIR) + "variables/scientific.kv1";
                }else if(words_uc[i] == "-STD" || words_uc[i] == "-STANDARD"){
                    fn = string(RESOURCE_DIR) + "variables/standard.kv1";
                }else if(words_uc[i] == "-CNV" || words_uc[i] == "-CONVERSION"){
                    fn = string(RESOURCE_DIR) + "variables/conversion.kv1";
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

            //Append directory if requested
            if (append_dir != 'n'){
                switch(append_dir){
                    case('h'):
                        if (settings.home_dir[settings.home_dir.length()-1] == '/'){
                            fn = settings.home_dir + fn;
                        }else{
                            fn = settings.home_dir + '/' + fn;
                        }
                        break;
                    case('s'):
                        if (settings.save_dir[settings.save_dir.length()-1] == '/'){
                            fn = settings.save_dir + fn;
                        }else{
                            fn = settings.save_dir + '/' + fn;
                        }
                        break;
                }
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
                    if (!kv.load_KV1(fn, fail_line)){
                        if (fail_line == -1){
                            IFPRINT << indent_line(1) << "ERROR: File \"" << fn << "\" not found." << endl;
                        }else{
                            IFPRINT << indent_line(1) << "ERROR: Failed to load file: '" + fn + "'.\n" << indent_line(2) << "Failed on line " << to_string(fail_line) << '.' << endl;
                        }
                    }else if(extension != ""){
                        in_header = kv.get_header();
                        IFPRINT << indent_line(1) << "File '" + fn + "' read successfully." << endl;
                    }else{
                        in_header = kv.get_header();
                    }
                    break;
                case 2:
                    if (!kv.load_KV1(fn, fail_line)){
                        if (fail_line == -1){
                            IFPRINT << indent_line(1) << "ERROR: File \"" << fn << "\" not found." << endl;
                        }else{
                            IFPRINT << indent_line(1) << "ERROR: Failed to load file: '" + fn + "'.\n" << indent_line(2) << "Failed on line " << to_string(fail_line) << '.' << endl;
                        }
                    }else if(extension != ""){
                        in_header = kv.get_header();
                        IFPRINT << indent_line(1) << "File '" + fn + "' read successfully." << endl;
                    }else{
                        in_header = kv.get_header();
                    }
                    break;
                case 3:
                    if (!kv.load_KV1(fn, fail_line)){
                        if (fail_line == -1){
                            IFPRINT << indent_line(1) << "ERROR: File \"" << fn << "\" not found." << endl;
                        }else{
                            IFPRINT << indent_line(1) << "ERROR: Failed to load file: '" + fn + "'.\n" << indent_line(2) << "Failed on line " << to_string(fail_line) << '.' << endl;
                        }
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

        }else if(words_uc[0] == "SAVE"){

            string fn = "";
            bool save_comments = true;
            bool append_save_dir = false;
            bool append_home_dir = false;
            int file_type = 1; //1 = KV1, 2 = KV2, 3 = KV3
            kv.set_header(out_header);
            for (int i = 1 ; i < words.size() ; i++ ){
                if (words_uc[i] == "-KV1"){
                    file_type = 1;
                }else if(words_uc[i] == "-KV2"){
                    file_type = 2;
                }else if(words_uc[i] == "-KV3"){
                    file_type = 3;
                }else if(words_uc[i] == "-SD" || words_uc[i] == "-SAVE_DIR"){
                    append_save_dir = true;
                    append_home_dir = false;
                }else if(words_uc[i] == "-H" || words_uc[i] == "-HOME_DIR" || words_uc[i] == "-HOME" ){
                    append_save_dir = false;
                    append_home_dir = true;
                }else if(words_uc[i] == "-C"){
                    save_comments = true;
                }else if(words_uc[i] == "-NC"){
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
        }else if(words_uc[0] == "VHDR"){
            if (in_header != ""){
                IFPRINT << indent_line(1) << "READ HEADER: " << endl;
                IFPRINT << indent_in_string(in_header, 2) << endl;
            }
            if (out_header != ""){
                IFPRINT << indent_line(1) << "WRITE HEADER: " << endl;
                IFPRINT << indent_in_string(out_header, 2) << endl;
            }
        }else if(words_uc[0] == "SHDR"){

            string hdr = "";
            bool copy_in_header = false;
            for (int i = 1 ; i < words.size() ; i++ ){
                if (words_uc[i] == "-IN"){
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
        }else if(words_uc[0] == "RUN"){
            bool silence = true;
            string fn = "";
            bool persist = false;

            for (int i = 1 ; i < words.size() ; i++){
                if (words_uc[i] == "-S"){
                    silence = true;
                }else if(words_uc[i] == "-NS"){
                    silence = false;
                }else if(words_uc[i] == "-P"){
                    persist = true;
                }else if(words_uc[i] == "-NP"){
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

			vector<int> fail_lines;
			vector<string> fail_messages;
            if (!run_interpret(fn, kv, result, functions, persist, (!silence), indent_line(1), record, true, settings, in_header, out_header, fail_lines, fail_messages, case_sensitive_functions)){
                if (result.type == 'e'){
                    if (persist){
                    		cout << indent_line(1) << "FAILED TO RUN FILE: '" << fn << "'. Failed " << fail_lines.size() << " times on lines ";
							for (int l = 0 ; l < fail_lines.size() ; l++){
								cout << fail_lines[l];
								if (l+1 < fail_lines.size()) cout << ", ";
							}
							cout << "." << endl;
							if (fail_messages.size() != fail_lines.size()){
								cout << "SOFTWARE ERROR: Mismatched number of fail lines and fail messages!" << endl;
							}
							for (int l = 0 ; l < fail_messages.size() ; l++){
								cout << indent_line(2) << "Line " << fail_lines[l] << ": " << fail_messages[l] << endl;
							}
                    }else{
						if (fail_lines.size() > 0){
							cout << indent_line(1) << "FAILED TO RUN FILE: '" << fn << "'. Failed on line " << fail_lines[0] << " and aborted." << endl;
							cout << indent_line(2) << result.s << endl;
						}else{
							cout << "SOFTWARE ERROR: run_interpret() RETURNED INVALID PARAMETERS." << endl;
						}

                    }
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
        }else if(words_uc[0] == "SVPRG"){

            string filename = "";
            char append_dir = 'n'; //n = none, h = home, s = save_dir
            for (int i = 1 ; i < words.size() ; i++){
                if(words_uc[i] == "-H" || words_uc[0] == "-HOME"){
                    append_dir = 'h';
                }else if(words_uc[i] == "-SD" || words_uc[i] == "-SAVE_DIR"){
                    append_dir = 's';
                }else if(words[i][0] != '-'){
                    filename = filename + words[i];
                }
            }
            if (filename == "" || filename == settings.home_dir || filename == settings.save_dir || filename == settings.home_dir+"/" || filename == settings.save_dir+"/") filename = "program.clc";

            //Append directory if requested
            if (append_dir != 'n'){
                switch(append_dir){
                    case('h'):
                        if (settings.home_dir[settings.home_dir.length()-1] == '/'){
                            filename = settings.home_dir + filename;
                        }else{
                            filename = settings.home_dir + '/' + filename;
                        }
                        break;
                    case('s'):
                        if (settings.save_dir[settings.save_dir.length()-1] == '/'){
                            filename = settings.save_dir + filename;
                        }else{
                            filename = settings.save_dir + '/' + filename;
                        }
                        break;
                }
            }

            fstream file;
            file.open(filename, std::ofstream::out | std::ofstream::trunc);
            if (!file.is_open()){
                IFPRINT << indent_line(1) << "ERROR: Failed to create file " << filename << endl;
            }

            for (int i = 0 ; i < record.size() ; i++){
                file << record[i].command << endl;
            }

            file.close();
        }else if(words_uc[0] == "DELETE"){
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
        }else if(words_uc[0] == "PRCD"){

            bool count_start_beginning = false;
            double count = -1;
            bool print_errors = false;
            for (int i = 1 ; i < words.size() ; i++){
                if (words_uc[i] == "-B"){
                    count_start_beginning = true;
                }else if(words_uc[i] == "-NB"){
                    count_start_beginning = false;
                }else if(words_uc[i] == "-A"){
                    print_errors = true;
                }else if(words_uc[i] == "-NA"){
                    print_errors = false;
                }else if(words[i][0] != '-'){
                    if (!str_to_double(words[i], count)){
                        IFPRINT << indent_line(1) << "ERROR: Failed to interpret '" + words[i] + "' as number" << endl;
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

        }else if(words_uc[0] == "SET"){
            run_settings_menu(settings);
            kv.set(KV_PRINT_THRESHOLD, settings.threshold);
            kv.set(KV_PRINT_PRECISION, (double)settings.precision);
        }else if(words_uc[0] == "RELIGION"){
            if (!kv.contains("religion")) kv.add_bool("religion", false);
            IFPRINT << indent_line(1) << bool_to_str(false) << endl;
        }else if(words_uc[0] == "MATLAB"){
            if (!kv.contains("MATLAB")) kv.add_string("MATLAB", "Worst 'programming language' in the history of the universe.");
            IFPRINT << indent_line(1) << "Spawn of satan" << endl;
        }else if(words_uc[0] == "CLHIST"){
            clear_history();
            IFPRINT << indent_line(1) << "Input history cleared" << endl;
        }else if(words_uc[0] == "SVSTATE"){
            
            //Process flags
            
            //Create directory
            
            //Save RCD file
            
            //Save KV1 file
            
            //Save Input History file
            
            //Save settings file
            
            //Zip directory
            
        }else if(words_uc[0] == "LDSTATE"){
            
            //Process flags
            
            //Unzip directory (but keep origional zip file, copy if neccesary)
            
            //read RCD file
            
            //read KV1 file
            
            //read Input History file
            
            //read settings file
            
            //delete unzipped file
        }else if(words_uc[0] == "RABBIT"){
            print_file(string(RESOURCE_DIR) + "doc/rabbit.txt", 0);
        }else if(words_uc[0] == "CLRCD"){
            record.clear();
            IFPRINT << indent_line(1) << "Record cleared." << endl;
        }else if(words_uc[0] == "SVRCD"){
            string filename = "";
            char append_dir = 'n'; //n = none, h = home, s = save_dir
            for (int i = 1 ; i < words.size() ; i++){
                if(words_uc[i] == "-H" || words_uc[0] == "-HOME"){
                    append_dir = 'h';
//                    if (filename.length() < settings.home_dir.length() || filename.substr(0, settings.home_dir.length()) != settings.home_dir){
//                        if (settings.home_dir[settings.home_dir.length()-1] != '/'){
//                            filename = settings.home_dir + '/' + filename;
//                        }else{
//                            filename = settings.home_dir + filename;
//                        }
//                    }
                }else if(words_uc[i] == "-SD" || words_uc[i] == "-SAVE_DIR"){
                    append_dir = 's';
//                    if (filename.length() < settings.save_dir.length() || filename.substr(0, settings.save_dir.length()) != settings.save_dir){
//                        if (settings.save_dir[settings.save_dir.length()-1] != '/'){
//                            filename = settings.save_dir + '/' + filename;
//                        }else{
//                            filename = settings.save_dir + filename;
//                        }
//                    }
                }else if(words[i][0] != '-'){
                    filename = filename + words[i];
                }
            }
            if (filename == "" || filename == settings.home_dir || filename == settings.save_dir || filename == settings.home_dir+"/" || filename == settings.save_dir+"/") filename = "record.rcd";

            //Append directory if requested
            if (append_dir != 'n'){
                switch(append_dir){
                    case('h'):
                        if (settings.home_dir[settings.home_dir.length()-1] == '/'){
                            filename = settings.home_dir + filename;
                        }else{
                            filename = settings.home_dir + '/' + filename;
                        }
                        break;
                    case('s'):
                        if (settings.save_dir[settings.save_dir.length()-1] == '/'){
                            filename = settings.save_dir + filename;
                        }else{
                            filename = settings.save_dir + '/' + filename;
                        }
                        break;
                }
            }

            save_record(filename, record, settings);
        }else{  //If not keyword, evaluate expression
            //Evaluate expression
            string feed_string = cat_tokens(words, 0, " ");
//            ensure_whitespace(feed_string, "-");
//            cout << "Feed string: " << feed_string << endl;
//            cout << "Input: " << input << endl;
            if (!interpret(feed_string, kv, result, functions, true, case_sensitive_functions)){
//            if (!interpret(input, kv, result, functions, true)){
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
                char notation_type;
                if (settings.force_sci){
                    notation_type = 's';
                }else if (settings.force_fixed){
                    notation_type = 'f';
                }else{
                    notation_type = 'x'; //Automatically select based on value
                }
                IFPRINT << indent_line(1) << akt_tostring(result, settings.precision, settings.threshold, notation_type) << endl;
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
