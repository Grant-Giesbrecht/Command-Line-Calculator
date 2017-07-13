#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>

#include "Libraries/Eigen/Eigen"

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
#define KV_PRINT_COMMENT 6
#define KV_PRINT_PRECISION 7
#define KV_PRINT_THRESHOLD 8
#define KV_OVERWRITE_ON_LOAD 9

#define KV_HEADER_COMMENT "KVar_Header_Comment_NOT_A_VARIABLE"

typedef struct{
	std::string ID;
	char type;
}identifier;

typedef struct{
    char type;
    KMatrix km;
    double d;
    bool b;
    std::string s;
    std::string comment;
    std::string ID;
}aktc; //all ktype commented

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
	bool get_var_index(std::string var_id, int* index);
    
	bool get_var_lindex(std::string var_id, int* index, char* type);
	char get_var_type(std::string var_id);

	//Add/remove variables
    bool add_variable(std::string var_id, double val, std::string comment="");
    bool add_variable(std::string var_id, std::string val, std::string comment="");
    bool add_variable(std::string var_id, KMatrix val, std::string comment="");
    bool add_variable(std::string var_id, Eigen::MatrixXd& val, std::string comment="");
    bool add_variable(std::string var_id, bool val, std::string comment="");
    bool set_variable(std::string var_id, double val);
    bool set_variable(std::string var_id, std::string val);
    bool set_variable(std::string var_id, KMatrix val);
    bool set_variable(std::string var_id, Eigen::MatrixXd& val);
    bool set_variable(std::string var_id, bool val);
    bool set_comment(std::string var_id, std::string comment);
    bool get_variable(std::string var_id, double& output);
    bool get_variable(std::string var_id, std::string& output);
    bool get_variable(std::string var_id, bool& output);
    bool get_variable(std::string var_id, KMatrix& output);
    bool get_variable(std::string var_id, Eigen::MatrixXd& val);
    aktc get_variable(std::string var_id);
    std::string get_comment(std::string var_id, bool& success);
    std::string get_comment(std::string var_id);
    bool delete_var(std::string var_id);
    
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
    std::string get_header();
    void set_header(std::string);
	

	//Read write functions
	bool load_file_old(std::string filename, bool print_out=false); // DO THIS - Not yet done - headers not implemented, string reading must still incorperate special comments, I must compile and test the function, and I must write the function to do a full-match comment check (for // comments)
    bool load_file(std::string filename, long* fail_line=NULL);
    bool load_KV1(std::string filename, long* fail_line=NULL);
    bool load_KV2(std::string filename, long* fail_line=NULL);
    bool load_KV3(std::string filename, long* fail_line=NULL);
	bool write_KV1(std::string filename);
    bool write_KV2(std::string filename);
    bool write_KV3(std::string filename);

	//Standard functions
	void print(int indent=0, bool use_spaces=false);

private:

	//Settings Variables
	bool save_comments;
	bool read_comments;
	int comment_style;
	bool double_space_comment;
	bool print_allowed;
    bool print_comments;
    double print_precision;
    double print_threshold;
    bool overwrite_variables_on_load;

	//Vectors
    std::vector<aktc> variables;
    std::string header;
    
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
