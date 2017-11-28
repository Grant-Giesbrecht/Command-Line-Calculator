#include "KVar.hpp"
//#include "KInterpAux.hpp"
//#include "CLIK.hpp"

#define VEC_DOUBLE 'd'
#define VEC_MATRIX 'm'
#define VEC_STRING 's'
#define VEC_BOOL 'b'

#define DOUBLE_MARK "d"
#define MATRIX_MARK "m"
#define STRING_MARK "s"
#define BOOL_MARK "b"

#define IFPRINT if (KVar::print_allowed)

#define LATEST_VAR_VERSION 1.0

using namespace std;

/*-----------------------------------------------------*\
|-------------- INITIALIZATION FUNCTIONS ---------------|
\*-----------------------------------------------------*/

KVar::KVar(){
    KVar:print_allowed = false;
    print_comments = true;
    KVar::read_comments = true;
    KVar::save_comments = true;
    KVar::overwrite_variables_on_load = false;
    KVar::print_precision = 8;
    KVar::print_threshold = 1e5;
}

KVar::KVar(string ofile){
    KVar::print_allowed = false;
    print_comments = true;
    KVar::read_comments = true;
    KVar::save_comments = true;
    KVar::overwrite_variables_on_load = false;
    KVar::print_precision = 8;
    KVar::print_threshold = 1e5;
}

KVar::~KVar(){

}

//User Macros

void KVar::set(int prop, double val){

	switch(prop){
		case(KV_COMMENT_STYLE):
			KVar::comment_style = (int)val;
			break;
        case(KV_PRINT_PRECISION):
            KVar::print_precision = val;
            break;
        case(KV_PRINT_THRESHOLD):
            KVar::print_threshold = val;
            break;
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
        case KV_PRINT_COMMENT:
            KVar::print_comments = val;
            break;
        case KV_OVERWRITE_ON_LOAD:
            KVar::overwrite_variables_on_load =  val;
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
    int unused_int;
	return get_var_index(var_id, &unused_int);
}

void KVar::clear(){
    KVar::variables.clear();
    header = "";
//	KVar::IDs.clear();
//	KVar::comments.clear();
//	KVar::double_vars.clear();
//	// KVar::kmatrix_vars.clear();
//	KVar::matrix_vars.clear();
//	KVar::string_vars.clear();
//	KVar::bool_vars.clear();
}

vector<string> KVar::get_variable_IDs(){
	vector<string> out;
	for(int i = 0 ; i < KVar::variables.size() ; i++){
		out.push_back(KVar::variables[i].ID);
	}
	return out;
}

int KVar::num_variables(){
	return (int)KVar::variables.size();
}

char KVar::get_var_type(string varname){

	int x;
	get_var_index(varname, &x);
    
    return variables[x].type;
}

/*-----------------------------------------------------*\
|--------------- MODIFICATION FUNCTIONS ----------------|
\*-----------------------------------------------------*/

bool KVar::add_variable(string var_id, double val, string comment){
    
    aktc temp;
    temp.ID = var_id;
    temp.type = 'd';
    temp.d = val;
    temp.comment = comment;
    variables.push_back(temp);
    
    return true;
}

bool KVar::add_variable(string var_id, bool val, string comment){
    
    aktc temp;
    temp.ID = var_id;
    temp.type = 'b';
    temp.b = val;
    temp.comment = comment;
    variables.push_back(temp);
    
    return true;
}

bool KVar::add_variable(string var_id, KMatrix val, string comment){
    
    aktc temp;
    temp.ID = var_id;
    temp.type = 'm';
    temp.km = val;
    temp.comment = comment;
    variables.push_back(temp);
    
    return true;
}

bool KVar::add_variable(string var_id, Eigen::MatrixXd& val, string comment){
    
    aktc temp;
    temp.ID = var_id;
    temp.type = 'm';
    temp.km = KMatrix(val);
    temp.comment = comment;
    variables.push_back(temp);
    
    return true;
}

bool KVar::add_variable(string var_id, string val, string comment){
    
    aktc temp;
    temp.ID = var_id;
    temp.type = 's';
    temp.s = val;
    temp.comment = comment;
    variables.push_back(temp);
    
    return true;
}

bool KVar::set_variable(string var_id, double val){
    
    int x;
    if (!get_var_index(var_id, &x)){
        return false;
    }
    
    variables[x].d = val;
    
    return true;
}

bool KVar::set_variable(string var_id, bool val){
    
    int x;
    if (!get_var_index(var_id, &x)){
        return false;
    }
    
    variables[x].b = val;
    
    return true;
}

bool KVar::set_variable(string var_id, string val){
    
    int x;
    if (!get_var_index(var_id, &x)){
        return false;
    }
    
    variables[x].s = val;
    
    return true;
}

bool KVar::set_variable(string var_id, KMatrix val){
    
    int x;
    if (!get_var_index(var_id, &x)){
        return false;
    }
    
    variables[x].km = val;
    
    return true;
}

bool KVar::set_variable(string var_id, Eigen::MatrixXd& val){
    
    int x;
    if (!get_var_index(var_id, &x)){
        return false;
    }
    
    variables[x].km = KMatrix(val);
    
    return true;
}

bool KVar::get_variable(string var_id, double& val){
    
    int x;
    if (!get_var_index(var_id, &x)){
        return false;
    }
    
    val = variables[x].d;
    
    return true;
}

bool KVar::get_variable(string var_id, string& val){
    
    int x;
    if (!get_var_index(var_id, &x)){
        return false;
    }
    
    val = variables[x].s;
    
    return true;
}

bool KVar::get_variable(string var_id, bool& val){
    
    int x;
    if (!get_var_index(var_id, &x)){
        return false;
    }
    
    val = variables[x].b;
    
    return true;
}

bool KVar::get_variable(string var_id, KMatrix& val){
    
    int x;
    if (!get_var_index(var_id, &x)){
        return false;
    }
    
    val = variables[x].km;
    
    return true;
}

bool KVar::get_variable(string var_id, Eigen::MatrixXd& val){
    
    int x;
    if (!get_var_index(var_id, &x)){
        return false;
    }
    
    val = variables[x].km.to_Eigen();
    
    return true;
}

bool KVar::add_double(std::string var_id, double val, string comment){

    return add_variable(var_id, val, comment);
}

bool KVar::get_double(std::string var_id, double& out){

    return get_variable(var_id, out);
}

bool KVar::set_double(std::string var_id, double in){
    
    return set_variable(var_id, in);
    
}

bool KVar::add_matrix(std::string var_id, KMatrix& val, string comment){

	return add_variable(var_id, val, comment);
}

bool KVar::get_matrix(std::string var_id, KMatrix& out){

	return get_variable(var_id, out);
    
}

bool KVar::set_matrix(std::string var_id, KMatrix in){
    
    return set_variable(var_id, in);
    
}

bool KVar::add_matrix(std::string var_id, Eigen::MatrixXd& val, string comment){

	return add_variable(var_id, val, comment);
}

bool KVar::get_matrix(std::string var_id, Eigen::MatrixXd& out){

	return get_variable(var_id, out);
    
}

bool KVar::set_matrix(std::string var_id, Eigen::MatrixXd in){
    
    return set_variable(var_id, in);
    
}

bool KVar::add_string(std::string var_id, string val, string comment){

	return add_variable(var_id, val, comment);
    
}

bool KVar::get_string(std::string var_id, string& out){

	return get_variable(var_id, out);
    
}

bool KVar::set_string(std::string var_id, string in){
    
    return set_variable(var_id, in);
    
}

bool KVar::add_bool(std::string var_id, bool val, string comment){

	return add_variable(var_id, val, comment);
    
}

bool KVar::get_bool(std::string var_id, bool& out){

	return get_variable(var_id, out);
    
}

bool KVar::set_bool(std::string var_id, bool in){
    
    return set_variable(var_id, in);
    
}

string KVar::get_comment(std::string var_id, bool& success){

    int x;
    success = get_var_index(var_id, &x);
    
    return variables[x].comment;
}

string KVar::get_comment(std::string var_id){
	
    bool temp;
	return get_comment(var_id, temp);
}

bool KVar::set_comment(std::string var_id, string nc){
    
    int x;
    if (!get_var_index(var_id, &x)){
        return false;
    }
    
    variables[x].comment = nc;
    
    return true;
}

bool KVar::delete_var(std::string var_id){
    
    int x;
    if (!get_var_index(var_id, &x)){
        IFPRINT cout << "Failed to find specified variable" << endl;
        return false;
    }
    
    variables.erase(variables.begin() + x);
    
    return true;
}

string KVar::get_header(){
    return KVar::header;
}

void KVar::set_header(std::string hdr){
    KVar::header = hdr;
}

/*-----------------------------------------------------*\
|----------------- READ/WRITE FUNCTIONS ----------------|
\*-----------------------------------------------------*/

bool KVar::load_file_old(string filename, bool print_out){

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
				version = stod(tok[1]);
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
						double temp = strtod(parts[3]);
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
								num = strtod(parts[i]);
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
									num = strtod(parts[i]);
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

bool KVar::load_KV1(std::string filename){
    long temp;
    return load_KV1(filename, temp);
}

//bool KVar::load_KV2(std::string filename){
//    long temp;
//    return load_KV2(filename, temp);
//}

//bool KVar::load_KV3(std::string filename){
//    long temp;
//    return load_KV3(filename, temp);
//}

bool load_file(string filename, long* fail_line){
 
    return true;
}

/*
 Fail_line = -1 -> File does not exist
 */
bool KVar::load_KV1(string filename, long& fail_line){
    
//    long backup_fail_line;
//    if (fail_line == NULL){
//        fail_line = &backup_fail_line;
//    }
    
    ifstream file(filename);
    if (!file.is_open()){
        fail_line = (long)-1;
        return false;
    }
    
    string line;
    vector<string> words;
    bool in_header = false;
    bool header_defined = false;
    header = "";
    bool first_line = true;
    fail_line = 0;
    while (file.good()){
        
        //Prepare next line of input
        getline(file, line);
        (fail_line)++;
        if (line.length() == 0){ //Skip blank lines
            continue;
        }
//        words = parse(line, " "); //Parse
        
        //Comments are removed - now semicolons are separated
        ensure_whitespace(line, "[;]\"");
        ensure_whitespace_full(line, "//");
        words = parse(line, " ");
        
        if (words.size() < 1 || words[0] == "//"){ //Skip comments
            continue;
        }
        
        //Determine file version
        double version;
        if (first_line){
            first_line = false;
            if (words.size() != 2){
                return false;
            }
            if (words[0] != "#VERSION"){
                return false;
            }
            if (!str_to_double(words[1], version)){
                return false;
            }
            continue;
        }
        
        if(words[0] == "#HEADER"){
            if (header_defined){
                return false;
            }
            if (in_header){
                header_defined = true;
            }
            in_header = !in_header;
        }else if(in_header){
            if (header.length() > 0){
                header = header + '\n';
            }
            header = header + cat_tokens(words, 0, " ");
        }else if (words[0] == DOUBLE_MARK){
            
            string var_name;
            double val;
            string comment;
            
            //Ensure minimum size is met TYPE NAME VALUE ;
            if (words.size() <  4){
                return false;
            }
            
            //Save variable name
            var_name = words[1];
            if (KVar::contains(var_name)){ //Avoid repeat variables...
                if (overwrite_variables_on_load){
                    KVar::delete_var(var_name); //Delete repeat if requested
                }else continue; //Otherwise don't load new variable
            }
            
            //Save value
            if (!str_to_double(words[2], val)){
                return false;
            }
            
            //Ensure semicolon is present
            if (words[3] != ";"){
                return false;
            }
            
            //Read comments
            if (words.size() > 4){
                if (words[4] != "//"){ //Ensure comment sign is present
                    return false;
                }
                if (read_comments){
                    comment = cat_tokens(words, 6, " "); //Save comment
                }
            }
            
            
            if (!add_variable(var_name, val, comment)){
                return false;
            }
        }else if(words[0] == MATRIX_MARK){
            
            string var_name;
            KMatrix val;
            string comment;
            
            //Ensure minimum size is met - TYPE NAME [ VALUE_1 ] ;
            if (words.size() <  6){
                return false;
            }
            
            //Save variable name
            var_name = words[1];
            if (KVar::contains(var_name)){ //Avoid repeat variables...
                if (overwrite_variables_on_load){
                    KVar::delete_var(var_name); //Delete repeat if requested
                }else continue; //Otherwise don't load new variable
            }
            
            //Save value
            string km_str;
            int lw;
            if (!next_phrase(words, &km_str, '[', ']', 2, &lw)){
                return false;
            }
            val = KMatrix(km_str);
            
            //Ensure semicolon is present
            if (words.size() <= lw+1 || words[lw+1] != ";"){
                return false;
            }
            
            //Read comments
            if (words.size() > lw+2){
                if (words[lw+2] != "//"){ //Ensure comment sign is present
                    return false;
                }
                if (read_comments){
                    comment = cat_tokens(words, lw+3, " "); //Save comment
                }
            }
            
            
            if (!add_variable(var_name, val, comment)){
                return false;
            }
            
        }else if(words[0] == STRING_MARK){
            
            string var_name;
            string val;
            string comment;
            
            //Ensure minimum size is met - TYPE NAME " VALUE_1 " ;
            if (words.size() <  6){
                return false;
            }
            
            //Save variable name
            var_name = words[1];
            if (KVar::contains(var_name)){ //Avoid repeat variables...
                if (overwrite_variables_on_load){
                    KVar::delete_var(var_name); //Delete repeat if requested
                }else continue; //Otherwise don't load new variable
            }
            
            //Save value
            int lw;
            if (!next_string(words, &val, 2, &lw)){
                return false;
            }
            
            //Ensure semicolon is present
            if (words.size() <= lw+1 || words[lw+1] != ";"){
                return false;
            }
            
            //Read comments
            if (words.size() > lw+2){
                if (words[lw+2] != "//"){ //Ensure comment sign is present
                    return false;
                }
                if (read_comments){
                    comment = cat_tokens(words, lw+3, " "); //Save comment
                }
            }
            
            
            if (!add_variable(var_name, val, comment)){
                return false;
            }
            
        }else if(words[0] == BOOL_MARK){
            
            string var_name;
            bool val;
            string comment;
            
            //Ensure minimum size is met TYPE NAME VALUE ;
            if (words.size() <  4){
                return false;
            }
            
            //Save variable name
            var_name = words[1];
            if (KVar::contains(var_name)){ //Avoid repeat variables...
                if (overwrite_variables_on_load){
                    KVar::delete_var(var_name); //Delete repeat if requested
                }else continue; //Otherwise don't load new variable
            }
            
            //Save value
            if (!str_to_bool(words[2], val)){
                return false;
            }
            
            //Ensure semicolon is present
            if (words[3] != ";"){
                return false;
            }
            
            //Read comments
            if (words.size() > 4){
                if (words[5] != "//"){ //Ensure comment sign is present
                    return false;
                }
                if (read_comments){
                    comment = cat_tokens(words, 6, " "); //Save comment
                }
            }
            
            
            if (!add_variable(var_name, val, comment)){
                return false;
            }
        }else{
            return false;
        }
    }
    
    
    return true;
}


/*
 Writes all variables to a KV1 file by the name of 'filename'
 
 filename - name of file to write (if it already exists it will be replaced)
 
 Returns true if the write was successful
 */
bool KVar::write_KV1(string filename){
	
    ofstream file;
    file.open(filename, std::ofstream::out | std::ofstream::trunc);
    if (!file.is_open()){
        return false;
    }
    
    file << "#VERSION 1.0" << endl << endl;
    
    if (header.length() > 0){
        file << "#HEADER" << endl;
        file << header << endl;
        file << "#HEADER" << endl << endl;
    }
    
    for (int i = 0 ; i < variables.size() ; i++){
        switch (variables[i].type) {
            case 'd':
                file << DOUBLE_MARK << " " << variables[i].ID << " " << hp_string(variables[i].d) << ";";
                if (variables[i].comment.length() > 0 && KVar::save_comments){
                    file << " //" << variables[i].comment;
                }
                file << endl;
                break;
            case 'm':
                file << MATRIX_MARK << " " << variables[i].ID << " " << variables[i].km.get_string() << ";";
                if (variables[i].comment.length() > 0 && KVar::save_comments){
                    file << " //" << variables[i].comment;
                }
                file << endl;
                break;
            case 'b':
                file << BOOL_MARK << " " << variables[i].ID << " " << bool_to_str(variables[i].b) << ";";
                if (variables[i].comment.length() > 0 && KVar::save_comments){
                    file << " //" << variables[i].comment;
                }
                file << endl;
                break;
            case 's':
                file << STRING_MARK << " " << variables[i].ID << " \"" << variables[i].s << "\";";
                if (variables[i].comment.length() > 0 && KVar::save_comments){
                    file << " //" << variables[i].comment;
                }
                file << endl;
                break;
            default:
                break;
        }
    }
    
    file.close();
    
    return true;
}

bool KVar::write_KV2(std::string filename){
    
    ofstream file;
    file.open(filename, std::ofstream::out | std::ofstream::trunc | std::ofstream::binary);
    if (!file.is_open()){
        return false;
    }
    
    
    file << "KV2>VERSION2.0<>";
    file << header << endl;
    file << "<";
    
    for (int i = 0 ; i < variables.size() ; i++){
        switch (variables[i].type) {
            case 'd':
                file << ">d$" << variables[i].ID << " = " << hp_string(variables[i].d) << ";";
                if (variables[i].comment.length() > 0 && KVar::save_comments){
                    file << " //" << variables[i].comment;
                }
                file << endl;
                break;
            case 'm':
                file << "matrix " << variables[i].ID << " = " << variables[i].km.get_string() << ";";
                if (variables[i].comment.length() > 0 && KVar::save_comments){
                    file << " //" << variables[i].comment;
                }
                file << endl;
                break;
            case 'b':
                file << "bool " << variables[i].ID << " = " << bool_to_str(variables[i].b) << ";";
                if (variables[i].comment.length() > 0 && KVar::save_comments){
                    file << " //" << variables[i].comment;
                }
                file << endl;
                break;
            case 's':
                file << "string " << variables[i].ID << " = \"" << variables[i].s << "\";";
                if (variables[i].comment.length() > 0 && KVar::save_comments){
                    file << " //" << variables[i].comment;
                }
                file << endl;
                break;
            default:
                break;
        }
    }
    
    file.close();
    
    return true;
}

bool KVar::write_KV3(std::string filename){
    
    return true;
}

/*-----------------------------------------------------*\
|----------------- STANDARD FUNCTIONS ------------------|
\*-----------------------------------------------------*/

void KVar::print(int indent, bool use_spaces){

    string comment_string(" \tCMNT: "); //" CM\\:"
    
	//Doubles
	int counter = 0;
	for (int i = 0 ; i < variables.size() ; i++){
		if (variables[i].type == 'd'){
			if (counter == 0) cout << indent_line(indent, use_spaces, false) << "DOUBLES:" << endl; //Print header if first double
            bool use_scientific_notation = (select_notation(variables[i].d, KVar::print_threshold) == 's');
            cout << indent_line(indent, use_spaces, false) << "\t" << variables[i].ID << ": " << hp_string(variables[i].d, KVar::print_precision, use_scientific_notation); //Print name and value
            if (print_comments && variables[i].comment.length() > 0) cout << comment_string << variables[i].comment;
            cout << endl;
			counter++; //Increment counter
		}
	}

	//Matricies
	counter = 0;
	for (int i = 0 ; i < variables.size() ; i++){
		if (variables[i].type == 'm'){
			if (counter == 0) cout << indent_line(indent, use_spaces, false) << "MATRICIES:" << endl; //Print header if first double
			cout << indent_line(indent, use_spaces, false) << "\t" << variables[i].ID << ": "; //Print name and value
            if (print_comments && variables[i].comment.length() > 0) cout << comment_string << variables[i].comment;
            cout << endl;
			KMatrix km(variables[i].km);
			int mindent = indent;
			if (!use_spaces) mindent *= 8;
			mindent += 12;
			km.print(mindent, true);
			counter++; //Increment counter
		}
	}

	//Strings
	counter = 0;
	for (int i = 0 ; i < variables.size() ; i++){
		if (variables[i].type == 's'){
			if (counter == 0) cout << indent_line(indent, use_spaces, false) << "STRINGS:" << endl; //Print header if first double
			cout << indent_line(indent, use_spaces, false) << "\t" << variables[i].ID << ": " << variables[i].s; //Print name and value
            if (print_comments && variables[i].comment.length() > 0) cout << comment_string << variables[i].comment;
			counter++; //Increment counter
            cout << endl;
		}
	}

	//Bools
	counter = 0;
	for (int i = 0 ; i < variables.size() ; i++){
		if (variables[i].type == 'b'){
			if (counter == 0) cout << indent_line(indent, use_spaces, false) << "BOOLEANS:" << endl; //Print header if first double
			cout << indent_line(indent, use_spaces, false) << "\t" << variables[i].ID << ": " << bool_to_str(variables[i].b); //Print name and value
            if (print_comments && variables[i].comment.length() > 0) cout << comment_string << variables[i].comment;
            cout << endl;
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

	if (!KVar::get_var_index(var_id, lindex)){
		return false;
	}

	*lindex = KVar::count_type(*lindex, *type)-1;
    
	return true;
}

/*
Gives location information for variable represented by 'var_id'

var_id - variable ID of the variable of interest
index - the index of the variable 'var_id' in vector 'variables'

Returns false if variable is not found - 'index' is set to -1 and 'type' to '\0'
*/
bool KVar::get_var_index(string var_id, int* index){

	*index = -1;
	for (int i = 0 ; i < variables.size() ; i++){
		if (variables[i].ID == var_id){
			*index = i;
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
