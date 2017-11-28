#include "KInterpAux.hpp"
#include "kc_aux.hpp"
#include "CLIK.hpp"
#include "string_manip.hpp"

#define DOUBLE_ID "double"
#define STRING_ID "string"
#define BOOL_ID "bool"
#define MATRIX_ID "matrix"

using namespace std;

/*
Detects a variable (broken up in 'words'). Varifies name legitimacy and repetition. Checks variable type. Returns errors or variable name and type in 'output'.

words - vector of strings to interpet, looking for new variable
begin_idx - location (in 'words') at which to beign variable "scan"
end_idx - index (in 'words') at which to end variable "scan". Inclusive of 'end_idx'
kv - KVar with variables to avoid repetition, and to add new variables
varname - string that returns variable name.
type - type of varible detected (s - string, d - double, m - matrix, b - boolean, e - Error occured)
error - string containing the returned error.

Returns true if successful, otherwise returns false.
*/
//bool detect_variable(std::vector<std::string> words, int begin_idx, int end_idx , KVar& kv, std::string& varname, char& type, std::string* error);
bool detect_variable(std::vector<std::string> words, int begin_idx, int end_idx , KVar& kv, string& varname, char& type, string* error){

	if (end_idx - begin_idx > 1){
		type = 'e';
		*error = "ERROR: Too many words detected to left of assignment operator";
	}

	if (words[begin_idx] == DOUBLE_ID){
		type = 'd';
		if (1+begin_idx < words.size() && valid_varname(words[begin_idx+1], kv, error)){
			varname = words[begin_idx+1];
		}else{
			type = 'e';
            if (valid_varname(words[begin_idx+1], kv, error)){
                *error = "ERROR: Double variable was not able to be detected on left sign of assignment operator.";
            }
			return false;
		}
	}else if(words[begin_idx] == STRING_ID){
		type = 's';
		if (1+begin_idx < words.size() && valid_varname(words[begin_idx+1], kv, error)){
			varname = words[begin_idx+1];
		}else{
			type = 'e';
            if (valid_varname(words[begin_idx+1], kv, error)){
                *error = "ERROR: String variable was not able to be detected on left sign of assignment operator.";
            }
			return false;
		}
	}else if(words[begin_idx] == BOOL_ID){
		type = 'b';
		if (1+begin_idx < words.size() && valid_varname(words[begin_idx+1], kv, error)){
			varname = words[begin_idx+1];
		}else{
			type = 'e';
            if (valid_varname(words[begin_idx+1], kv, error)){
                *error = "ERROR: Boolean variable was not able to be detected on left sign of assignment operator.";
            }
            return false;
		}
	}else if(words[begin_idx] == MATRIX_ID){
		type = 'm';
		if (1+begin_idx < words.size() && valid_varname(words[begin_idx+1], kv, error)){
			varname = words[begin_idx+1];
		}else{
			type = 'e';
            if (valid_varname(words[begin_idx+1], kv, error)){
                *error = "ERROR: Matrix variable was not able to be detected on left sign of assignment operator.";
            }
			return false;
		}
	}else if( valid_varname(words[begin_idx], kv, error)){
		type = 'd';
		varname = words[begin_idx];
    }else if(is_varname(words[begin_idx], kv, error)){
        type = kv.get_var_type(words[begin_idx]);
        varname = words[begin_idx];
	}else{
		type = 'e';
		return false;
	}

	return true;
}

/*
Converts a char representing a KPack type to string.

in - char to convert

Returns string equivilent or UNKNOWN if not recognized.
*/
string char_to_typestr(char in){
	if (in == 'd'){
		return "double";
	}else if(in == 'm'){
		return "matrix";
	}else if(in == 's'){
		return "string";
	}else if(in == 'b'){
		return "bool";
	}else if(in == 'e'){
		return "error message";
	}else{
		return "UNKNOWN";
	}
}
