#include <vector>
#include "KInterpAux.hpp"
#include "KInterp.hpp"
#include "string_manip.hpp"
#include "stdutil.hpp"
#include "kc_aux.hpp"
#include "KIFunctions.hpp"
#include "CLIK.hpp"
#include "KMatrix.hpp"
#include "string_manip.hpp"

#define OPERATE_PRECISION 400 //15

using namespace std;

string this_file_name = "KInterp.cpp";

/*
Interprets a string as a mathematical expression. Accepts variables (specified in the KVar object), and will return the result of the expression.

input - input string to analyze
vars - KVar object containing variables to use in analysis (and to which to add if new variables are created)
out - Result of evaluation

Returns true if evaluation completed successfully and without error. Else returns false and reports the error code in 'out'.
*/
bool interpret(std::string input, KVar& vars, all_ktype& out, vector<func_id> interp_functions, bool allow_print){

	//Process input string (Ensure whitespace and parse)
	vector<string> words = space_and_parse_negatives(input);

    for (int i = 0 ; i < words.size() ; i++){
        if (words[i] == ""){
            words.erase(words.begin()+i);
        }
    }

//	Test Point 1
//	 for (int i = 0 ; i < words.size() ; i++){
//	 	std::cout << ">" << words[i] << "<" << endl;
//	 }

	/*--------------------------------------------------------------------------------
	------ Now program checks each operator and makes new calls to 'interpret()' -----
	--------------------------------------------------------------------------------*/

	//Check for equals signs
	for (int i = 0 ; i < words.size() ; i++){
		if (words[i].length() == 1 && words[i][0] == '='){ //Variable assignment detected, evaluate expression and assign to left side

			//Evaluate expression
			if (!interpret(cat_tokens(words, i+1, " "), vars, out, interp_functions)){
				return false;
			}

			//Detect the variable and it's type
			string varname;
			string error;
			char type;
			if (!detect_variable(words, 0, i-1, vars, varname, type, &error)){
				out.type = 'e';
				out.s = error;
				return false;
			}

			//Assign value to new variable
			if (type != out.type){
				out.s = "ERROR: Can not assign " + char_to_typestr(out.type) + " value to " + char_to_typestr(type) + " variable.";
				out.type = 'e';
				return false;
			}

			//Interpret expression
//            if (interpret(cat_tokens(words, i+1, " "), vars, out, interp_functions, allow_print)){
            switch (out.type) {
                case 'd':
                    if (!is_varname(words[i-1], vars, &error)){
                        vars.add_double(words[i-1], out.d);
                    }else{
                        vars.set_double(words[i-1], out.d);
                    }
                    break;
                case 'm':
                    if (!is_varname(words[i-1], vars, &error)){
                        vars.add_matrix(words[i-1], out.km);
                    }else{
                        vars.set_matrix(words[i-1], out.km);
                    }
                    break;
                case 'b':
                    if (!is_varname(words[i-1], vars, &error)){
                        vars.add_bool(words[i-1], out.b);
                    }else{
                        vars.set_bool(words[i-1], out.b);
                    }
                    break;
                case 's':
                    if (!is_varname(words[i-1], vars, &error)){
                        vars.add_string(words[i-1], out.s);
                    }else{
                        vars.set_string(words[i-1], out.s);
                    }
                    break;
                default:
                    break;
            }
            return true;
//            }else{
//                return false;
//            }

		}
	}

//     for (int i = 0 ; i < words.size() ; i++){
//     	std::cout << ">" << words[i] << "<" << endl;
//     }

    //Check for update-assignment operators
    for (int i = 0 ; i < words.size() ; i++){
        if (words[i].length() == 2 && words[i][1] == '='){ //Variable assignment detected, evaluate expression and assign to left side

//            cout << "Update assign!" << endl;

            if (words[i][0] == '=' || words[i][0] == '<' || words[i][0] == '>' || words[i][0] == '!') continue;

            //Evaluate expression
            if (!interpret(cat_tokens(words, i+1, " "), vars, out, interp_functions)){
                return false;
            }

            //Detect the variable and it's type
            string varname;
            string error;
            char type;
            if (!detect_variable(words, 0, i-1, vars, varname, type, &error)){
                out.type = 'e';
                out.s = error;
                return false;
            }

//            //Assign value to new variable
//            if (type != out.type){
//                out.s = "ERROR: Can not assign " + char_to_typestr(out.type) + " value to " + char_to_typestr(type) + " variable.";
//                out.type = 'e';
//                return false;
//            }

            //Verify that variable exists
            if (!is_varname(words[i-1], vars, &error)){
                out.s = "ERROR: Variable '" + words[i-1] + "' has not been declared and can not been used with an update-assign operator";
                out.type = 'e';
                return false;
            }

            //Interpret expression
            switch (type) {
                case 'd':{
                        double orig;
                        vars.get_variable(words[i-1], orig);

                        if (out.type != 'd'){
                            out.s = "ERROR: Must update-assign a double to a double";
                            out.type = 'e';
                            return false;
                        }

                        switch(words[i][0]){
                            case('+'):
                                orig = orig + out.d;
                                break;
                            case('-'):
                                orig = orig - out.d;
                                break;
                            case('*'):
                                orig = orig * out.d;
                                break;
                            case('/'):
                                orig = orig / out.d;
                                break;
                            case('^'):
                                orig = pow(orig, out.d);
                                break;
                        }

                        vars.set_variable(words[i-1], orig);

                        //Set output akt to be equal to the variable's new value
                        out.type = 'd';
                        out.d = orig;

                    }
                    break;
                case 'm':{
                        KMatrix orig;
                        vars.get_variable(words[i-1], orig);

                        if (out.type == 'd'){

                            switch(words[i][0]){
                                case('+'):
                                    orig.add_double(out.d);
                                    break;
                                case('-'):
                                    orig.add_double(-1*out.d);
                                    break;
                                case('*'):
                                    orig.multiply_double(out.d);
                                    break;
                                case('/'):
                                    orig.multiply_double(1/out.d);
                                    break;
                                case('^'):
                                    orig.power_double(out.d);
                                    break;
                                default:
                                    out.s = "ERROR: Can only apply add, subtract, multiply, divide, and exponent operator to strings.";
                                    out.type = 'e';
                                    return false;
                                    break;
                            }

                            vars.set_variable(words[i-1], orig);
                        }else if(out.type == 'm'){
                            switch(words[i][0]){
                                case('+'):
                                    orig = orig + out.km;
                                    break;
                                case('-'):
                                    orig = orig - out.km;
                                    break;
                                case('*'):
                                    orig = orig * out.km;
                                    break;
                                default:
                                    out.s = "ERROR: Can only apply add, subtract, and multiply operator to strings.";
                                    out.type = 'e';
                                    return false;
                                    break;
                            }

                            vars.set_variable(words[i-1], orig);
                        }else{
                            out.s = "ERROR: Must update-assign a double or matrix to a matrix";
                            out.type = 'e';
                            return false;
                        }

                        //Set output akt to be equal to the variable's new value
                        out.type = 'm';
                        out.km = orig;

                    }
                    break;
                case 'b'://{
//                        bool orig;
//                        vars.get_variable(words[i-1], orig);
//
//                        if (out.type == 'd'){
//                            orig.add_double(out.d);
//                            vars.set_variable(words[i-1], orig);
//                        }else if(out.type == 'm'){
//                            vars.set_variable(words[i-1], orig + out.km);
//                        }else{
//                            out.s = "ERROR: Must update-assign a double or matrix to a matrix";
//                            out.type = 'e';
//                            return false;
//                        }
//                    }
                    break;
                case 's':{
                        string orig;
                        vars.get_variable(words[i-1], orig);

                        if (words[i][0] != '+'){
                            out.s = "ERROR: Can only apply add operator to strings.";
                            out.type = 'e';
                            return false;
                        }

                        if (out.type == 's'){
                            vars.set_variable(words[i-1], orig + out.s);
                        }else{
                            out.s = "ERROR: Must update-assign a string to a string.";
                            out.type = 'e';
                            return false;
                        }

                    //Set output akt to be equal to the variable's new value
                    out.type = 's';
                    out.s = orig + out.s;

                    }
                    break;
                default:
                    break;
            }



            return true;
        }
    }

//    cout << "About to convert vars to lits" << endl;
//     for (int i = 0 ; i < words.size() ; i++){
//         std::cout << ">" << words[i] << "<" << endl;
//     }

    //Check for implied 'ans'
    if (words.size() > 0 && words[0].size() > 0 && str_to_op(words[0]) && words[0] != "!"){
        if (words[0] == "-"){
            words.insert(words.begin(), "0");
        }else{
            words.insert(words.begin(), "ans");
        }
    }

	//Convert variables to literals
	for (int i = 0 ; i < words.size() ; i++){

		if (is_varname(words[i], vars, NULL)){
			char type = vars.get_var_type(words[i]);
			if (type == 'd'){
				double double_out;
				if (!vars.get_double(words[i], double_out)){
					out.type ='e';
					out.s = "ERROR: Failed to locate double variable '" + words[i] + "'";
					return false;
				}

                int replace_start_idx = idx_from_word_idx(input, words, i, POSITION_WORD_START, 0);
                int replace_end_idx = idx_from_word_idx(input, words, i, POSITION_WORD_END, 0);

				words[i] = hp_string(double_out, OPERATE_PRECISION);
                //modify 'input' (origional string), so variable name is replaced by hp_string's output

                input = input.substr(0, replace_start_idx) + words[i] + input.substr(replace_end_idx+1);

			}else if(type == 'b'){
				bool bool_out;
				if (!vars.get_bool(words[i], bool_out)){
					out.type = 'e';
					out.s = "ERROR: Failed to locate bool variable '" + words[i] + "'";
					return false;
				}
                int replace_start_idx = idx_from_word_idx(input, words, i, POSITION_WORD_START, 0);
                int replace_end_idx = idx_from_word_idx(input, words, i, POSITION_WORD_END, 0);
				words[i] = bool_to_str(bool_out, true);
                input = input.substr(0, replace_start_idx) + words[i] + input.substr(replace_end_idx+1);
			}else if(type == 's'){
				string str_out;
				if (!vars.get_string(words[i], str_out)){
					out.type = 'e';
					out.s = "ERROR: Failed to locate string variable '" + words[i] + "'";
					return false;
				}
                int replace_start_idx = idx_from_word_idx(input, words, i, POSITION_WORD_START, 0);
                int replace_end_idx = idx_from_word_idx(input, words, i, POSITION_WORD_END, 0);
                inject_words_into_strvec(words, '\"'+str_out+'\"', i, i);
                input = input.substr(0, replace_start_idx) + '\"'+str_out+'\"' + input.substr(replace_end_idx+1);
				// words[i] = str_out;
			}else if(type == 'm'){
				KMatrix mat_out;
				if (!vars.get_matrix(words[i], mat_out)){
					out.type = 'e';
					out.s = "ERROR: Failed to locate matrix variable '" + words[i] + "'";
					return false;
				}
                int replace_start_idx = idx_from_word_idx(input, words, i, POSITION_WORD_START, 0);
                int replace_end_idx = idx_from_word_idx(input, words, i, POSITION_WORD_END, 0);
                string temp = mat_out.get_string(); //This is used so that if get_string() behavior changes (maybe due to updates to function in future, same string will still be used in next two lines.
				inject_words_into_strvec(words, temp, i, i);
                input = input.substr(0, replace_start_idx) + temp + input.substr(replace_end_idx+1);
				// words[i] = mat_out.get_string();
			}else{
				out.type = 'e';
				out.s = "SOFTWARE ERROR: Failed to determine variable type. Problem in file: " + this_file_name;
				return false;
			}
		}
	}

//     // Test Point 3
//    cout << "About to eval functions" << endl;
//     for (int i = 0 ; i < words.size() ; i++){
//         std::cout << ">" << words[i] << "<" << endl;
//     }

	//Evaluate Functions
	for (int i = 0 ; i < words.size() ; i++){

		for (int j = 0 ; j < interp_functions.size() ; j++){
			if (interp_functions[j].identifier == words[i]){ //If interpreted function is found, parse its arguments and evaluate

				//Verify next char is OPAREN
				if (i+1 >= words.size() || words[i+1] != "("){
					out.type = 'e';
					out.s = "ERROR: Misuse of function '" + words[i] +"()'";
					return false;
				}
				i += 2;

				//Read function arguments
				int k;
				vector<all_ktype> arguments;
				all_ktype temp_akt;
				vector<string> argument;
				int paren_level = 1;
				// double out_double;
				// KMatrix out_mat;
				// bool out_bool;
				// int last_word;
				// string out_str;
                bool paren_closed = false;
				for (k = i; k < words.size() ; k++){
//                    cout << "\t\t" << words[k] << endl;
					if (words[k] == ")"){ //Look for CPAREN char
						if (paren_level == 1){
							if (!interpret(cat_tokens(argument, 0, " "), vars, temp_akt, interp_functions)){
								out.type = 'e';
								out.s = "ERROR: Failed to evaluate fuction argument: '" + cat_tokens(argument, 0, " ") + "'. (" + temp_akt.s + ")";
								return false;
							}
							arguments.push_back(temp_akt);
                            paren_closed = true;
							break;
						}else{
							paren_level--;
                            argument.push_back(")");
						}
					}else if(words[k] == "("){
						paren_level++;
                        argument.push_back("(");
					}else if(words[k] == "," && paren_level == 1){ //Is argument to function
						if (!interpret(cat_tokens(argument, 0, " "), vars, temp_akt, interp_functions)){
							out.type = 'e';
							out.s = "ERROR: Failed to evaluate fuction argument: '" + cat_tokens(argument, 0, " ") + "'. (" + temp_akt.s + ")";
							return false;
						}
						arguments.push_back(temp_akt);
                        argument.clear();
					}else{
						argument.push_back(words[k]);
					}
				} //(END FOR LOOP)

                if (!paren_closed){
                    out.type = 'e';
                    out.s = "ERROR: Function parenthesis were not closed.";
                    return false;
                }

				//Evaluate Function and replace function with literal
				all_ktype result = (interp_functions[j].function)(arguments);
				if (result.type == 'e'){
					out.type = 'e';
					out.s = result.s;
					return false;
				}else if(result.type == 'm'){
					inject_words_into_strvec(words, result.km.get_string(), i-2, k);
				}else if(result.type == 's'){
					inject_words_into_strvec(words, result.s, i-2, k);
				}else if(result.type == 'd'){
					inject_words_into_strvec(words, hp_string(result.d), i-2, k);
				}else if(result.type == 'b'){
					inject_words_into_strvec(words, bool_to_str(result.b), i-2, k);
				}else{
					out.type = 'e';
					out.s = "SOFTWARE ERROR: Improper type returned by interpreted function '" + interp_functions[j].identifier + "'. This error was detected in file: " + this_file_name;
					return false;
				}

			}
		}

	}

	// //Test Point 4
//    cout << "About to eval parentheses and brackets" << endl;
//     for (int i = 0 ; i < words.size() ; i++){
//         std::cout << ">" << words[i] << "<" << endl;
//     }

	//Evaluate paretheses and brackets (same as parenthesis according to CLC)
	for (int i = 0 ; i < words.size() ; i++){

		if (words[i] == "(" || words[i] == "{"){ //Parentheses detected

			int paren_level = 1;
			int j;
			vector<string> paren_expression;
			for (j = i+1 ; j < words.size() ; j++){ //Locate what expression is contained in the parentheses
				if (words[j] == "{" || words[j] == "("){
					paren_level++;
				}else if(words[j] == ")" || words[j] == "}"){
					if (paren_level == 1){
						break;
					}else{
						paren_level--;
					}
				}
				paren_expression.push_back(words[j]);
			}

			//Replace contents of parentheses with evaluated expression
			all_ktype result;
			if (!interpret(cat_tokens(paren_expression, 0, " "), vars, result, interp_functions)){
				out.type = 'e';
				out.s = "ERROR: Failed to interpret expression '" + cat_tokens(paren_expression, 0, " ") + "'.";
				return false;
			}
			if (result.type == 'e'){
				out.type = 'e';
				out.s = result.s;
				return false;
			}else if(result.type == 'm'){
				inject_words_into_strvec(words, result.km.get_string(), i, j);
			}else if(result.type == 's'){
				inject_words_into_strvec(words, result.s, i, j);
			}else if(result.type == 'd'){
				inject_words_into_strvec(words, hp_string(result.d), i, j);
			}else if(result.type == 'b'){
				inject_words_into_strvec(words, bool_to_str(result.b), i, j);
			}else{
				out.type = 'e';
				out.s = "SOFTWARE ERROR: Improper type returned by interpreted function '" + interp_functions[j].identifier + "'. This error was detected in file: " + this_file_name;
				return false;
			}


		}

	}

	//Test Point 5
//    for (int i = 0 ; i < words.size() ; i++){
//        std::cout << "TP5>" << words[i] << "< Input: " << input << endl;
//    }

	//Break sentence into vector of all_ktype for easier parsing.
	std::vector<all_ktype> tokens;
	all_ktype temp_akt;
	string out_str;
	int last_word;
	double out_double;
	bool out_bool;
    int last_index = 0;
    string str_to_double_error = "";
	for (int k = 0 ; k < words.size() ; k++){
//        if (next_string(words, &out_str, k, &last_word)){ //TODO: Follow this chain and determine how to improve operation
        if (next_phrase(words, input, &out_str, '\"', '\"', k, &last_word, &last_index)){
//            bool next_phrase(std::vector<std::string> in, std::string in_full, std::string* value, char starter, char ender, int start_word, int* last_word, int* last_index, bool allow_before=false);
			k = last_word;
			temp_akt.type = 's';
			temp_akt.s = out_str;
			tokens.push_back(temp_akt);
        }else if(next_phrase(words, input, &out_str, '[', ']', k, &last_word, &last_index)){
//        }else if(next_phrase(words, &out_str, '[', ']', k, &last_word)){
			k = last_word;
			temp_akt.type = 'm';
            KMatrix temp_mat(out_str);
            if (temp_mat.is_open()){
                temp_akt.km = temp_mat;
                tokens.push_back(temp_akt);
//                cout << "KM Str: " << temp_akt.km.get_string() << endl;
//                cout << to_string(temp_akt.km(0, 0)*1e34) << endl;
            }else{
                out.type = 'e';
                out.s = "ERROR: Failed to create matrix from string \"" + out_str + "\"";
                return false;
            }
		}else if(str_to_double(words[k], out_double, str_to_double_error, "") || (str_to_double_error != "")){
            if (str_to_double_error != ""){
                out.type = 'e';
                out.s = str_to_double_error;
                return false;
            }
            double empty_double;
			if (k > 0 && words[k-1] == "-" && !(k > 1 && (str_to_double(words[k-2], empty_double, "") || words[k-2][words[k-2].length()-1] == ']'))){ //previous word is '-' and 2-prior word isn't a double or KMatrix
                out_double *= -1;
			}
			temp_akt.type = 'd';
			temp_akt.d = out_double;
//            if (out_double < 0 && tokens.size() > 0 && tokens[tokens.size()-1].type == 'o' && tokens[tokens.size()-1].s == "-"){
//                tokens.pop_back();
//            }
			tokens.push_back(temp_akt);
		}else if(str_to_bool(words[k], out_bool)){
			temp_akt.type = 'b';
			temp_akt.b = out_bool;
			tokens.push_back(temp_akt);
		}else if(str_to_op(words[k])){
			temp_akt.type = 'o';
			temp_akt.s = words[k];
			tokens.push_back(temp_akt);
		}else{
			out.type = 'e';
			out.s = "ERROR: Unidentified variable '"+ words[k] +"'";
			return false;
		}
	}

	//Test Point 6
//	for (int i = 0 ; i < words.size() ; i++){
//		std::cout << "TP6>" << words[i] << "<" << input<<  endl;
//	}
//    for (int i = 0 ; i < tokens.size() ; i++){
//        std::cout << akt_tostring(tokens[i]) << endl;
//    }

    //Evaluate implied 'ans'

    /*====================================================================*\
    |================== EVALUATE SIMPLIFIED EXPRESSION ====================|
    \*====================================================================*/

    //Evaluate exponents
	all_ktype a, b;
    int start_idx = 0;
    int end_idx = 0;
	for (int i = 0 ; i < tokens.size() ; i++){
		if (tokens[i].type == 'o'){
			if(tokens[i].s == "^"){

                //Determine 'a'
                if (i == 0){
                    out.type = 'e';
                    out.s = "SOFTWARE ERROR: Implied 'ans' failed to evaluate! This error was detected in file" + this_file_name;
                    return false;
                }else{
                    a = tokens[i-1];
                    start_idx = i-1;
                }

                //Determine 'b'
                if (i+1 >= tokens.size()){
                    out.type = 'e';
                    out.s = "ERROR: Operator '^' requires second argument.";
                    return false;
                }else{
                    b = tokens[i+1];
                    end_idx = i+1;
                }

                //Perform operation
                if (a.type == 'd' && b.type =='d'){ //Double ^ double
                    a.d = pow(a.d, b.d);
                    a.type = 'd';
                }else if(a.type == 'd' && b.type == 'm'){ //Double ^ matrix
                    for (int r = 0 ; r < b.km.rows() ; r++){
                        for (int c = 0 ; c < b.km.cols() ; c++){
                            b.km(r, c) = pow(b.km(r,c), a.d);
                        }
                    }
                    a.km = b.km;
                    a.type = 'm';
                }else if(a.type == 'm' && b.type == 'd'){ //matrix ^ double
                    for (int r = 0 ; r < b.km.rows() ; r++){
                        for (int c = 0 ; c < b.km.cols() ; c++){
                            a.km(r, c) = pow(a.km(r, c), b.d);
                        }
                    }
                    a.type = 'm';
                }else{
                    out.type = 'e';
                    out.s = "ERROR: Exponent operator (^) can only operate on doubles and matricies.";
                    return false;
                }

                inject_akt_into_aktvec(tokens, a, start_idx, end_idx);
                i = start_idx;
			}
        }
	}

    //Evaluate Multiplication, division, modulus
    for (int i = 0 ; i < tokens.size() ; i++){
        if (tokens[i].type == 'o'){
            if(tokens[i].s == "*"){
                //Determine 'a'
                if (i == 0){
                    out.type = 'e';
                    out.s = "SOFTWARE ERROR: Implied 'ans' failed to evaluate! This error was detected in file" + this_file_name;
                    return false;
                }else{
                    a = tokens[i-1];
                    start_idx = i-1;
                }

                //Determine 'b'
                if (i+1 >= tokens.size()){
                    out.type = 'e';
                    out.s = "ERROR: Operator '*' requires second argument.";
                    return false;
                }else{
                    b = tokens[i+1];
                    end_idx = i+1;
                }

                //Perform operation
                if (a.type == 'd' && b.type =='d'){ //Double * double
                    a.d = a.d * b.d;
                    a.type = 'd';
                }else if(a.type == 'd' && b.type == 'm'){ //Double * matrix
                    for (int r = 0 ; r < b.km.rows() ; r++){
                        for (int c = 0 ; c < b.km.cols() ; c++){
                            b.km(r, c) *= a.d;
                        }
                    }
                    a.km = b.km;
                    a.type = 'm';
                }else if(a.type == 'm' && b.type == 'd'){ //matrix * double
                    for (int r = 0 ; r < b.km.rows() ; r++){
                        for (int c = 0 ; c < b.km.cols() ; c++){
                            a.km(r, c) *= b.d;
                        }
                    }
                    a.type = 'm';
                }else if(a.type == 'm' && b.type == 'm'){ //Matrix * Matrix
                    if (a.km.cols() != b.km.rows()){
                        out.type = 'e';
                        out.s = "ERROR: Matricies can only be multiplied if dimensions match pattern AxB * B*C where A, B, and C are positive integers.";
                        return false;
                    }
                    a.km = a.km * b.km;
                    a.type = 'm';
                }else{
                    out.type = 'e';
                    out.s = "ERROR: Multiplication operator (*) can only operate on doubles and matricies.";
                    return false;
                }

                inject_akt_into_aktvec(tokens, a, start_idx, end_idx);
                i = start_idx;

            }else if(tokens[i].s == "/"){
                //Determine 'a'
                if (i == 0){
                    out.type = 'e';
                    out.s = "SOFTWARE ERROR: Implied 'ans' failed to evaluate! This error was detected in file" + this_file_name;
                    return false;
                }else{
                    a = tokens[i-1];
                    start_idx = i-1;
                }

                //Determine 'b'
                if (i+1 >= tokens.size()){
                    out.type = 'e';
                    out.s = "ERROR: Operator '/' requires second argument.";
                    return false;
                }else{
                    b = tokens[i+1];
                    end_idx = i+1;
                }

                //Perform operation
                if (a.type == 'd' && b.type =='d'){ //Double / double
                    a.d = a.d / b.d;
                    a.type = 'd';
                }else if(a.type == 'd' && b.type == 'm'){ //Double / matrix
                    for (int r = 0 ; r < b.km.rows() ; r++){
                        for (int c = 0 ; c < b.km.cols() ; c++){
                            b.km(r, c) /= a.d;
                        }
                    }
                    a.km = b.km;
                    a.type = 'm';
                }else if(a.type == 'm' && b.type == 'd'){ //matrix / double
                    for (int r = 0 ; r < b.km.rows() ; r++){
                        for (int c = 0 ; c < b.km.cols() ; c++){
                            a.km(r, c) /= b.d;
                        }
                    }
                    a.type = 'm';
                }else{
                    out.type = 'e';
                    out.s = "ERROR: Division operator (/) can only operate on doubles and matricies.";
                    return false;
                }

                inject_akt_into_aktvec(tokens, a, start_idx, end_idx);
                i = start_idx;
            }else if(tokens[i].s == "%"){

                //Determine 'a'
                if (i == 0){
                    out.type = 'e';
                    out.s = "SOFTWARE ERROR: Implied 'ans' failed to evaluate! This error was detected in file" + this_file_name;
                    return false;
                }else{
                    a = tokens[i-1];
                    start_idx = i-1;
                }

                //Determine 'b'
                if (i+1 >= tokens.size()){
                    out.type = 'e';
                    out.s = "ERROR: Operator '%' requires second argument.";
                    return false;
                }else{
                    b = tokens[i+1];
                    end_idx = i+1;
                }

                //Perform operation
                if (a.type == 'd' && b.type =='d'){ //Double % double
                    a.d = (int)a.d % (int)b.d;
                    a.type = 'd';
                }else if(a.type == 'd' && b.type == 'm'){ //Double % matrix
                    for (int r = 0 ; r < b.km.rows() ; r++){
                        for (int c = 0 ; c < b.km.cols() ; c++){
                            b.km(r, c) = (int)b.km(r, c) % (int)a.d;
                        }
                    }
                    a.km = b.km;
                    a.type = 'm';
                }else if(a.type == 'm' && b.type == 'd'){ //matrix % double
                    for (int r = 0 ; r < b.km.rows() ; r++){
                        for (int c = 0 ; c < b.km.cols() ; c++){
                            a.km(r, c) = (int)a.km(r,c) % (int)b.d;
                        }
                    }
                    a.type = 'm';
                }else{
                    out.type = 'e';
                    out.s = "ERROR: Modulus operator (%) can only operate on doubles and matricies.";
                    return false;
                }

                inject_akt_into_aktvec(tokens, a, start_idx, end_idx);
                i = start_idx;

            }
        }
    }

    //Evaluate addition & subtration
    for (int i = 0 ; i < tokens.size() ; i++){
        if (tokens[i].type == 'o'){
            if (tokens[i].s == "+"){
                //Determine 'a'
                if (i == 0){
                    out.type = 'e';
                    out.s = "SOFTWARE ERROR: Implied 'ans' failed to evaluate. This error was detected in file '" + this_file_name + "'";
                    return false;
                }else{
                    a = tokens[i-1];
                    start_idx = i-1;
                }

                //Determine 'b'
                if (i+1 >= tokens.size()){
                    out.type = 'e';
                    out.s = "ERROR: Operator '+' requires second argument.";
                    return false;
                }else{
                    b = tokens[i+1];
                    end_idx = i+1;
                }

                //Perform operation
                if (a.type == 'd' && b.type =='d'){ //Double + double
                    a.d = a.d + b.d;
                }else if(a.type == 'd' && b.type == 'm'){ //Double + matrix
                    for (int r = 0 ; r < b.km.rows() ; r++){
                        for (int c = 0 ; c < b.km.cols() ; c++){
                            b.km(r, c) += a.d;
                        }
                    }
                    a.km = b.km;
                    a.type = 'm';
                }else if(a.type == 'm' && b.type == 'd'){ //matrix + double
                    for (int r = 0 ; r < b.km.rows() ; r++){
                        for (int c = 0 ; c < b.km.cols() ; c++){
                            a.km(r, c) += b.d;
                        }
                    }
                    a.type = 'm';
                }else if(a.type == 'm' && b.type == 'm'){ //Matrix + Matrix
                    a.km = a.km + b.km;
                    a.type = 'm';
                }else if(a.type == 's' && b.type == 's'){ //String + String
                    a.s = a.s + b.s;
                    a.type = 's';
                }else{
                    out.type = 'e';
                    out.s = "ERROR: Addition operator (+) can only operate on doubles, matricies, and strings.";
                    return false;
                }

                inject_akt_into_aktvec(tokens, a, start_idx, end_idx);
                i = start_idx;

            }else if(tokens[i].s == "-"){
                //Determine 'a'
                if (i == 0){
                    out.type = 'e';
                    out.s = "SOFTWARE ERROR: Implied 'ans' failed to evaluate! This error was detected in file" + this_file_name;
                    return false;
                }else{
                    a = tokens[i-1];
                    start_idx = i-1;
                }

                //Determine 'b'
                if (i+1 >= tokens.size()){
                    out.type = 'e';
                    out.s = "ERROR: Operator '-' requires second argument.";
                    return false;
                }else{
                    b = tokens[i+1];
                    end_idx = i+1;
                }

                //Perform operation
                if (a.type == 'd' && b.type =='d'){ //Double - double
                    a.d = a.d - b.d;
                    a.type = 'd';
                }else if(a.type == 'd' && b.type == 'm'){ //Double - matrix
                    vector<vector<double> > d_to_km;
                    vector<double> mid_vec;
                    for (int r = 0 ; r < b.km.rows() ; r++){
                        for (int c = 0 ; c < b.km.cols() ; c++){
                            mid_vec.push_back(a.d);
                        }
                        d_to_km.push_back(mid_vec);
                    }
                    KMatrix temp_km(d_to_km);
                    a.km = b.km - temp_km;
                    a.type = 'm';
                }else if(a.type == 'm' && b.type == 'd'){ //matrix - double
                    vector<vector<double> > d_to_km;
                    vector<double> mid_vec;
                    for (int r = 0 ; r < b.km.rows() ; r++){
                        for (int c = 0 ; c < b.km.cols() ; c++){
                            mid_vec.push_back(b.d);
                        }
                        d_to_km.push_back(mid_vec);
                    }
                    KMatrix temp_km(d_to_km);
                    a.km = a.km - temp_km;
                    a.type = 'm';
                }else if(a.type == 'm' && b.type == 'm'){ //Matrix - Matrix
                    a.km = a.km - b.km;
                    a.type = 'm';
                }else{
                    out.type = 'e';
                    out.s = "ERROR: Subtraction operator (-) can only operate on doubles and matricies.";
                    return false;
                }

                inject_akt_into_aktvec(tokens, a, start_idx, end_idx);
                i = start_idx;

            }
        }
    }

    //Evaluate &&, ||, !
    for (int i = 0 ; i < tokens.size() ; i++){
        if (tokens[i].type == 'o'){
            if(tokens[i].s == "&&"){
                //Determine 'a'
                if (i == 0){
                    out.type = 'e';
                    out.s = "SOFTWARE ERROR: Implied 'ans' failed to evaluate! This error was detected in file" + this_file_name;
                    return false;
                }else{
                    a = tokens[i-1];
                    start_idx = i-1;
                }

                //Determine 'b'
                if (i+1 >= tokens.size()){
                    out.type = 'e';
                    out.s = "ERROR: Operator '&&' requires second argument.";
                    return false;
                }else{
                    b = tokens[i+1];
                    end_idx = i+1;
                }

                //Perform operation
                if (a.type == 'b' && b.type =='b'){ //bool & bool
                    a.b = a.b && b.b;
                }else{
                    out.type = 'e';
                    out.s = "ERROR: And operator (&&) can only operate on bools.";
                    return false;
                }

                inject_akt_into_aktvec(tokens, a, start_idx, end_idx);
                i = start_idx;
            }else if(tokens[i].s == "||"){
                //Determine 'a'
                if (i == 0){
                    out.type = 'e';
                    out.s = "SOFTWARE ERROR: Implied 'ans' failed to evaluate! This error was detected in file" + this_file_name;
                    return false;
                }else{
                    a = tokens[i-1];
                    start_idx = i-1;
                }

                //Determine 'b'
                if (i+1 >= tokens.size()){
                    out.type = 'e';
                    out.s = "ERROR: Operator '||' requires second argument.";
                    return false;
                }else{
                    b = tokens[i+1];
                    end_idx = i+1;
                }

                //Perform operation
                if (a.type == 'b' && b.type =='b'){ //bool & bool
                    a.b = a.b || b.b;
                }else{
                    out.type = 'e';
                    out.s = "ERROR: Or operator (||) can only operate on bools.";
                    return false;
                }

                inject_akt_into_aktvec(tokens, a, start_idx, end_idx);
                i = start_idx;
            }else if(tokens[i].s == "!" && tokens.size() > (i+1) && tokens[i+1].type == 'b' ){
                a = tokens[i+1];
                a.b = !a.b;
                inject_akt_into_aktvec(tokens, a, i, i+1);
            }
        }
    }

    //Evaluate ==, !=, >=, <=, >, <
    for (int i = 0 ; i < tokens.size() ; i++){
        if (tokens[i].type == 'o'){
            if(tokens[i].s == "=="){
                //Determine 'a'
                if (i == 0){
                    out.type = 'e';
                    out.s = "SOFTWARE ERROR: Implied 'ans' failed to evaluate! This error was detected in file" + this_file_name;
                    return false;
                }else{
                    a = tokens[i-1];
                    start_idx = i-1;
                }

                //Determine 'b'
                if (i+1 >= tokens.size()){
                    out.type = 'e';
                    out.s = "ERROR: Operator '==' requires second argument.";
                    return false;
                }else{
                    b = tokens[i+1];
                    end_idx = i+1;
                }

                //Perform operation
                if (a.type == 'b' && b.type =='b'){ //bool & bool
                    a.b = a.b == b.b;
                }else if(a.type == 'd' && b.type == 'd'){
                    a.b = (a.d == b.d);
                }else if(a.type == 'm' && b.type == 'm'){
                    a.km -= b.km;
                    a.b = true;
                    for (int r = 0; r < a.km.rows() ; r++){
                        for (int c = 0 ; c < a.km.cols() ; c++){
                            if (a.km(r, c) != 0){
                                a.b = false;
                                break;
                            }
                        }
                        if (!a.b) break;
                    }
                }else if(a.type == 's' && b.type == 's'){
                    a.b = (a.s == b.s);
                }else{
                    out.type = 'e';
                    out.s = "ERROR: equals operator (==) can only operate on variables of the same type.";
                    return false;
                }
                a.type = 'b';

                inject_akt_into_aktvec(tokens, a, start_idx, end_idx);
                i = start_idx;
            }else if(tokens[i].s == "!="){
                //Determine 'a'
                if (i == 0){
                    out.type = 'e';
                    out.s = "SOFTWARE ERROR: Implied 'ans' failed to evaluate! This error was detected in file" + this_file_name;
                    return false;
                }else{
                    a = tokens[i-1];
                    start_idx = i-1;
                }

                //Determine 'b'
                if (i+1 >= tokens.size()){
                    out.type = 'e';
                    out.s = "ERROR: Operator '!=' requires second argument.";
                    return false;
                }else{
                    b = tokens[i+1];
                    end_idx = i+1;
                }

                //Perform operation
                if (a.type == 'b' && b.type =='b'){ //bool & bool
                    a.b = a.b != b.b;
                }else if(a.type == 'd' && b.type == 'd'){
                    a.b = (a.d != b.d);
                }else if(a.type == 'm' && b.type == 'm'){
                    a.km -= b.km;
                    a.b = false;
                    for (int r = 0; r < a.km.rows() ; r++){
                        for (int c = 0 ; c < a.km.cols() ; c++){
                            if (a.km(r, c) != 0){
                                a.b = true;
                                break;
                            }
                        }
                        if (!a.b) break;
                    }
                }else if(a.type == 's' && b.type == 's'){
                    a.b = (a.s != b.s);
                }else{
                    out.type = 'e';
                    out.s = "ERROR: equals operator (==) can only operate on variables of the same type.";
                    return false;
                }
                a.type = 'b';

                inject_akt_into_aktvec(tokens, a, start_idx, end_idx);
                i = start_idx;
            }else if(tokens[i].s == "<="){
                //Determine 'a'
                if (i == 0){
                    out.type = 'e';
                    out.s = "SOFTWARE ERROR: Implied 'ans' failed to evaluate! This error was detected in file" + this_file_name;
                    return false;
                }else{
                    a = tokens[i-1];
                    start_idx = i-1;
                }

                //Determine 'b'
                if (i+1 >= tokens.size()){
                    out.type = 'e';
                    out.s = "ERROR: Operator '>=' requires second argument.";
                    return false;
                }else{
                    b = tokens[i+1];
                    end_idx = i+1;
                }

                //Perform operation
                if (a.type == 'b' && b.type =='b'){ //bool & bool
                    a.b = a.b <= b.b;
                }else if(a.type == 'd' && b.type == 'd'){
                    a.b = (a.d <= b.d);
                }else if(a.type == 'm' && b.type == 'm'){
                    a.km -= b.km;
                    a.b = true;
                    for (int r = 0; r < a.km.rows() ; r++){
                        for (int c = 0 ; c < a.km.cols() ; c++){
                            if (a.km(r, c) > 0){
                                a.b = false;
                                break;
                            }
                        }
                        if (!a.b) break;
                    }
                }else if(a.type == 's' && b.type == 's'){
                    a.b = (a.s <= b.s);
                }else{
                    out.type = 'e';
                    out.s = "ERROR: equals operator (==) can only operate on variables of the same type.";
                    return false;
                }
                a.type = 'b';

                inject_akt_into_aktvec(tokens, a, start_idx, end_idx);
                i = start_idx;
            }else if(tokens[i].s == ">="){
                //Determine 'a'
                if (i == 0){
                    out.type = 'e';
                    out.s = "SOFTWARE ERROR: Implied 'ans' failed to evaluate! This error was detected in file" + this_file_name;
                    return false;
                }else{
                    a = tokens[i-1];
                    start_idx = i-1;
                }

                //Determine 'b'
                if (i+1 >= tokens.size()){
                    out.type = 'e';
                    out.s = "ERROR: Operator '<=' requires second argument.";
                    return false;
                }else{
                    b = tokens[i+1];
                    end_idx = i+1;
                }

                //Perform operation
                if (a.type == 'b' && b.type =='b'){ //bool & bool
                    a.b = a.b >= b.b;
                }else if(a.type == 'd' && b.type == 'd'){
                    a.b = (a.d >= b.d);
                }else if(a.type == 'm' && b.type == 'm'){
                    a.km -= b.km;
                    a.b = true;
                    for (int r = 0; r < a.km.rows() ; r++){
                        for (int c = 0 ; c < a.km.cols() ; c++){
                            if (a.km(r, c) < 0){
                                a.b = false;
                                break;
                            }
                        }
                        if (!a.b) break;
                    }
                }else if(a.type == 's' && b.type == 's'){
                    a.b = (a.s >= b.s);
                }else{
                    out.type = 'e';
                    out.s = "ERROR: equals operator (==) can only operate on variables of the same type.";
                    return false;
                }

                a.type = 'b';

                inject_akt_into_aktvec(tokens, a, start_idx, end_idx);
                i = start_idx;
            }else if(tokens[i].s == ">"){
                //Determine 'a'
                if (i == 0){
                    out.type = 'e';
                    out.s = "SOFTWARE ERROR: Implied 'ans' failed to evaluate! This error was detected in file" + this_file_name;
                    return false;
                }else{
                    a = tokens[i-1];
                    start_idx = i-1;
                }

                //Determine 'b'
                if (i+1 >= tokens.size()){
                    out.type = 'e';
                    out.s = "ERROR: Operator '>' requires second argument.";
                    return false;
                }else{
                    b = tokens[i+1];
                    end_idx = i+1;
                }

                //Perform operation
                if (a.type == 'b' && b.type =='b'){ //bool & bool
                    a.b = a.b > b.b;
                }else if(a.type == 'd' && b.type == 'd'){
                    a.b = (a.d > b.d);
                }else if(a.type == 'm' && b.type == 'm'){
                    a.km -= b.km;
                    a.b = true;
                    for (int r = 0; r < a.km.rows() ; r++){
                        for (int c = 0 ; c < a.km.cols() ; c++){
                            if (a.km(r, c) <= 0){
                                a.b = false;
                                break;
                            }
                        }
                        if (!a.b) break;
                    }
                }else if(a.type == 's' && b.type == 's'){
                    a.b = (a.s > b.s);
                }else{
                    out.type = 'e';
                    out.s = "ERROR: equals operator (==) can only operate on variables of the same type.";
                    return false;
                }

                a.type = 'b';

                inject_akt_into_aktvec(tokens, a, start_idx, end_idx);
                i = start_idx;
            }else if(tokens[i].s == "<"){
                //Determine 'a'
                if (i == 0){
                    out.type = 'e';
                    out.s = "SOFTWARE ERROR: Implied 'ans' failed to evaluate! This error was detected in file" + this_file_name;
                    return false;
                }else{
                    a = tokens[i-1];
                    start_idx = i-1;
                }

                //Determine 'b'
                if (i+1 >= tokens.size()){
                    out.type = 'e';
                    out.s = "ERROR: Operator '<' requires second argument.";
                    return false;
                }else{
                    b = tokens[i+1];
                    end_idx = i+1;
                }

                //Perform operation
                if (a.type == 'b' && b.type =='b'){ //bool & bool
                    a.b = a.b < b.b;
                }else if(a.type == 'd' && b.type == 'd'){
                    a.b = (a.d < b.d);
                }else if(a.type == 'm' && b.type == 'm'){
                    a.km -= b.km;
                    a.b = true;
                    for (int r = 0; r < a.km.rows() ; r++){
                        for (int c = 0 ; c < a.km.cols() ; c++){
                            if (a.km(r, c) >= 0){
                                a.b = false;
                                break;
                            }
                        }
                        if (!a.b) break;
                    }
                }else if(a.type == 's' && b.type == 's'){
                    a.b = (a.s < b.s);
                }else{
                    out.type = 'e';
                    out.s = "ERROR: equals operator (==) can only operate on variables of the same type.";
                    return false;
                }
                a.type = 'b';

                inject_akt_into_aktvec(tokens, a, start_idx, end_idx);
                i = start_idx;
            }
        }
    }

    if (tokens.size() == 1){
        out.type = tokens[0].type;
        out.s = tokens[0].s;
        out.d = tokens[0].d;
        out.b = tokens[0].b;
        out.km = tokens[0].km;
    }else{
        out.type = 'e';
        out.s = "ERROR: Expression did not evaluate to a single value.";
        if (tokens.size() == 0){
            out.s = "ERROR: Empty expression.";
        }
        return false;
    }

	return true;
}

/*
Ensures appropriate whitespace and parsing for the KInterp function 'interpret()'.

input - string to space and parse

returns the parsed vector of words
*/
vector<std::string> space_and_parse(std::string input){

	//Process input string (Ensure whitespace and parse)
	ensure_whitespace(input, "[](){};+/*=^%!");
	ensure_whitespace_full(input, "//");
	ensure_whitespace_full(input, "||");
	ensure_whitespace_full(input, "&&");

	vector<string> words = parse(input, " ");
	return words;

}

/*
 Ensures appropriate whitespace and parsing for the KInterp function 'interpret()'.

 input - string to space and parse

 returns the parsed vector of words
 */
vector<std::string> space_and_parse_protected(std::string input){

    //Process input string (Ensure whitespace and parse)
    vector<string> protected_items;
    protected_items.push_back("+=");
    protected_items.push_back("-=");
    protected_items.push_back("*=");
    protected_items.push_back("/=");
    protected_items.push_back("^=");
    protected_items.push_back("==");
    protected_items.push_back("!=");
    protected_items.push_back(">=");
    protected_items.push_back("<=");
    ensure_whitespace_protected(input, "[](){};+/*=^%!", protected_items);
    ensure_whitespace_full(input, "//");
    ensure_whitespace_full(input, "||");
    ensure_whitespace_full(input, "&&");
    ensure_whitespace_full(input, "+=");
    ensure_whitespace_full(input, "-=");
    ensure_whitespace_full(input, "*=");
    ensure_whitespace_full(input, "/=");
    ensure_whitespace_full(input, "^=");
    ensure_whitespace_full(input, "==");
    ensure_whitespace_full(input, ">=");
    ensure_whitespace_full(input, "<=");

    vector<string> words = parse(input, " ");
    return words;

}

/*
 Ensures appropriate whitespace and parsing for the KInterp function 'interpret()' and handles negatives specially.

 input - string to space and parse

 returns the parsed vector of words
 */
vector<std::string> space_and_parse_negatives(std::string input){

    //Process input string (Ensure whitespace and parse)
    vector<string> protected_items;
    protected_items.push_back("+=");
    protected_items.push_back("-=");
    protected_items.push_back("*=");
    protected_items.push_back("/=");
    protected_items.push_back("^=");
    protected_items.push_back("==");
    protected_items.push_back("!=");
    protected_items.push_back(">=");
    protected_items.push_back("<=");
    ensure_whitespace_protected(input, "[](){};+/*=^%!,", protected_items); //27.10.2017 - added ',' to ensure_whitespace list. Not thuroughly tested yet.
    ensure_whitespace_full(input, "//");
    ensure_whitespace_full(input, "||");
    ensure_whitespace_full(input, "&&");

    	//Whitespace for minus sign
        for (int i = 0 ; i < input.length() ; i++){
            if (input[i] == '-'){ //Add whitespace if character is '-' & next condition is met...
                char last_character = ' ';
                int char_index = i;
                while(last_character == ' ' && char_index > 0){
                    last_character = input[--char_index];
                }
                if ((i > 0 && is_negative_restricted_character(last_character)) || (i+1 < input.length() && input[i+1] == '=') ){ //Don't add whitespace if previous character exists and is a restricted character
                    continue;
                }else if(i == 0) continue;

                input = input.substr(0, i) + ' ' + input[i] + ' ' + input.substr(i+1);
                i++;
            }
        }

        //Ensure whitespace duplicates are removed
        for (int i = 0 ; i < input.length() ; i++){
            if (input[i] == ' ' && i > 0 && input[i-1] == ' '){
                input = input.substr(0, i) + input.substr(i+1);
            }
        }

    vector<string> words = parse(input, " ");
    return words;

}

bool is_negative_restricted_character(char c){
    return (c == 'e' || c == 'E' || c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%' || c == '=' || c == '(' || c == '[' || c == '{' || c == ';');
}

/*
Injects a string into a vector of strings at the location 'begin', and deletes all words up though 'end' (referenced before injecting 'inject'). Inject is then parsed.

words - string vector inwhich to inject 'inject'.
inject - a string to inject into 'words', and which will be parsed after being added to 'words'. 'inject' will replace all of the words from 'begin' to 'end' inclusive in 'words'.

Void return
*/
void inject_words_into_strvec(std::vector<std::string>& words, std::string inject, int begin, int end){

	//Clear items in 'words' that are to be erased
	for (int i = begin ; i <= end ; i++){
		words.erase(words.begin() + begin);
	}

	//parse 'inject' and add into 'words'
	vector<string> add = space_and_parse(inject);
	for (int i = 0 ; i < add.size() ; i++){
		words.insert(words.begin() + begin, add[add.size() -1 - i]);
	}
}

/*
Deletes the 'all_ktypes' in 'words' between 'begin' and 'end'. Inserts 'inject' where the values were removed.

words - vector to modify
inject - all_ktype to insert into 'words'
begin - index to begin removing items from 'words' and at which to locate 'inject'
end - index at which to stop eliminating items from 'words'

Void return
*/
void inject_akt_into_aktvec(std::vector<all_ktype>& words, all_ktype inject, int begin, int end){

    //Clear items in 'words' that are to be erased
    for (int i = begin ; i <= end ; i++){
        words.erase(words.begin() + begin);
    }

    words.insert(words.begin() + begin, inject);
}

string akt_tostring(all_ktype akt, int precision, int threshold){

    if (akt.type == 'd'){
        char notation = select_notation(akt.d, threshold);
        return hp_string(akt.d, precision, (notation == 's'));
    }else if(akt.type == 'o'){
        return akt.s;
    }else if(akt.type == 's'){
        return akt.s;
    }else if(akt.type == 'b'){
        return bool_to_str(akt.b);
    }else if(akt.type == 'm'){
        return akt.km.get_string();
    }else if(akt.type == 'e'){
        return akt.s;
    }else{
        return "UNDEFINED AKT TYPE";
    }
}

/*
notation_type - if:
    'f' - fixed notation
    's' - Scientific notation
    'x' - select notation based on threshold
*/
string akt_tostring(all_ktype akt, int precision, int threshold, char notation_type){

    if (akt.type == 'd'){
        char notation;
        switch(notation_type){ //Select notation
            case('f'):
                notation = 'f';
                break;
            case('s'):
                notation = 's';
                break;
            default:
                notation = select_notation(akt.d, threshold);
                break;
        }
        return hp_string(akt.d, precision, (notation == 's'));
    }else if(akt.type == 'o'){
        return akt.s;
    }else if(akt.type == 's'){
        return akt.s;
    }else if(akt.type == 'b'){
        return bool_to_str(akt.b);
    }else if(akt.type == 'm'){
        return akt.km.get_string();
    }else if(akt.type == 'e'){
        return akt.s;
    }else{
        return "UNDEFINED AKT TYPE";
    }
}

/*
 Converts an 'all_ktype' to a string for printing. The 'formal' setting removes type identifiers for a final product, whereas normal operation is better for debugging.

 akt - all_ktype to convert to string
 formal - activate 'formal' setting. Removes all but the value contained by the all_ktype (Otherwise specifies type).

 Returns a string representing the all_ktype.
*/
string akt_tostring(all_ktype akt, bool formal){

    if (!formal){
        if (akt.type == 'd'){
            return "DB:"+to_string(akt.d);
        }else if(akt.type == 'o'){
            return "OP:"+akt.s;
        }else if(akt.type == 's'){
            return "ST:"+akt.s;
        }else if(akt.type == 'b'){
            return "BL:"+bool_to_str(akt.b);
        }else if(akt.type == 'm'){
            return "KM:"+akt.km.get_string();
        }else if(akt.type == 'e'){
            return "ERROR:"+akt.s;
        }else{
            return "UNDEFINED AKT TYPE";
        }
    }else{
        if (akt.type == 'd'){
            return hp_string(akt.d);
        }else if(akt.type == 'o'){
            return akt.s;
        }else if(akt.type == 's'){
            return akt.s;
        }else if(akt.type == 'b'){
            return bool_to_str(akt.b);
        }else if(akt.type == 'm'){
            return akt.km.get_string();
        }else if(akt.type == 'e'){
            return akt.s;
        }else{
            return "UNDEFINED AKT TYPE";
        }
    }
}

//Delete 'print_error
bool run_interpret(std::string filename, KVar& vars, all_ktype& out, std::vector<func_id> interp_functions, bool persist, bool print_results, string indentation, vector<record_entry>& record, bool delete_comments, program_settings settings, string& in_header, string& out_header, vector<int>& fail_lines, std::vector<std::string>& fail_messages){

    //Open file
    ifstream file(filename);
    if (!file.is_open()){
        out.type = 'e';
        out.s = "ERROR: Failed to open file '" + filename + "'";
        return false;
    }

    bool running = true;
    string s;
    all_ktype last;
    last.type = 's';
    last.s = "No commands successfully evaluated in file '" + filename + "'";
    int line = 0;
    int num_failures = 0;
	bool retval = true;
    while(file.good()){
        getline(file, s);
        line++;

        if (delete_comments){
            remove_comments(s, "//");
        }

        interpret_with_keywords(s, vars, out, interp_functions, running, record, !print_results, settings, in_header, out_header);

        if (out.type == 'e'){
			fail_lines.push_back(line);
            if (!persist){
                return false;
            }else{
				fail_messages.push_back(out.s);
                // out = last; //This way a valid aktype is returned no matter what
				retval = false;
            }
        }else{
            last = out;
        }

    }

    return retval;
}

bool run_interpret(std::string filename, KVar& vars, all_ktype& out, std::vector<func_id> interp_functions, bool persist, bool print_results, string indentation, vector<record_entry>& record, bool delete_comments, program_settings settings, string& in_header, string& out_header){
	vector<int> dummy;
	vector<string> fm;
	return run_interpret(filename, vars, out, interp_functions, persist, print_results, indentation, record, delete_comments, settings, in_header, out_header, dummy, fm);
}
std::vector<std::vector<std::string> > form_sentences(std::vector<std::string> input){

//    for (int i = 0 ; i < input.size() ; i++){
//        cout << '\'' << input[i] << '\'' << endl;
//    }

    vector<vector<string> > sentences;

    int bracket_level = 0;
    int last_word_dump = 0;
    for (int i = 0 ; i < input.size() ; i++){
        if (input[i][0] == '['){
            bracket_level++;
        }else if(input[i][0] == ']'){
            bracket_level--;
        }else if(input[i][0] == ';' && bracket_level == 0){
            vector<string> temp_vector;
            for (int j = last_word_dump ; j < i ; j++){
                temp_vector.push_back(input[j]);
            }
            last_word_dump = i+1;
            sentences.push_back(temp_vector);
        }
    }

    if (last_word_dump < input.size()){
        vector<string> temp_vector;
        for (int j = last_word_dump ; j < input.size() ; j++){
            temp_vector.push_back(input[j]);
        }
        sentences.push_back(temp_vector);
    }

    return sentences;
}
