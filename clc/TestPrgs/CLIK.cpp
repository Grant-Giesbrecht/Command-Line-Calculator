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

    int last_idx_if_null;
    if (last_idx == NULL){
        last_idx = &last_idx_if_null;
    }
    
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

in - input string in which to look for a enclosed phrase
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
                if (starter == ender){
                    success = true;
                    *last_idx = i;
                    return out;
                }else{
                    success = false;
                    *last_idx = i;
                    return "";
                }
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

    //std::string next_phrase(std::string in, bool& success, int start_idx, char starter, char ender, int* last_idx=NULL, bool allow_before=false);
//    bool next_phrase(std::vector<std::string> in, std::string* value, char starter, char ender, int start_word, int* last_word);
    bool success = next_phrase(in, value, '\"', '\"', start_word, last_word);
    
    remove_end_whitespace(*value);
    
    return success;
    
//	string str = cat_tokens(in, start_word, " ");
//
//	bool success;
//	int end_idx;
//	*value = next_string(str, success, 0, &end_idx);
//
//    for (int i = start_word ; i < in.size() ; i++){
//        if (in[i][0] == '\"'){
//            *last_word = i;
//            break;
//        }
//    }
    
//	int total = 0;
//	for (int i = start_word ; i < in.size(); i++ ){
//		total = total + in[i].length() + 1;
//		if (total >= end_idx){
//			*last_word = i;
//            break;
//		}
//	}

}

bool next_phrase(std::vector<std::string> in, std::string in_full, std::string* value, char starter, char ender, int start_word, int* last_word, bool allow_before){
    
    bool success;
    int last_idx;

//    string next_phrase(string in, bool& success, int start_idx, char starter, char ender, int* last_idx, bool allow_before)
    
    //Find start index
    int start_idx = idx_from_word_idx(in_full, in, start_word, POSITION_WORD_START);
    
    //Find phrase
    *value = next_phrase(in_full, success, start_idx, starter, ender, &last_idx, allow_before);
//    string next_phrase(string in, bool& success, int start_idx, char starter, char ender, int* last_idx, bool allow_before)
    
    if (!success) return false;
    
    //Find last word
//    word_idx_from_idx();
    
    int lw = 0;
    for (int i = 0; i < last_idx ; i++){
        if (in_full[i] == ' '){
            lw++;
        }
    }
    *last_word = lw;
    
    return true;
    
}

bool next_phrase(std::vector<std::string> in, std::string* value, char starter, char ender, int start_word, int* last_word){

	string str = cat_tokens(in, start_word, " ");

	bool success;
    int last_idx;
//	int end_idx;
    
    //std::string next_phrase(std::string in, bool& success, int start_idx, char starter, char ender, int* last_idx=NULL, bool allow_before=false);
    //    bool next_phrase(std::vector<std::string> in, std::string* value, char starter, char ender, int start_word, int* last_word);
    *value = next_phrase(str, success, 0, starter, ender, &last_idx);
    
    //[1 + 2 + 3]
    //01234567890123456789
    //00000000001111111111

    int lw = 0;
    for (int i = 0; i < last_idx ; i++){
        if (str[i] == ' '){
            lw++;
        }
    }
    *last_word = lw;
    
    //    bool starter_triggered = false;
//    for (int i = start_word ; i < in.size() ; i++){
//        if (in[i][0] == ender && ( ender != starter || starter_triggered)){
//            *last_word = i;
//            break;
//        }else if(in[i][0] == starter && starter == ender){
//            starter_triggered = true;
//        }
//    }
    
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
        *value = strtod(in);
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

/*
 Return the index of the character in 'str_in' at which word at index 'word_idx' in 'words_in' starts or ends, depending on 
 'position_on_word'.
 
 PARAMETERS:
 str_in - string whose index returned is where word 'word_idx' is
 words_in - vector of strings input. string of index 'word_idx' will be found in 'str_in'.
 word_idx - index of word in 'words_in' to find in 'str_in'.
 position_on_word - specify if beginning or end position of word should be reported.
    OPTIONS:
    POSITION_WORD_START - Return character index at start of word
    POSITION_WORD_END - Return character index at end of word
 
 Returns the index where the word specified by 'word_idx' in 'words_in' exists. Returns -1 if error occurs.
 */
int idx_from_word_idx(std::string str_in, std::vector<std::string> words_in, int word_idx, int position_on_word){
    
    //Set target
    string target = words_in[word_idx];
    
    //Find word
    for (int i = 0 ; i <= str_in.length()-target.length() ; i++){
        if (str_in.substr(i, target.length()) == target){ //Check for match
            return (position_on_word == 1)? (i+(int)target.length()-1) : i ;
        }
    }
    
    return -1;
}

/*
 Returns the index of the word in 'words_in' that contains the character indicated by 'str_idx' in 'words_in'
 
 PARAMETERS:
 str_in - string in which to specify index to translate to word index
 words_in - vector of string words, whose index will be returned
 str_idx - index of the character in 'str_in' designating a word in 'words_in'
 
 Returns -1 if no match, returns -2 if whitespace matched. Else returns the index of the word in 'words_in' matching the specified char 
 in str_in.
 */
int word_idx_from_idx(std::string str_in, std::vector<std::string> words_in, int str_idx){
    
    //Return an error if the index is out of bounds
    if (str_idx >= str_in.length() || str_idx < 0) return -1;
    
    //Return -2 if matching whitespace
    if (str_in[str_idx] == ' ' || str_in[str_idx] == '\t') return -2;
    
    //LOOKUP: The Promise
    
    //Find word specified in 'str_in'
    int start_idx = 0;
    int end_idx = (int)str_in.length()-1;
    for (int a = str_idx ; a < str_in.length() ; a++){
        if (str_in[a] == ' ' || str_in[a] == '\t'){
            end_idx = a-1;
            break;
        }
    }
    
    for (int a = str_idx ; a >= 0 ; a--){
        if (str_in[a] == ' ' || str_in[a] == '\t'){
            start_idx = a+1;
            break;
        }
    }
    
    string target = str_in.substr(start_idx, (end_idx - start_idx + 1));
    cout << "TARGET: " << target << endl;
    
    //Check for matches between specified word and 'words_in'
    for (int i = 0 ; i < words_in.size() ; i++){
        if (words_in[i] == target){
            return i;
        }
        cout << words_in[i] << endl;
    }
    
    return -1;
}



