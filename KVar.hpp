#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>

#include "Libraries/Eigen/Eigen"

// #include "../KMatrix/KMatrix.hpp"
// #include "../UFiles/string_manip.hpp"
// #include "../UFiles/stdutil.hpp"
#include "KMatrix.hpp"
#include "string_manip.hpp"
#include "stdutil.hpp"

#ifndef KVAR_HPP
#define KVAR_HPP

//User Macros
#define KV_COMMENT_STYLE 1
	// KV_COMMENT_STYLE options
	#define KV_COMMENT_ABOVE 1.0
	#define KV_COMMENT_SAME_LINE 2.0
#define KV_DOUBLE_SPACE 2
#define KV_SAVE_COMMENTS 3
#define KV_READ_COMMENTS 4
#define KV_ALLOW_PRINT 5

#define KV_HEADER_COMMENT "KVar_Header_Comment_NOT_A_VARIABLE"

typedef struct{
	std::string ID;
	char type;
}identifier;

class KVar{
public:
	KVar();
	KVar(std::string filename);
	~KVar();

	void set(int prop, double val);
	void set(int prop, bool val=true);
	void set(int prop, std::string val);

	//Access info
	bool contains(std::string var_id);
	void clear();
	std::vector<std::string> get_variable_IDs();
	int num_variables();
	bool get_var_index(std::string var_id, int* index, char* type);
	bool get_var_lindex(std::string var_id, int* index, char* type);
	char get_var_type(std::string var_id);

	//Add/remove variables
	bool add_double(std::string var_id, double val, std::string comment="");
	bool get_double(std::string var_id, double& out);
    bool set_double(std::string var_id, double in);
	bool add_matrix(std::string var_id, KMatrix& val, std::string comment="");
	bool get_matrix(std::string var_id, KMatrix& out);
    bool set_matrix(std::string var_id, KMatrix in);
	bool add_matrix(std::string var_id, Eigen::MatrixXd& val, std::string comment="");
	bool get_matrix(std::string var_id, Eigen::MatrixXd& out);
    bool set_matrix(std::string var_id, Eigen::MatrixXd in);
	bool add_string(std::string var_id, std::string val, std::string comment="");
	bool get_string(std::string var_id, std::string& out);
    bool set_string(std::string var_id, std::string in);
	bool add_bool(std::string var_id, bool val, std::string comment="");
	bool get_bool(std::string var_id, bool& out);
    bool set_bool(std::string var_id, bool in);
	std::string get_comment(std::string var_id, bool& success);
	std::string get_comment(std::string var_id);
    bool delete_var(std::string var_id);

	//Read write functions
	bool load_file(std::string filename, bool print_out=false); // DO THIS - Not yet done - headers not implemented, string reading must still incorperate special comments, I must compile and test the function, and I must write the function to do a full-match comment check (for // comments)
	bool write_file(std::string filename); //DO THIS

	//Standard functions
	void print(int indent=0, bool use_spaces=false);

private:

	//Settings Variables
	bool save_comments;
	bool read_comments;
	int comment_style;
	bool double_space_comment;
	bool print_allowed;

	//Vectors
	std::vector<identifier> IDs;
	std::vector<std::string> comments; //Alligns with IDs in vector
	std::vector<double> double_vars;
	// std::vector<KMatrix> kmatrix_vars;
	std::vector<Eigen::MatrixXd> matrix_vars;
	std::vector<std::string> string_vars;
	std::vector<bool> bool_vars;

	//Private Functions
	int count_type(int index, char type=-1);

};


#endif
