#include <ctype.h>

#include "kc_aux.hpp"
#include "string_manip.hpp"
#include "CLIK.hpp"

using namespace std;

vector<string> fill_keywords(){

	vector<string> keywords;

	keywords.push_back("exit"); //0
	keywords.push_back("set"); //1
	keywords.push_back("clear"); //2
	keywords.push_back("cls"); //3
	keywords.push_back("help"); //4
	keywords.push_back("clvar"); //5
	keywords.push_back("lsvar"); //6
	keywords.push_back("prcd");	//7
	keywords.push_back("svprg"); //8
	keywords.push_back("exec"); //9
	keywords.push_back("matrix"); //10
	keywords.push_back("bool"); //11
	keywords.push_back("string"); //12
	keywords.push_back("double"); //13

	return keywords;

}

bool add_matrix(vector<string> words , KVar& kv){

	//Declare local variables
	string name = "NO_NAME";
	string comment = "";

	//Check double keyword
	if (words.size() < 4 || words[0] != "matrix") return false;

	//Check name is valid
	string error;
	if (!valid_varname(words[1], kv, &error)){
		cout << '\t' << error << endl;
		return false;
	}
	name = words[1];

	//Check equal sign present
	if (words[2] != "=") return false;

	//Check double value is valid
	int last_word;
	string matrix_content;
	if (!next_phrase(words, &matrix_content, '[', ']', 3, &last_word)) return false;

	KMatrix value(matrix_content);

	//Check for semicolons, or descriptor comments
	bool semi_used = false;
	for (int i = last_word+1 ; i < words.size() ; i++){
		if (words[i] == "//"){
			cat_tokens(words, i+1, " ");
			break;
		}else if(words[i] == ";"){
			if (semi_used) return false;
			semi_used = true;
		}else{
			// cout << "\t\t\t>" << words[i] << "<" << endl;
			return false;
		}
	}

	kv.add_matrix(name, value, comment);

	return true;
}

bool add_bool(vector<string> words, KVar& kv ){

	//Declare local variables
	string name = "NO_NAME";
	bool value = false;
	string comment = "";

	//Check double keyword
	if (words.size() < 4 || words[0] != "bool") return false;

	//Check name is valid
	string error;
	if (!valid_varname(words[1], kv, &error)){
		cout << '\t' << error << endl;
		return false;
	}
	name = words[1];

	//Check equal sign present
	if (words[2] != "=") return false;

	//Check bool value is valid
	if (!next_bool(words[3], &value)) return false;

	//Check for semicolons, or descriptor comments
	bool semi_used = false;
	for (int i = 4 ; i < words.size() ; i++){
		if (words[i] == "//"){
			cat_tokens(words, i+1, " ");
			break;
		}else if(words[i] == ";"){
			if (semi_used) return false;
			semi_used = true;
		}else{
			cout << "\t\t\t>" << words[i] << "<" << endl;
			return false;
		}
	}

	kv.add_bool(name, value, comment);

	return true;
}

bool add_string(vector<string> words, KVar& kv ){

	//Declare local variables
	string name = "NO_NAME";
	string value = "";
	string comment = "";

	//Check double keyword
	if (words.size() < 4 || words[0] != "string") return false;

	//Check name is valid
	string error;
	if (!valid_varname(words[1], kv, &error)){
		cout << '\t' << error << endl;
		return false;
	}
	name = words[1];

	//Check equal sign present
	if (words[2] != "=") return false;

	//Check double value is valid
	int last_word;
	if (!next_string(words, &value, 3, &last_word)) return false;

	//Check for semicolons, or descriptor comments
	bool semi_used = false;
	for (int i = last_word+1 ; i < words.size() ; i++){
		if (words[i] == "//"){
			cat_tokens(words, i+1, " ");
			break;
		}else if(words[i] == ";"){
			if (semi_used) return false;
			semi_used = true;
		}else{
			// cout << "\t\t\t>" << words[i] << "<" << endl;
			return false;
		}
	}

	kv.add_string(name, value, comment);

	return true;
}

bool add_double(vector<string> words, KVar& kv){

	//Declare local variables
	string name = "NO_NAME";
	double value = -1;
	string comment = "";

	//Check double keyword
	if (words.size() < 4 || words[0] != "double") return false;

	//Check name is valid
	string error;
	if (!valid_varname(words[1], kv, &error)){
		cout << '\t' << error << endl;
		return false;
	}
	name = words[1];

	//Check equal sign present
	if (words[2] != "=") return false;

	//Check double value is valid
	if (!next_double(words[3], &value)) return false;

	//Check for semicolons, or descriptor comments
	bool semi_used = false;
	for (int i = 4 ; i < words.size() ; i++){
		if (words[i] == "//"){
			cat_tokens(words, i+1, " ");
			break;
		}else if(words[i] == ";"){
			if (semi_used) return false;
			semi_used = true;
		}else{
			cout << "\t\t\t>" << words[i] << "<" << endl;
			return false;
		}
	}

	kv.add_double(name, value, comment);

	return true;
}

bool valid_varname(string name, KVar& kv, string* error){

	*error = "";

    if (name == "double" || name == "string" || name == "matrix" || name == "bool"){
        *error = "ERROR: Variable cannot be a type keyword.";
        return false;
    }
    
	if (!isalpha(name[0])){
		*error = "ERROR: Variable name must begin with a letter.";
		return false;
	}

	for (int i = 0 ; i < name.length() ; i++){
		if (!(isalpha(name[i]) || name[i] == '_' || isnum(string(1, name[i])))){
			*error = "ERROR: Non-valid character detected in variable name. Must be alphanumeric or an underscore.";
			return false;
		}
	}

	if (is_varname(name, kv, error)){
		*error = "ERROR: Proposed new variable name already in use.";
		return false;
	}

	return true;
}

bool is_varname(string name, KVar& kv, string* error){

	vector<string> names = kv.get_variable_IDs();
	if (strvec_contains(names, name) != -1){
		return true;
	}

	return false;
}

bool is_operator(string in, string* op){

	remove_end_whitespace(in);

	if (in == "*"){
		*op = "*";
		return true;
	}else if (in == "/"){
		*op = "/";
		return true;
	}else if (in == "+"){
		*op = "+";
		return true;
	}else if (in == "-"){
		*op = "-";
		return true;
	}else if (in == "("){
		*op = "(";
		return true;
	}else if (in == ")"){
		*op = ")";
		return true;
	}else if (in == "^"){
		*op = "^";
		return true;
	}else if (in == "%"){
		*op = "%";
		return true;
	}else if (in == "=="){
		*op = "==";
		return true;
	}else if (in == ">"){
		*op = ">";
		return true;
	}else  if (in == "<"){
		*op = "<";
		return true;
	}else if (in == "<="){
		*op = "<=";
		return true;
	}else if (in == ">="){
		*op = ">=";
		return true;
	}else if (in == "!="){
		*op = "!=";
		return true;
	}else if (in == "&&"){
		*op = "&&";
		return true;
	}else if (in == "||"){
		*op = "||";
		return true;
	}else if (in == "!"){
		*op = "!";
		return true;
	}else{
		return false;
	}

}

bool interpret(std::string in, KVar& kv, char* return_type, double* double_return, bool* bool_return, std::string* string_return, KMatrix* matrix_return, string* error){

		/*
		Step 1: Group all words into tokens - each token is classified as one of the 4 datatypes or as an operator (Functions are evaluated and assigned their return type)
		Step 2: Verify legitimacy of sentence structure (AKA not SOM for example)
		Step 3: Evaluate through sentence
		Step 4: Return product
		*/

		*error = "";
		ensure_whitespace(in, "[];+-*=^%");
		ensure_whitespace_full(in, "//");
		vector<string> words = parse(in, " ");

		//Classify all tokens S,M,D,B,O
		// vector<char> classifications;
		vector<token> tokens;
		string str_out;
		bool bool_out;
		double double_out;
		int pointer = 0;
		int last;

string str2 = "\"Hello world\" True [1, 2; 3, 4]";

		next_string(words, &str_out, pointer, &last);

		cout << "str_out:" << str_out << "\npointer: " << pointer << "\nlast: " << last << endl;

		for (int i = 0 ; i < words.size() ; i++){

				cout << i << endl;

				try{
				if (next_string(words, &str_out, pointer, &last)){ //Is a string literal (value enclosed by quotes)
					pointer = last + 1;
					token nt;
					nt.type = 'S';
					nt.s  = str_out;
					tokens.push_back(nt);
				}else if(next_phrase(words, &str_out, '[', ']', pointer, &last)){ //Is a matrix literal (Matrix enclosed by brackets)
					pointer = last + 1;
					token nt;
					nt.type = 'M';
					KMatrix km_new(str_out);
					nt.m = km_new;
					tokens.push_back(nt);
				}else if(next_double(words[pointer], &double_out)){ //Double literal (floating point number)
					pointer = last + 1;
					token nt;
					nt.type = 'D';
					nt.s  = str_out;
					tokens.push_back(nt);
				}else if(next_bool(words[pointer], &bool_out)){ //Is a boolean literal (True or False - non-case sensitive)
					pointer = last + 1;
					token nt;
					nt.type = 'B';
					nt.s  = str_out;
					tokens.push_back(nt);
				}else if(is_varname(words[pointer], kv)){ //Is a used variable name - will turn variable into a literal
					token nt;
					nt.s  = str_out;
					tokens.push_back(nt);
					char type;
					if (!kv.get_var_index(words[pointer], NULL, &type)) return false;
					if (type == 'd'){
						nt.type = 'D';
						kv.get_double(words[pointer], double_out);
						nt.d = double_out;
					}else if (type == 'm'){
						nt.type = 'M';
						KMatrix km_new;
						kv.get_matrix(words[pointer], km_new);
						nt.m = km_new;
					}else if (type == 'b'){
						nt.type = 'B';
						kv.get_bool(words[pointer], bool_out);
						nt.b = bool_out;
					}else if (type == 's'){
						nt.type = 'S';
						kv.get_string(words[pointer], str_out);
						nt.s = str_out;
					}else if (type == 'k'){ //Should be able to be eliminated - just legacy support for old KVar versions
						nt.type = 'M';
						KMatrix km_new;
						kv.get_matrix(words[pointer], km_new);
						nt.m = km_new;
					}else{
						return false;
					}
					pointer = last + 1;
				}else if(is_operator(words[pointer], &str_out)){ //Is an operator
					pointer = last + 1;
					token nt;
					nt.type = 'O';
					nt.s  = str_out;
					tokens.push_back(nt);
				}
				}catch(...){}
		}


		for (int i = 0 ; i < tokens.size() ; i++){
			if (tokens[i].type == 'D'){
				cout << to_string(tokens[i].d) << endl;
			}else if(tokens[i].type == 'M'){
				tokens[i].m.print();
			}else  if(tokens[i].type == 'B'){
				cout << bool_to_str(tokens[i].b) << endl;
			}else  if(tokens[i].type == 'S'){
				cout << tokens[i].s << endl;
			}else  if(tokens[i].type == 'O'){
				cout << tokens[i].s << endl;
			}
		}



// PRIM	// "STRING contents"			S
		// [1, 2; 3, 4];				M
		// True							B
		// 1.654654684e15				D
// VAR		// variable_name				SMDB
// OPR		// * / ^ % - + ( )				O

		// D + - * / ^ % D				D
		// D == >= <= > < != D			B
		// S + S						S
		// S == S						B
		// M + - * M					M
		// M + - * / ^ D				M
		// M == M						B
		// B == >= <= > < != B			B
    return true;
}
