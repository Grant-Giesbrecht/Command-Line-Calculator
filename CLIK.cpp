#include "CLIK.hpp"
#include "string_manip.hpp"
using namespace std;

/*
Reads a string (defined by being enclosed in single or double quotes) and returns the detected string.

in - input string in which to look for a enclosed string
success - Returns true if string detected
start_idx - index in 'in' at which to begin scan
last_idx - index at which first detected string ended (aka index of closing quotes)
allow_before - if true, function will not fail if non whitespace occurs before detected string

Returns first detected string, empty string upon fail
*/
string next_string(string in, bool& success, int start_idx, int* last_idx, bool allow_before){

	string out = "";
	bool in_string = false;

	success = false;
	*last_idx = start_idx;

	for (int i = start_idx; i < in.length() ; i++){
		if (in[i] == '\'' || in[i] == '\"'){

			if (in_string){ //End
				success = true;
				*last_idx = i;
				return out;
			}else{ //Begin string
				in_string = true;
			}

		}else if( in_string ){
			out = out + in[i];
		}else if(in[i] != ' ' && in[i] != '\t' && !allow_before){
			success = false;
			*last_idx = i;
			return "";
		}

	}

	return out;
}

/*
Reads a string (defined by being enclosed in 'starter' char and 'ender' char) and returns the detected string.

in - input string in which to look for a enclosed string
success - Returns true if string detected
start_idx - index in 'in' at which to begin scan
last_idx - index at which first detected string ended (aka index of closing quotes)
allow_before - if true, function will not fail if non whitespace occurs before detected string

Returns first detected string, empty string upon fail
*/
string next_phrase(string in, bool& success, int start_idx, char starter, char ender, int* last_idx, bool allow_before){

	string out = "";
	bool in_string = false;

	success = false;
	*last_idx = start_idx;

	for (int i = start_idx; i < in.length() ; i++){
		if (in[i] == starter){

			if (in_string){ //End

			}else{ //Begin string
				in_string = true;
			}
		}else if(in[i] == ender){

			if (in_string){
				success = true;
				*last_idx = i;
				return out;
			}else if(!allow_before){
				success = false;
				*last_idx = i;
				return "";
			}

		}else if( in_string ){
			out = out + in[i];
		}else if(in[i] != ' ' && in[i] != '\t' && !allow_before){
			success = false;
			*last_idx = i;
			return "";
		}

	}

	return out;
}

bool next_string(std::vector<std::string> in, std::string* value, int start_word, int* last_word){

	string str = cat_tokens(in, start_word, " ");

	bool success;
	int end_idx;
	*value = next_string(str, success, 0, &end_idx);

    for (int i = start_word ; i < in.size() ; i++){
        if (in[i][0] == '\"'){
            *last_word = i;
            break;
        }
    }
    
//	int total = 0;
//	for (int i = start_word ; i < in.size(); i++ ){
//		total = total + in[i].length() + 1;
//		if (total >= end_idx){
//			*last_word = i;
//            break;
//		}
//	}

	return success;

}

bool next_phrase(std::vector<std::string> in, std::string* value, char starter, char ender, int start_word, int* last_word){

	string str = cat_tokens(in, start_word, " ");

	bool success;
	int end_idx;
	*value = next_phrase(str, success, 0, starter, ender, &end_idx);
    
    //[1 + 2 + 3]
    //01234567890123456789
    //00000000001111111111

    for (int i = start_word ; i < in.size() ; i++){
        if (in[i][0] == ender){
            *last_word = i;
            break;
        }
    }
    
//    *last_word = -1;
//    bool whitespace = true;
//    for (int i = 0 ; i < end_idx ; i++){
//        if (i == 40){
//            int useless;
//            useless = 3;
//        }
//        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\r' && str[i] != '\n'){
//            if (whitespace){
//                whitespace = false;
//                (*last_word)++;
//            }
//        }else{
//            whitespace = true;
//        }
//    }
    
//	int total = 0;
//	for (int i = start_word ; i < in.size(); i++ ){
//		total = total + in[i].length() + 1;
//		if (total >= end_idx){
//			*last_word = i;
//		}
//	}

	return success;

}

bool next_double(string in, double* value){

	try{
		*value = stod(in);
	}catch(...){
		*value = -1;
		return false;
	}

	return true;
}

bool next_bool(string in, bool* value){

	//Skip spaces
	int i = 0;
	for ( ; i < in.length() ; i++){
		if (in[i] != ' ') break;
	}

	if (in.length() < i+4){
		return false;
	}else if(to_uppercase(in.substr(i, 4)) == "TRUE"){
		*value = true;
		return true;
	}else if(in.length() < 5){
		return false;
	}else if(to_uppercase(in.substr(i, 5)) == "FALSE"){
		*value = false;
		return true;
	}else{
		return false;
	}
}
