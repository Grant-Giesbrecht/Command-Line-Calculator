#include "string_manip.hpp"

#include <iostream>
#include <stdio.h>
#include <cmath>
#include <ctgmath>

using namespace std;

/*
 Ensures that space characters sandwich all the target characters in the string 'in'
 
 in - string in which to ensure targets surrounded by whitespace
 targets - characters to surround with whitespace
 
 Void return
 */
void ensure_whitespace(string& in, string targets){
    
    //Add whitespace buffer around all target characters
    for (int i = 0 ; i < in.length() ; i++){
        if (targets.find(in[i]) != string::npos){ //Found in string
            in = in.substr(0, i) + " " + in[i] + " " + in.substr(i+1);
            i++;
        }
    }
    
    //Remove excess white space - no consecutive spaces
    for (int i = 0; i+1 < in.length() ; i++){
        if (in[i] == ' ' && in[i+1] == ' '){
            in = in.substr(0, i) + in.substr(i+1);
            i--;
        }
    }
    
}

/*
 Ensures that space characters sandwich all the target characters in the string 'in' unless those characters are constituting a protected target.
 
 in - string in which to ensure targets surrounded by whitespace
 targets - characters to surround with whitespace
 protected_targets - vector of strings where each string defines a protected target. eg a sequence of characters that will not be broken up.
 
 Void return
 */
void ensure_whitespace_protected(string& in, string targets, vector<string> protected_targets){
    
    //Add whitespace buffer around all target characters
    for (int i = 0 ; i < in.length() ; i++){
        if (targets.find(in[i]) != string::npos){ //Found in string
            
            //Check for protected targets
            bool pt = false;
            for (int j = 0 ; j < protected_targets.size() ; j++){ //Iterate through each protected target to check for match
                for (int k = 0; k < protected_targets[j].length() ; k++){ //Iterate through each character in PT to see if match possible
                    if (protected_targets[j][k] == in[i]){ //Match possible, explicitly check
                        if ( i-k >= 0 && protected_targets[j] == in.substr(i-k, protected_targets[j].length())){
//                            cout << "PT:\t" << in << endl;
//                            cout << "\t";
//                            for (int l = 0; l < i ; l++){
//                                cout << ' ';
//                            }
//                            cout << '^' << endl;
                            pt = true; //Was match, flag not to add whitespace
                        }
                        string s = in.substr(i-k, protected_targets[j].length());
                        int a;
                        int b  = 4;
                    }
                }
            }
            
            //Add whitespace
            if (!pt){
                in = in.substr(0, i) + " " + in[i] + " " + in.substr(i+1);
                i++;
            }
        }
    }
    
    //Remove excess white space - no consecutive spaces
    for (int i = 0; i+1 < in.length() ; i++){
        if (in[i] == ' ' && in[i+1] == ' '){
            in = in.substr(0, i) + in.substr(i+1);
            i--;
        }
    }
    
}

/*
 Ensures that space characters sandwich the target string in the string 'in'
 
 in - string in which to ensure targets surrounded by whitespace
 target - string to surround with whitespace - must appear in full to be surrounded
 
 Void return
 */

void ensure_whitespace_full(string& in, string target){

    //Add whitespace buffer around all target characters
    for (int i = 0 ; i+target.length() < in.length() ; i++){
        if (in.substr(i, target.length()) == target ){
            in = in.substr(0, i) + " " + in.substr(i, target.length()) + " " + in.substr(i+target.length());
            i += 1 + target.length();
        }
    }
    
    //Remove excess white space - no consecutive spaces
    for (int i = 0; i+1 < in.length() ; i++){
        if (in[i] == ' ' && in[i+1] == ' '){
            in = in.substr(0, i) + in.substr(i+1);
            i--;
        }
    }
    
}

void remove_end_whitespace(std::string& io){

    while (io.length() > 0){
        if (io[0] == ' ' || io[0] == '\t'){
            io = io.substr(1);
        }else break;
    }

    while (io.length() > 0){
        if (io[io.length() - 1] == ' ' || io[io.length()] == '\t'){
            io = io.substr(0, io.length()-1);
        }else break;
    }

}

/*
 Parses a string into a vector of strings. String broken at each instance of 'delin'. 'delin' is excluded from the output.
 
 input - string to parse
 delin - deliniating characters
 
 Returns a vector of parsed strings
 */
vector<string> parse(string input, string delin){
    
    vector<string> output;
    
    int len_counter = 0; //Number of elements in the deliated seciton
    for (int i = 0; i < input.length() ; i++){
        
        if ((delin.find(input[i]) != string::npos)){ //Deliniator found
            
            //Add block to output if section exists (not two deliniators in a row)
            if (len_counter > 0){
                output.push_back(input.substr(i-len_counter , len_counter));
            }
            
            //Reset length counter
            len_counter = 0;
            
        }else if(i+1 == input.length()){ //Handle end conditions
            len_counter++;
            
            if (input.substr(i-len_counter+1 , len_counter).length() > 0){
                output.push_back(input.substr(i-len_counter+1 , len_counter));
            }
            
        }else{
            
            //Increment length counter
            len_counter++;
            
        }
    }
	
	return output;
}

/*
 Determines if the string represents a number (decimal or integer). Accepts scientific notation and negatives.
 
 s - string to analyze
 
 Returns true if 's' is a number
 */
bool isnum(string s){

    bool allow_e = true;
    bool allow_punct = true;

    for (int i = 0 ; i < s.length() ; i++){
        if (s[i] == '0' || s[i] == '1' ||s[i] == '2' ||s[i] == '3' ||s[i] == '4' ||s[i] == '5' ||s[i] == '6' ||s[i] == '7' || s[i] == '8' ||s[i] == '9' ){
            //Do nothing, all is well            
        }else if(i == 0 && s[i] == '-'){
            if (i+1 >= s.length() || !(s[i+1] == '0' || s[i+1] == '1' || s[i+1] == '2' || s[i+1] == '3' || s[i+1] == '4' || s[i+1] == '5' || s[i+1] == '6' || s[i+1] == '7' || s[i+1] == '8' || s[i+1] == '9' ) ){ //If first char is minus, next character must number
                if (i+2 < s.length() && s[i+1] == '.' && ( s[i+1] == '0' || s[i+2] == '1' || s[i+2] == '2' || s[i+2] == '3' || s[i+2] == '4' || s[i+2] == '5' || s[i+2] == '6' || s[i+2] == '7' || s[i+2] == '8' || s[i+2] == '9' )){ //If next char is decimal then number, process as if decimal
                    allow_punct = false;
                }else{
                    return false;
                }
                
            }
            i++;
        }else if(allow_e && s[i] == 'e'){

            //Ensure e is not first char
            if (i == 0) return false;

            //Check that e is followed by a number
            if (i+1 >= s.length() || !(s[i+1] == '0' || s[i+1] == '1' || s[i+1] == '2' || s[i+1] == '3' || s[i+1] == '4' || s[i+1] == '5' || s[i+1] == '6' || s[i+1] == '7' || s[i+1] == '8' || s[i+1] == '9' || s[i+1] == '-') ){
                return false;
            }

            //If e is followed by '-', ensure number follows
            if (s[i+1] == '-'){
                if (i+2 >= s.length() || !(s[i+2] == '0' || s[i+2] == '1' || s[i+2] == '2' || s[i+2] == '3' || s[i+2] == '4' || s[i+2] == '5' || s[i+2] == '6' || s[i+2] == '7' || s[i+2] == '8' || s[i+2] == '9') ){
                    return false;
                }
                i++;
            }
            i++;
            allow_e = false;
        }else if(allow_punct && s[i] == '.'){
            allow_punct = false;
        }else{
            return false;
        }

    }

    return true;
}


/*
 Removes any characters following an instance of the 'commentor' string. Must be complete 'commentor' to trigger. Modifies input string 's'.
 
 s - string whose comments are to be removed
 commentor - string indicating comment follows
 
 Void return
 */
void remove_comments(string& s, string commentor){
    
	for (int i = 0 ; i < s.length() ; i++){
		if (s.substr(i, commentor.length()) == commentor){
			s = s.substr(0, i);
		}
	}

}

/*
 Removes any character included in 'targets' from the end of 's'. The target char must be the last character in order to be removed. Modifies 's'
 
 s - string to modify
 targets - characters to target
 
 Void return
 */
void remove_from_end(string& s, string targets){

	for (int i = (int)s.length() - 1 ; i >= 0 ; i--){
		if ( targets.find(s.substr(i, 1)) != string::npos){
			s.pop_back();
		}else{
			i = -1;
		}
	}

}

/*
 Concatonates a vector of strings into single string
 
 t - vector of strings to concatonate
 idx - index at which to start concatonating (inclusive & through end)
 joint - string to add between each vector element concatonated
 
 Returns concatonated strings
 */
string cat_tokens(vector<string> t, int idx, string joint){

	if (t.size() <= idx){
		return "";
	}
    
    if (idx < 0) idx = 0;

	string out = t[idx]; 
	for (int i = idx+1 ; i < t.size() ; i++){
		out = out + joint + t[i]; 
	} 

	return out;
}

/*
 Converts the input string's lowercase letters to uppercase letters.
 
 s - string to convert to upper case
 
 Returns the capitolized string
 */
string to_uppercase(std::string s){
	for (int i = 0; i < s.length() ; i++){
		s[i] = toupper(s[i]);
	}
	return s;
}

/*
 Converts the input string's uppercase letters to lowercase letters.
 
 s - string to convert
 
 Rerturns the lowercase string
 */
string to_lowercase(std::string s){
	for (int i = 0; i < s.length() ; i++){
		s[i] = tolower(s[i]);
	}
	return s;
}

/*
 Returns a string representing the input boolean value.
 
 x - bool representing value to output
 uppercase - if true, will print TRUE/FALSE instead of True/False
 */
string bool_to_str(bool x, bool uppercase){
	if (x){
		if (uppercase){
			return "TRUE";
		}else{
			return "True";
		}
	}else{
		if (uppercase){
			return "FALSE";
		}else{
			return "False";
		}
	}
}

string format_newline(string input, string prefix){
    
    string out = "";
    
    vector<string> parts = parse(input, "\n");
    for (unsigned long i = 0 ; i < parts.size() ; i++){
        if (out != "") out = out + '\n';
        out = out + prefix + parts[i];
    }
    
    return out;
}

string hp_string(double input, int precision, bool scientific){

    int len = (int)precision+100;
	char buff[len];
//    for (int i = 0 ; i < len ; i++){
//        buff[i] = '0';
//    }
    string precision_str = "%." + to_string((int)precision);
    precision_str = (scientific) ? precision_str + "e" : precision_str + "f";
	sprintf(buff, precision_str.c_str(), input);
	string out(buff);
//    string out = "";
//    for (int i = 0 ; i < len ; i++){
//        out = out + buff[i];
//    }

    return out;
}

double strtod(std::string input, bool* success){
    
    //[-][###][.][###][E][-][###]
    
    if (success != NULL) *success = true;
    
    return stod(input);
    
//    double x = 0;
//    bool neg = false;
//    int start = 0;
//    
//    //Remove excess whitespace
//    for (int i = 0 ; i < input.length() ; i++){
//        if (input[i] == ' ' || input[i] == '\t'){
//            input = input.substr(0, i) + input.substr(i+1);
//            i--;
//        }
//    }
//    
//    //Look for negatives
//    if (input[0] == '-'){
//        neg = true;
//        start = 1;
//    }
//    
//    //Read 1st number (before sceintific notation component)
//    
//    //Determine length of number
//    bool decimal_used = false;
//    int pt_idx = -1;
//    int end = -1;
//    
//    for (int i = start ; i < input.length() ; i++){
//        if (input [i] == '.'){
//            if (decimal_used){
//                return -1;
//                if (success != NULL) *success = false;
//            }
//            decimal_used = true;
//            pt_idx = i;
//        }else if((i+1 == input.length() && end == -1) || input[i+1] == 'e' || input[i+1] == 'E'){
//            end = i;
//        }else if(!isnum(input.substr(i, 1))){
//            //pass
//        }
//    }
//    if (!decimal_used){
//        pt_idx = end+1;
//    }
//    
//    //Calculate value of first number
//    if (start != pt_idx){
//        int place = 0;
//        for (int i = start ; i < pt_idx ; i++ ){
//            x = x*10 + ( ((int)input[i])-48 );
//            place++;
//        }
//        //        double decimal = 0; //Alternate evaluation meathod - faster and potentially less accurate in some situation
//        //        for (int i = end ; i > pt_idx ; i--){
//        //            decimal += ( ((int)input[i])-48 );
//        //            decimal /= 10;
//        //        }
//        place = -1;
//        for (int i = pt_idx+1 ; i <= end ; i++){
//            x += pow(10, place) * ( ((int)input[i])-48 );
//            place--;
//        }
//    }
//    
//    //Negate negatives
//    if (neg){
//        x = -1 * x;
//    }
//    
//    //Evalute power of 10
//    if (end != input.length()-1){
//        if (end + 2 > input.length()){
//            if (success != NULL) *success = false;
//            return -1;
//        }
//        bool subsuccess;
//        double power = strtod(input.substr(end+2), &subsuccess);
//        if (!subsuccess){
//            if (success != NULL) *success = false;
//            return -1;
//        }
//        x *= pow(10, power);
//    }
//    
//    return x;
}

/*

*/
char select_notation(double num, int precision, int threshold, bool force_sci, bool force_fix){
    
    // cout.precision(precision);

    if (force_sci){
        cout << std::scientific;
        return 's';
    }else if(force_fix){
        cout << std::fixed;
        return 'f';
    }

    char ret = select_notation(num, threshold);
    if (ret == 's'){
        cout << std::scientific;
    }else{
        cout << std::fixed;
    }

    return ret;
    
    // if (force_sci || num > threshold){
    //     cout << std::scientific;
    // }else if(force_fix || num >= 1){
    //     cout << std::fixed;
    // }else{
    //     if (num < 1/threshold){
    //         cout << std::scientific;
    //     }else{
    //         cout << std::fixed;
    //     }
    //     //Show as much as possible is 'precision' digits, with as few characters
    //     //If #_1 > prec, sci & ignore #_2
    //     //If #_1 < prec && NONZERO: fixed
    //     //   If #_2 > prec-length(#_1), show #_2
    //     //   If #_2 < prec-length(#_1), ignore #_2
    //     //IF #_1 == 0:
    //     //   If #_2 < prec, sci
    //     //   If #_2 > prec, fixed
        
    // }
    
}

char select_notation(double num, int threshold){
    
    num = abs(num);
    
    if (num > threshold){
        return 's';
    }else if(num >= 1){
        return 'f';
    }else{
        double a = 1.0/threshold;
        if (num < a){
            return 's';
        }else{
            return 'f';
        }
    }
    
}

std::string add_space_indicators(std::string input){
    return add_space_indicators(input, "");
}

std::string add_space_indicators(std::string input, std::string indentation){
    string out;
    out = indentation + input + indentation + "\n";
    for (int i = 0 ; i < input.length(); i++){
        if (input[i] == ' '){
            out = out + '^';
        }else{
            out = out + ' ';
        }
    }
    return out;
}

/*
 Determines if a string represents a string literal. If so, it determines its value.
 
 input - string in which to search for string
 output - returned string (if found)
 
 Returns true iff a string is found
 */
bool str_to_string(std::string input, string& output){
    
    //Look for matrix (enclosed in square brackets). Return if not found
    bool success;
    int last_idx;
    string phrase = next_string(input, success, 0, &last_idx);
    if (!success) return false;
    
    //Write to output if string detection successful
    output = phrase;
    
    return true;
}

///*
//Determines if a string represents a double. If so, it determines its value.
//
//input - string in which to search for a double literals
//output - returned double (if one is found)
//
//Returns true iff a double is found
//*/
//bool str_to_double(std::string input, double& output){
//
//    if (!isnum(input)) return false;
//
//    output = atof(input.c_str());
//
//    return true;
//}

/*
 Determines if a string represents a double. If so, it determines its value.
 
 previous_word - word preceeding input. If it is a minus sign, the number is made negative
 input - string in which to search for a double literals
 output - returned double (if one is found)
 
 Returns true iff a double is found
 */
bool str_to_double(std::string input, double& output, string previous_word){
    
    string error_val;
    return str_to_double(input, output, error_val, previous_word);
    
}

/*
 Determines if a string represents a double. If so, it determines its value.
 
 previous_word - word preceeding input. If it is a minus sign, the number is made negative
 input - string in which to search for a double literals
 output - returned double (if one is found)
 
 Returns true iff a double is found
 */
bool str_to_double(std::string input, double& output, string& error, std::string previous_word){
    
    bool negative = false;
    
    cout << std::scientific;
    
    if (previous_word == "-" && isnum(previous_word + input)){
        negative = true;
    }
    
    if (!isnum(input)) return false;
    
    try{
        output = stod(input);
        output = strtod(input);
    }catch(const std::out_of_range& oor){
        error = "Number out of range (" + string(oor.what()) + ")";
    }catch(...){
        error = "SOFTWARE ERROR: Failed to convert string to double";
        return false;
    }
    //    output = strtod(input.c_str(), NULL);
    
    if (negative){
        output = output * -1;
    }
    
    return true;
    
}

/*
 Determines if a string represents a bool. If so, it determines its value
 
 input - string to analyze
 out - bool output
 
 Returns true if 'input' is a bool
 */
bool str_to_bool(std::string input, bool& output){
    
    //Remove potentially
    remove_end_whitespace(input);
    
    if (to_uppercase(input) == "TRUE"){
        output = true;
    }else if(to_uppercase(input) == "FALSE"){
        output = false;
    }else{
        return false;
    }
    
    return true;
}

/*
 Determines if input is an operator +-/%*^&=><!|
 
 input - string to analyze
 
 Returns true if 'input' is an operator
 */
bool str_to_op(std::string input){
    if (input == "+" || input == "-" || input == "/" || input == "*" || input == "^" || input == "%" || input == "!" || input == "||" || input == "&&" || input == "==" || input == "!=" || input == ">=" || input == "<=" || input == "=" || input == ">" || input == "<"){
        return true;
    }
    return false;
}

//From CLIK

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
    
    bool success = next_phrase(in, value, '\"', '\"', start_word, last_word);
    
    remove_end_whitespace(*value);
    
    return success;
    
}

/*
 Locates the next phrase contained by 'starter' and 'ender'. Looks for phrase in 'in', and outputs phrase's origional form from 'in_full' into the pointer 'value'. Phrase search begins at 'start_word', and continues through the end or until the phrase is found.
 
 in - vector or strings containing the words of 'in_full'.
 in_full - origional form of 'in', from which the detected phrase's actual value will be drawn.
 value - outputs the phrase that was detected
 starter - starting character for the phrase
 ender - ending character for the phrase
 last_word - returns the last word in 'in' that was used in the detected phrase
 last_index - returns index of last character in last used word in phrase, input index of end of last used word (if this function was run multiple times. Only function is to prevent identical words from being mistaken for eachother. Enter 0 if you don't wish to use this feature).
 allow_before - if true, function will not fail if non whitespace occurs before detected string
 
 Returns true if a phrase was detected
 */
bool next_phrase(std::vector<std::string> in, std::string in_full, std::string* value, char starter, char ender, int start_word, int* last_word, int* last_index, bool allow_before){
    
    bool success;
    
    if (*last_index < 0) *last_index = 0;
    
    //Find index of 'in_full' at which to begin search for phrase (convert start_word (w/ respect to 'in') to start_idx (w/ respect to 'in_full').
    int start_idx = idx_from_word_idx(in_full, in, start_word, POSITION_WORD_START, *last_index);
    
    //Find phrase
    *value = next_phrase(in_full, success, start_idx, starter, ender, last_index, allow_before);
    
    if (!success) return false;
    
    //Find last word
    *last_word = word_idx_from_idx(in_full, in, *last_index, start_word);
    
    return true;
    
}

bool next_phrase(std::vector<std::string> in, std::string* value, char starter, char ender, int start_word, int* last_word){
    
    string str = cat_tokens(in, start_word, " ");
    
    bool success;
    int last_idx;
    
    *value = next_phrase(str, success, 0, starter, ender, &last_idx);
    
    //[1 + 2 + 3]
    //01234567890123456789
    //00000000001111111111
    
    int lw = start_word;
    for (int i = 0; i < last_idx ; i++){
        if (str[i] == ' '){
            lw++;
        }
    }
    *last_word = lw;
    
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
 minimum_idx - index of 'str_in' at which to begin search for 'words_in[word_idx]'.
 
 Returns the index where the word specified by 'word_idx' in 'words_in' exists. Returns -1 if error occurs.
 */
int idx_from_word_idx(std::string str_in, std::vector<std::string> words_in, int word_idx, int position_on_word, int minimum_idx){
    
    //check bounds
    if (word_idx > words_in.size()) return -1;
    
    //Set target
    string target = words_in[word_idx];
    
    //Ensure target can fit in str_in
    if (str_in.length() < target.length()) return -1;
    
    //Find word
    for (int i = minimum_idx ; i <= str_in.length()-target.length() ; i++){
        if (str_in.substr(i, target.length()) == target){ //Check for match
            return (position_on_word == POSITION_WORD_END)? (i+(int)target.length()-1) : i ;
        }
    }
    
    return -1;
}

/*
 Returns the index of the word in 'words_in' that contains the character indicated by 'str_idx' in 'str_in'
 
 PARAMETERS:
 str_in - string in which to specify index to translate to word index
 words_in - vector of string words, whose index will be returned
 str_idx - index of the character in 'str_in' designating a word in 'words_in'
 start_word_min - word in 'words_in' at which to start searching for match with 'str_in''s specified word.
 
 Returns -1 if no match, returns -2 if whitespace matched. Else returns the index of the word in 'words_in' matching the specified char
 in str_in.
 */
int word_idx_from_idx(std::string str_in, std::vector<std::string> words_in, int str_idx, int start_word_min){
    
    //Return an error if the index is out of bounds
    if (str_idx >= str_in.length() || str_idx < 0) return -1;
    
    //Return error if the start_idx_min is out of bounds
    if (start_word_min < 0 || str_idx < start_word_min) return -1;
    
    //Return -2 if matching whitespace
    if (str_in[str_idx] == ' ' || str_in[str_idx] == '\t') return -2;
    
    //LOOKUP: The Promise
    
    //================================== Find word specified in 'str_in' ==============================================
    
    //Find end index of specified word in 'str_in'
    int start_idx = 0;
    int end_idx = (int)str_in.length()-1;
    for (int a = str_idx ; a < str_in.length() ; a++){
        if (str_in[a] == ' ' || str_in[a] == '\t'){
            end_idx = a-1;
            break;
        }
    }
    
    //Find beginning of specified word in 'str_in'
    for (int a = str_idx ; a >= 0 ; a--){
        if (str_in[a] == ' ' || str_in[a] == '\t'){
            start_idx = a+1;
            break;
        }
    }
    
    string target = str_in.substr(start_idx, (end_idx - start_idx + 1));
    
    //Check for matches between specified word and 'words_in'
    for (int i = start_word_min ; i < words_in.size() ; i++){
        if (words_in[i] == target){
            return i;
        }
    }
    
    return -1;
}







