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
            if (i+1 >= s.length() || !(s[i+1] == '0' || s[i+1] == '1' || s[i+1] == '2' || s[i+1] == '3' || s[i+1] == '4' || s[i+1] == '5' || s[i+1] == '6' || s[i+1] == '7' || s[i+1] == '8' || s[i+1] == '9') ){
                return false;
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
    
    double x = 0;
    bool neg = false;
    int start = 0;
    
    //Remove excess whitespace
    for (int i = 0 ; i < input.length() ; i++){
        if (input[i] == ' ' || input[i] == '\t'){
            input = input.substr(0, i) + input.substr(i+1);
            i--;
        }
    }
    
    //Look for negatives
    if (input[0] == '-'){
        neg = true;
        start = 1;
    }
    
    //Read 1st number (before sceintific notation component)
    
    //Determine length of number
    bool decimal_used = false;
    int pt_idx = -1;
    int end = -1;
    
    for (int i = start ; i < input.length() ; i++){
        if (input [i] == '.'){
            if (decimal_used){
                return -1;
                if (success != NULL) *success = false;
            }
            decimal_used = true;
            pt_idx = i;
        }else if((i+1 == input.length() && end == -1) || input[i+1] == 'e' || input[i+1] == 'E'){
            end = i;
        }else if(!isnum(input.substr(i, 1))){
            //pass
        }
    }
    if (!decimal_used){
        pt_idx = end+1;
    }
    
    //Calculate value of first number
    if (start != pt_idx){
        int place = 0;
        for (int i = start ; i < pt_idx ; i++ ){
            x = x*10 + ( ((int)input[i])-48 );
            place++;
        }
        //        double decimal = 0; //Alternate evaluation meathod - faster and potentially less accurate in some situation
        //        for (int i = end ; i > pt_idx ; i--){
        //            decimal += ( ((int)input[i])-48 );
        //            decimal /= 10;
        //        }
        place = -1;
        for (int i = pt_idx+1 ; i <= end ; i++){
            x += pow(10, place) * ( ((int)input[i])-48 );
            place--;
        }
    }
    
    //Negate negatives
    if (neg){
        x = -1 * x;
    }
    
    //Evalute power of 10
    if (end != input.length()-1){
        if (end + 2 > input.length()){
            if (success != NULL) *success = false;
            return -1;
        }
        bool subsuccess;
        double power = strtod(input.substr(end+2), &subsuccess);
        if (!subsuccess){
            if (success != NULL) *success = false;
            return -1;
        }
        x *= pow(10, power);
    }
    
    return x;
}

void select_notation(double num, int precision, int threshold, bool force_sci, bool force_fix){
    
    cout.precision(precision);
    
    if (force_sci || num > threshold){
        cout << std::scientific;
    }else if(force_fix || num >= 1){
        cout << std::fixed;
    }else{
        if (num < 1/threshold){
            cout << std::scientific;
        }else{
            cout << std::fixed;
        }
        //Show as much as possible is 'precision' digits, with as few characters
        //If #_1 > prec, sci & ignore #_2
        //If #_1 < prec && NONZERO: fixed
        //   If #_2 > prec-length(#_1), show #_2
        //   If #_2 < prec-length(#_1), ignore #_2
        //IF #_1 == 0:
        //   If #_2 < prec, sci
        //   If #_2 > prec, fixed
        
    }
    
}

char select_notation(double num, int threshold){
    
    
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









