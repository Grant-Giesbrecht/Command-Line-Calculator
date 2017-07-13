//#include <iostream>
//#include <stdio.h>
//#include <vector>
//#include <string>
//
//#include "string_manip.hpp"
//#include "stdutil.hpp"
//#include "kc_aux.hpp"
//#include "KMatrix.hpp"
//#include "KVar.hpp"
//
//using namespace std;
//
//int main(){
//
//	//Declare keywords
//	vector<string> keywords = fill_keywords();
//
//	//Declare variable vectors
//	KVar kv;
//	// vector<com_double> double_list;
//	// vector<com_string> string_list;
//	// vector<com_bool> bool_list;
//	// vector<com_matrix> matrix_list;
//	// vlist vars = {&double_list, &string_list, &bool_list, &matrix_list};
//
//	//Main loop
//	bool running = true;
//	string input;
//	vector<string> words;
//	int idx;
//	while(running){
//
//		//Prompt and recieve input
//		cout << "> ";
//		getline(cin, input);
//		ensure_whitespace(input, "[];+-*=^%");
//		ensure_whitespace_full(input, "//");
//		//Expand variables
//		words = parse(input, " ");
//
//		idx = strvec_contains(keywords, words[0]);
//		if (idx == 0){ //Exit CMD
//			running = false;
//			break;
//		}else if (idx == 2 || idx == 3){ //Clear CMD
//			try{
//				system("clear");
//				system("cls");
//			}catch(...){
//			}
//		}else if (idx == 4){ //Help CMD
//			print_file("kc_help.txt", 0);
//		}else if(idx == 5){ //Clear vars CMD
//			kv.clear();
//			cout << "\tAll variables cleared" << endl;
//		}else if(idx == 6){ //List vars CMD
//			kv.print(1);
//		}else if(idx == 7){ //Print record
//
//		}else if(idx == 8){ //Save record as program file
//
//		}else if(idx == 9){ //Execute program file
//
//		}else if (idx == 10){ //Matrix CMD
//			if (!add_matrix(words, kv)) cout << "\tERROR: Failed to add matrix" << endl;
//		}else if (idx == 11){ //Bool CMD
//			if (!add_bool(words, kv)) cout << "\tERROR: Failed to add bool" << endl;
//		}else if (idx == 12){ //String CMD
//			if (!add_string(words, kv)) cout << "\tERROR: Failed to add string" << endl;
//		}else if (idx == 13){ //Double CMD
//			if (!add_double(words, kv)) cout << "\tERROR: Failed to add double" << endl;
//		}else{ //DETERMINE IF DOUBLE OR EVALUATION
//			if (is_varname(words[0], kv)){ //Evaluate
//
//			}else if(valid_varname){ //Set as double
//
//			}
//		}
//	}
//
//	return 0;
//}
//
