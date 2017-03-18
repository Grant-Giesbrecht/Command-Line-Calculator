#include "KVar.hpp"

#define VEC_DOUBLE 'd'
#define VEC_MATRIX 'm'
#define VEC_STRING 's'
#define VEC_BOOL 'b'

#define IFPRINT if (KVar::print_allowed)

#define LATEST_VAR_VERSION 2.0

using namespace std;

/*-----------------------------------------------------*\
|-------------- INITIALIZATION FUNCTIONS ---------------|
\*-----------------------------------------------------*/

KVar::KVar(){
    KVar:print_allowed = false;
}

KVar::KVar(string ofile){
    KVar::print_allowed = false;
}

KVar::~KVar(){

}

//User Macros

void KVar::set(int prop, double val){

	switch(prop){
		case(KV_COMMENT_STYLE):
			KVar::comment_style = (int)val;
			break;
		// case():

		// 	break;
		// case():
		// 	break;
		default:
			IFPRINT cout << "ERROR: KVar::set() 'prop' not understood." << endl;
			break;
	}

}

void KVar::set(int prop, bool val){
	switch(prop){
		case(KV_DOUBLE_SPACE):
			KVar::double_space_comment = val;
			break;
		case(KV_SAVE_COMMENTS):
			KVar::save_comments = val;
			break;
		case(KV_READ_COMMENTS):
			KVar::read_comments = val;
			break;
        case (KV_ALLOW_PRINT):
            KVar::print_allowed = false;
            break;
		default:
			IFPRINT cout << "ERROR: KVar::set() 'prop' not understood." << endl;
			break;
	}
}

void KVar::set(int prop, string val){

}

/*-----------------------------------------------------*\
|------------------ ACCESS FUNCTIONS -------------------|
\*-----------------------------------------------------*/

bool KVar::contains(std::string var_id){
	return get_var_index(var_id, NULL, NULL);
}

void KVar::clear(){
	KVar::IDs.clear();
	KVar::comments.clear();
	KVar::double_vars.clear();
	// KVar::kmatrix_vars.clear();
	KVar::matrix_vars.clear();
	KVar::string_vars.clear();
	KVar::bool_vars.clear();
}

vector<string> KVar::get_variable_IDs(){
	vector<string> out;
	for(int i = 0 ; i < KVar::IDs.size() ; i++){
		out.push_back(KVar::IDs[i].ID);
	}
	return out;
}

int KVar::num_variables(){
	return KVar::IDs.size();
}

char KVar::get_var_type(string varname){

	char type;
	int x; //Just a filler variable for 'get_var_index()'.

	get_var_index(varname, &x, &type);

	return type;
}

/*-----------------------------------------------------*\
|--------------- MODIFICATION FUNCTIONS ----------------|
\*-----------------------------------------------------*/

bool KVar::add_double(std::string var_id, double val, string comment){

	identifier ni;
	ni.ID = var_id;
	ni.type = VEC_DOUBLE;

	KVar::double_vars.push_back(val);
	KVar::IDs.push_back(ni);
	KVar::comments.push_back(comment);

	return true;
}

bool KVar::get_double(std::string var_id, double& out){

	int idx;
	char type;
	if (!KVar::get_var_index(var_id, &idx, &type) || type != VEC_DOUBLE){
		return false;
	}

	out = double_vars[count_type(idx, type)-1];
	return true;
}

bool KVar::set_double(std::string var_id, double in){
    
    int idx;
    char type;
    if (!KVar::get_var_index(var_id, &idx, &type) || type != VEC_DOUBLE){
        return false;
    }
    
    double_vars[count_type(idx, type)-1] = in;
    return true;
}

bool KVar::add_matrix(std::string var_id, KMatrix& val, string comment){

	identifier ni;
	ni.ID = var_id;
	ni.type = VEC_MATRIX;

	KVar::matrix_vars.push_back(val.to_Eigen());
	KVar::IDs.push_back(ni);
	KVar::comments.push_back(comment);

	return true;
}

bool KVar::get_matrix(std::string var_id, KMatrix& out){

	int idx;
	char type;
	if (!KVar::get_var_index(var_id, &idx, &type) || type != VEC_MATRIX){
		return false;
	}

	out = matrix_vars[count_type(idx, type)-1];
	return true;
}

bool KVar::set_matrix(std::string var_id, KMatrix in){
    
    int idx;
    char type;
    if (!KVar::get_var_index(var_id, &idx, &type) || type != VEC_MATRIX){
        return false;
    }
    
    matrix_vars[count_type(idx, type)-1] = in.to_Eigen();
    return true;
}

bool KVar::add_matrix(std::string var_id, Eigen::MatrixXd& val, string comment){

	identifier ni;
	ni.ID = var_id;
	ni.type = VEC_MATRIX;

	KVar::matrix_vars.push_back(val);
	KVar::IDs.push_back(ni);
	KVar::comments.push_back(comment);

	return true;
}

bool KVar::get_matrix(std::string var_id, Eigen::MatrixXd& out){

	int idx;
	char type;
	if (!KVar::get_var_index(var_id, &idx, &type) || type != VEC_MATRIX){
		return false;
	}

	out = matrix_vars[count_type(idx, type)-1];
	return true;
}

bool KVar::set_matrix(std::string var_id, Eigen::MatrixXd in){
    
    int idx;
    char type;
    if (!KVar::get_var_index(var_id, &idx, &type) || type != VEC_MATRIX){
        return false;
    }
    
    matrix_vars[count_type(idx, type)-1] = in;
    return true;
}

bool KVar::add_string(std::string var_id, string val, string comment){

	identifier ni;
	ni.ID = var_id;
	ni.type = VEC_STRING;

	KVar::string_vars.push_back(val);
	KVar::IDs.push_back(ni);
	KVar::comments.push_back(comment);

	return true;
}

bool KVar::get_string(std::string var_id, string& out){

	int idx;
	char type;
	if (!KVar::get_var_index(var_id, &idx, &type) || type != VEC_STRING){
		return false;
	}

	out = string_vars[count_type(idx, type)-1];
	return true;
}

bool KVar::set_string(std::string var_id, string in){
    
    int idx;
    char type;
    if (!KVar::get_var_index(var_id, &idx, &type) || type != VEC_STRING){
        return false;
    }
    
    string_vars[count_type(idx, type)-1] = in;
    return true;
}

bool KVar::add_bool(std::string var_id, bool val, string comment){

	identifier ni;
	ni.ID = var_id;
	ni.type = VEC_BOOL;

	KVar::bool_vars.push_back(val);
	KVar::IDs.push_back(ni);
	KVar::comments.push_back(comment);

	return true;
}

bool KVar::get_bool(std::string var_id, bool& out){

	int idx;
	char type;
	if (!KVar::get_var_index(var_id, &idx, &type) || type != VEC_BOOL){
		return false;
	}

	out = bool_vars[count_type(idx, type)-1];
	return true;
}

bool KVar::set_bool(std::string var_id, bool in){
    
    int idx;
    char type;
    if (!KVar::get_var_index(var_id, &idx, &type) || type != VEC_BOOL){
        return false;
    }
    
    bool_vars[count_type(idx, type)-1] = in;
    return true;
}

string KVar::get_comment(std::string var_id, bool& success){

	int idx;
	char type;
	if (!KVar::get_var_index(var_id, &idx, &type)){
		success = false;
		IFPRINT cout << "Failed to find specified variable" << endl;
		return "";
	}

	success = true;
	return KVar::comments[idx];
}

string KVar::get_comment(std::string var_id){
	bool temp;
	return get_comment(var_id, temp);
}

bool KVar::delete_var(std::string var_id){
    
    /*
     std::vector<identifier> IDs;
     std::vector<std::string> comments; //Alligns with IDs in vector
     std::vector<double> double_vars;
     std::vector<Eigen::MatrixXd> matrix_vars;
     std::vector<std::string> string_vars;
     std::vector<bool> bool_vars;
     */
    
    int idx;
    char type;
    if (!get_var_index(var_id, &idx, &type)){
        IFPRINT cout << "Failed to find specified variable" << endl;
        return false;
    }
    
    int l_idx;
    if (!get_var_lindex(var_id, &l_idx, &type)){
        IFPRINT cout << "Failed to find specified variable" << endl;
        return false;
    }
    
    switch (type) {
        case 'd':
            double_vars.erase(double_vars.begin() + l_idx);
            break;
        case 'm':
            matrix_vars.erase(matrix_vars.begin() + l_idx);
            break;
        case 'b':
            bool_vars.erase(bool_vars.begin() + l_idx);
            break;
        case 's':
            string_vars.erase(string_vars.begin() + l_idx);
            break;
        default:
            return false;
            break;
    }
    comments.erase(comments.begin() + idx);
    IDs.erase(IDs.begin() + idx);
    
    
    return true;
}

/*-----------------------------------------------------*\
|----------------- READ/WRITE FUNCTIONS ----------------|
\*-----------------------------------------------------*/

bool KVar::load_file(string filename, bool print_out){

	//Determine the version used
	ifstream filev(filename);
    if (!filev.is_open()){
        return false;
    }
	string s;
	float version = 1.0; //If no VERSION macro, must be version 1.0
	bool scanning_version = true;
	while (filev.good() && scanning_version){
		getline(filev, s);
		remove_comments(s, "//"); //Must be exact match

		vector<string> tok = parse(s, " ");
		if (tok.size() <= 0){
			continue;
		}

		if (tok[0] == "#VERSION"){
			try{
				if (tok.size() < 2) continue;
				version = stof(tok[1]);
				if (version > LATEST_VAR_VERSION){
					IFPRINT cout << "WARNING: Using unknown file version: " << version << ". Interpreting as version " << LATEST_VAR_VERSION << " ." << endl;
				}
			}catch(...){
				IFPRINT cout << "WARNING: Using unknown file version: " << version << ". Interpreting as version " << LATEST_VAR_VERSION << " ." << endl;
				version = 2.0;
			}
		}

	}
	filev.close();

	//Open file
    ifstream file(filename);
    if (!file.is_open()){
        return false;
    }

	//Read file
	vector< vector<double> > matrix_buffer;
	string comment_buffer;
	bool in_header = true;
    while (file.good()){
        getline(file, s);

        //Remove comments from line (comments follow # sign)
        remove_comments(s, "//"); //Must be complete match

		//Ensure that semicolons aren't hidden in the middle of tokens
        ensure_whitespace(s, ";#");

        //Separate value and variable
        vector<string> parts = parse(s, " ");

        //Verify line is not blank
        if (parts.size() == 0){
            continue;
        }

		if (parts[2][0] == '='){

			in_header = false; //Mark that comments no longer go towards the header

			switch(parts[0][0]){
				case('d'):
					comment_buffer = "";
					if (parts.size() < 4) continue; //Prevent indexing errors
					cout << "1" << endl;
					if (parts.size() > 5){ // Read special comments (if there)
						if (parts[5][0] != '#'){
							cout << "@" << endl;
							IFPRINT cout << "ERROR: Unexpected character following double declaration!" << endl;
							continue;
						}
						comment_buffer = comment_buffer + cat_tokens(parts, 6, " ");
					}
					try{
						double temp = stod(parts[3]);
						cout << temp << endl;
						add_double(parts[1], temp, comment_buffer);
					}catch(...){
						cout << "!" << endl;
						//Do nothing
					}
					break;
				case('m'):{
						comment_buffer = "";
						if (parts.size() < 4) continue; //Prevent indexing errors
						string name = parts[1];
						matrix_buffer.clear();
						vector<double> temp;
						bool in_matrix = true;

						//Read in a line of the matrix (goes into vector<double> 'temp', which goes into vector<vector<doubel> > 'matrix_buffer')
						for (int i = 3 ; i < parts.size() ; i++){

							//Add special comments to 'comment_buffer'
							if (parts[i][0] == '#'){
								comment_buffer = comment_buffer + cat_tokens(parts, i+1, " ");
								break;
							}

							if (parts[i][parts[i].length()-1] == ';'){ //If last character of last word is a semicolon, matrix is complete.
								in_matrix = false;
							}

							if (!in_matrix) continue;

							double num;
							try{
								num = stod(parts[i]);
							}catch(...){
								num = -1;
								IFPRINT cout << "SYNTAX ERROR: Failed to interpret matrix element: " << parts[i] << endl;
							}
							temp.push_back(num);
						}

						matrix_buffer.push_back(temp); //Add temp to matrix_buffer

						//Loop until matrix read is complete
						while (in_matrix){

							//read and format new line
							getline(file, s);
							remove_comments(s, "//");
							ensure_whitespace(s, ";#"); //Ensure that semicolons aren't hidden in the middle of tokens
							parts = parse(s, " ");

							//Populate vector<double> 'temp', which is pushed back into vector<vector<double> > 'matrix_buffer'
							temp.clear();
							for (int i = 0 ; i < parts.size() ; i++){

								//Add special comments to 'comment_buffer'
								if (parts[i][0] == '#'){
									if (comment_buffer.size() > 0) comment_buffer = comment_buffer + ' ';
									comment_buffer = comment_buffer + cat_tokens(parts, i+1, " ");
									break;
								}

								if (parts[i][parts[i].length()-1] == ';'){
									in_matrix = false;
								}

								if (!in_matrix) continue;

								double num;
								try{
									num = stod(parts[i]);
								}catch(...){
									num = -1;
									if (print_out) cout << "SYNTAX ERROR: Failed to interpret matrix element: " << parts[i] << endl;
								}
								temp.push_back(num);
							}
							matrix_buffer.push_back(temp);
						}

						//Matrix buffer has been populated, so allow KMatrix to interpret it and actually create the matrix
						KMatrix km(matrix_buffer);
						add_matrix(name, km, comment_buffer); //Add KMatrix to object
					}
					break;
				case('s'):{
						comment_buffer = "";
						bool began = false;
						int end = -1;
						string str = "";
						for (int i = 3 ; i < parts.size() ; i++){

							//Add special comments to 'comment_buffer'
							if (parts[i][0] == '#' && end != -1){
								comment_buffer = comment_buffer + cat_tokens(parts, i+1, " ");
								break;
							}

							remove_from_end(parts[i], "; ");
							if (parts[i][0] == '"' && i == 3){
								began = true;
								str = parts[i].substr(1);
							}else if (parts[i][parts[i].length()-1] == '"'){
								end = i;
								parts[i].pop_back();
								str  = str + " " + parts[i];
							}else if(began){
								str  = str + " " + parts[i];
							}
						}
						if (!began || end == -1 ){
							if (print_out) cout << "SYNTAX ERROR: Failed to interpret string: '" << cat_tokens(parts, 3, " ") << "'." << endl;
							continue;
						}
						add_string(parts[1], str, comment_buffer);
					}
					break;
				case('b'):
					comment_buffer = "";
					if (parts.size() < 4) continue; //Prevent indexing error
					if (parts.size() > 5){ // Read special comments (if there)
						if (parts[5][0] != '#'){
							IFPRINT cout << "ERROR: Unexpected character following double declaration!" << endl;
							continue;
						}
						comment_buffer = comment_buffer + cat_tokens(parts, 6, " ");
					}
					remove_from_end(parts[3], "; ");
					if (to_uppercase(parts[3]) == "TRUE"){
						add_bool(parts[1], true, comment_buffer);
					}else if(to_uppercase(parts[3]) == "FALSE"){
						add_bool(parts[1], false, comment_buffer);
					}else{
						if (print_out) cout << "SYNTAX ERROR: Failed to interpret bool: '" << parts[3] << "'." << endl;
					}
					break;
				default:
					break;
			}
		}

	}

	return true;
}

bool KVar::write_file(string filename){
	return true;
}

/*-----------------------------------------------------*\
|----------------- STANDARD FUNCTIONS ------------------|
\*-----------------------------------------------------*/

void KVar::print(int indent, bool use_spaces){

	//Doubles
	int counter = 0;
	for (int i = 0 ; i < IDs.size() ; i++){
		if (IDs[i].type == VEC_DOUBLE){
			if (counter == 0) cout << indent_line(indent, use_spaces, false) << "DOUBLES:" << endl; //Print header if first double
			cout << indent_line(indent, use_spaces, false) << "\t" << IDs[i].ID << ": " << double_vars[counter] << endl; //Print name and value
			counter++; //Increment counter
		}
	}

	//Matricies
	counter = 0;
	for (int i = 0 ; i < IDs.size() ; i++){
		if (IDs[i].type == VEC_MATRIX){
			if (counter == 0) cout << indent_line(indent, use_spaces, false) << "MATRICIES:" << endl; //Print header if first double
			cout << indent_line(indent, use_spaces, false) << "\t" << IDs[i].ID << ": " << endl; //Print name and value
			KMatrix km(matrix_vars[counter]);
			int mindent = indent;
			if (!use_spaces) mindent *= 8;
			mindent += 12;
			km.print(mindent, true);
			counter++; //Increment counter
		}
	}

	//Strings
	counter = 0;
	for (int i = 0 ; i < IDs.size() ; i++){
		if (IDs[i].type == VEC_STRING){
			if (counter == 0) cout << indent_line(indent, use_spaces, false) << "STRINGS:" << endl; //Print header if first double
			cout << indent_line(indent, use_spaces, false) << "\t" << IDs[i].ID << ": " << string_vars[counter] << endl; //Print name and value
			counter++; //Increment counter
		}
	}

	//Bools
	counter = 0;
	for (int i = 0 ; i < IDs.size() ; i++){
		if (IDs[i].type == VEC_BOOL){
			if (counter == 0) cout << indent_line(indent, use_spaces, false) << "BOOLEANS:" << endl; //Print header if first double
			cout << indent_line(indent, use_spaces, false) << "\t" << IDs[i].ID << ": " << bool_to_str(bool_vars[counter]) << endl; //Print name and value
			counter++; //Increment counter
		}
	}
}

/*-----------------------------------------------------*\
|------------------ PRIVATE FUNCTIONS ------------------|
\*-----------------------------------------------------*/

/*
Gives location information for variable represented by 'var_id' ( Combination of KVar::() and KVar::count_type() ).

var_id - variable ID of the variable of interest
index - the index of the target variable, indexed with the variable's vector's specific index.
type - relays the datatype of the variable of interest

Returns false if variable is not found - 'index' is set to -1 and 'type' to '\0'
*/
bool KVar::get_var_lindex(string var_id, int* lindex, char* type){

	if (!KVar::get_var_index(var_id, lindex, type)){
		return false;
	}

	*lindex = KVar::count_type(*lindex, *type)-1;
    
	return true;
}

/*
Gives location information for variable represented by 'var_id'

var_id - variable ID of the variable of interest
index - the index FOR THE VECTOR KVar::IDs of the target variable (must use KVar::count_type() to get specific index or KVar::get_var_lindex())
type - relays the datatype of the variable of interest

Returns false if variable is not found - 'index' is set to -1 and 'type' to '\0'
*/
bool KVar::get_var_index(string var_id, int* index, char* type){

	*index = -1;
	*type = '\0';

	for (int i = 0 ; i < IDs.size() ; i++){
		if (IDs[i].ID == var_id){
			*index = i;
			*type = IDs[i].type;
			return true;
		}
	}

	return false;
}

/*
Reports number of variables of a specific type in master list up through and including the index 'index'.

index - index at which to stop counting (inclusive). If -1, will count through end of vector
type - type of variable to count

Returns number of variables of the specified type through the specified region.
*/
int KVar::count_type(int index, char type){

	int max;
	if (index == -1){
		max = KVar::IDs.size();
	}else if(index >= 0){
		max =  index+1;
	}else{
		return -1;
	}

	int count = 0;
	for (int i = 0 ; i < max ; i++){
		if (KVar::IDs[i].type == type){
			count++;
		}
	}

	return count;
}




		/*	__      __  __  _____	_______
			\ \    / / /  \ \____ \  || ||
			 \ \  / / / /\ \  __/_/  || ||
			  \ \/ / / / _\ \ \ \    || ||
			   \__/ /_/    \_\ \_\  _||_||_
		*/
