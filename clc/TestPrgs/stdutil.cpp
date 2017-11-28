#include <tgmath.h>
#include <fstream>
#include <iomanip>

#include "stdutil.hpp"

using namespace std;

void choose_scientific(double in, double threshold, double precision){
    
    if (in / threshold >= 1 || in < pow(10, -1*precision) ){
        cout.precision(precision);
        cout << std::scientific;
    }else{
        cout.precision(precision);
        cout << std::fixed;
    }
}

bool print_file(std::string filename, int tabs){
    
    ifstream file(filename);
    if (!file.is_open()){
        return false;
    }
    
    string s;
    while (file.good()){
        getline(file, s);
        
        cout << indent_line(tabs) << s << endl;
    }
    
    return true;
}

void print_vector(vector<vector<double> > v, int indent, bool spaces){

	//Select indentation type
	char indent_char = '\t';
	if (spaces){
		indent_char = ' ';
	}

	for (int r = 0 ; r < v.size() ; r++){
		
		//indent
		for (int i = 0; i < indent ; i++){
			cout << indent_char;
		}

		cout << "| ";
		for (int c = 0 ; c < v[r].size()-1 ; c++){
			cout << v[r][c] << ", ";
		}
		cout << v[r][v[r].size()-1] << " |" << endl;
	}

}

string indent_line(int indentation, bool use_spaces, bool indent_in_function){

	char ind = '\t';
	if (use_spaces) ind = ' ';

	string out = ""; 
	for (int i = 0 ; i < indentation ; i++){
		out = out + ind;
	}

	if (indent_in_function) cout << out;
	
	return out;
}


/*
Reports if a vector of strings contains a specific string. Reports the 1st index at which the target string is found.

vec - Vector of strings to search
target - String to search for in 'vec'

Returns an int representing the first index at which 'target' is present. -1 if not present in 'vec'.
*/
int strvec_contains(vector<string> vec, string target){

	for (int i = 0 ; i < vec.size() ; i++){
		if (vec[i] == target){
			return i;
		}
	}

	return -1;
}

/*
 Inserts tabs or spaces so all new lines are indented at least as far as specified by 'indentation'.
 
 in - string in which to insert indentation
 indentation - number of tabs (or spaces if specified by use_spaces) to insert after new lines
 use_spaces - specifies indentation with spaces rather than tabs
 
 Returns the modified string
 */
std::string indent_in_string(std::string in, int indentation, bool use_spaces){
    
    if (in[0] != '\n'){
        in = indent_line(indentation, use_spaces) + in;
    }
    
    for (int i = 0 ; i < in.length() ; i++){
        if (in[i] == '\n'){
            try{
                in = in.substr(0, i+1) + indent_line(indentation, use_spaces) + in.substr(i+1);
            }catch(...){}
            i += indentation;
        }
    }
    
    return in;
}
