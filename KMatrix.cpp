#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

// #include "../Libraries/Eigen/Eigen" Included in hpp

#include "KMatrix.hpp"
#include "string_manip.hpp"
#include "stdutil.hpp"

using namespace std;
using namespace Eigen;

/*-----------------------------------------------------*\
|-------------- INITIALIZATION FUNCTIONS ---------------|
\*-----------------------------------------------------*/

/*
Default constructor. Initializes to 1x1 matrix of 0s.
*/
KMatrix::KMatrix() : mtrx(1,1){
	mtrx = MatrixXd::Zero(1,1);
	KMatrix::is_open_b = true;
}

/*
Constructor. Initializes to 'r'x'c' matrix of 0s.
*/
KMatrix::KMatrix(int r, int c) : mtrx(r, c){
	mtrx = MatrixXd::Zero(r, c);
	KMatrix::is_open_b = true;
}

/*
Constructor. Creates KMatrix from string 'init'.
*/
KMatrix::KMatrix(string init) : mtrx(1, 1){
	mtrx = MatrixXd::Zero(1, 1);
	if (!KMatrix::matrix_from_string(init, mtrx)){
		KMatrix::is_open_b = false;
	}else{
		KMatrix::is_open_b = true;
	}
}

/*
Copy constructor. Creates KMatrix from KMatrix
*/
KMatrix::KMatrix(const KMatrix& other){ //Copy-constructor
	KMatrix::mtrx = other.mtrx;
	KMatrix::is_open_b = other.is_open_b;
}

/*
Constructor. Creates KMatrix from Eigen::MatrixXd
*/
KMatrix::KMatrix(Eigen::MatrixXd other){
	KMatrix::mtrx = other;
	KMatrix::is_open_b = true;
}

/*
Constructor. Creates KMatrix from vector of vector of doubles.
*/
KMatrix::KMatrix(std::vector<std::vector<double> > rv){

	KMatrix::is_open_b = true;

	//Get number of rows and columns, verify vector is correct dimensions
	int r = rv.size();
	if (r <= 0){ //Make sure vector isn't empty
		KMatrix::mtrx = MatrixXd::Zero(1, 1);
		KMatrix::is_open_b = false;
	}else{

		int c = rv[0].size();
		for (int i = 0 ; i < rv.size() ; i++){ //Make sure dimensions match
			if (rv[i].size() != c){
				KMatrix::mtrx = MatrixXd::Zero(1, 1);
				KMatrix::is_open_b = false;
				break;
			}
		}

		if (is_open_b){
			//Populate
			KMatrix::mtrx.resize(r, c);
			for (int rr = 0 ; rr < r ; rr++){
				for (int cc = 0 ; cc < c ; cc++ ){
					KMatrix::mtrx(rr, cc) = rv[rr][cc];
				}
			}
		}
	}
}

/*
Constructor. Creates a 1x3 matrix form an Eigen::Vector3d
*/
KMatrix::KMatrix(Eigen::Vector3d init) : mtrx(1, 3){

	KMatrix::mtrx(0, 0) = init(0);
	KMatrix::mtrx(0, 1) = init(1);
	KMatrix::mtrx(0, 2) = init(2);

	is_open_b = true;
}

/*
Deconstructor.
*/
KMatrix::~KMatrix(){

}

/*
Reports state of KMatrix (open or closed). Only useful immediately after constructing matrix to see if string, vector, or other initialization technique failed.

No parameters

Returns true if open.
*/
bool KMatrix::is_open(){
	return KMatrix::is_open_b;
}

/*-----------------------------------------------------*\
|----------------- OPERATOR OVERLOADS ------------------|
\*-----------------------------------------------------*/

/*
Swap function. Swaps contents in memory of 2 KMatricies

Parameters:
first - first KMatrix to swap
second - second KMatrix to swap

Void return
*/
void swap(KMatrix& first, KMatrix& second){
	std::swap(first.mtrx, second.mtrx);
	std::swap(first.is_open_b, second.is_open_b);
}

/*
+=  Operator. Adds contents of KMatricies and saves to left KMatrix. Adds cell by cell.

Parameters:
rv - right matrix

Returns reference to left KMatrix
*/
KMatrix& KMatrix::operator+=(const KMatrix& rv){
	KMatrix::mtrx += rv.mtrx;
	return *this;
}

/*
+  Operator. Adds contents of KMatricies cell by cell

Parameters:
lv - left matrix
rv - right matrix

Returns sum KMatrix
*/
KMatrix operator+(KMatrix lv, const KMatrix& rv){
	lv += rv;
	return lv;
}

/*
-=  Operator. Adds contents of KMatricies and saves to left KMatrix. Subtracts cell by cell.

Parameters:
rv - right matrix

Returns reference to left KMatrix
*/
KMatrix& KMatrix::operator-=(const KMatrix& rv){
	KMatrix::mtrx -= rv.mtrx;
	return *this;
}

/*
subtracts cell by cell

Parameters:
lv - left KMatrix
rv - right KMatrix

Returns sum
*/
KMatrix operator-(KMatrix lv, const KMatrix& rv){
	lv -= rv;
	return lv;
}

/*
Multiplies KMatricies using matrix multiplication.

Parameters:
rv - right matrix

Return left matrix
*/
KMatrix& KMatrix::operator*=(const KMatrix& rv){
	KMatrix::mtrx *= rv.mtrx;
	return *this;
}

/*
Multiplies with matrix multiplication

Parameters:
lv - left matrix
rv - right matrix

Return product
*/
KMatrix operator*(KMatrix lv, const KMatrix& rv){
	lv *= rv;
	return lv;
}

// /*
// Divides matricies.

// Parameters:
// rv - right matrix

// Return left matrix
// */
// KMatrix& KMatrix::operator/=(const KMatrix& rv){
// 	KMatrix::mtrx += rv.mtrx;
// 	return *this;
// }

// KMatrix operator/(KMatrix lv, const KMatrix& rv){
// 	lv /= rv;
// 	return lv;
// }

/*
Assignment operator. Assigns one KMatrix to a second

Parameters:
other - matrix to assign to '*this'

Returns reference to 'this' matrix
*/
KMatrix& KMatrix::operator=(KMatrix other){
	swap(*this, other);
	return *this;
}

/*
Access elements in the KMatricies

Parameters:
r - row to access
c - column to access

Returns a reference to that cell
*/
double& KMatrix::operator()(int r, int c){
	return KMatrix::mtrx(r, c);
}

/*
Assignment operator for string

Parameters:
other - string to convert to KMatrix and assign to this

Returns reference to this KMatrix
*/
KMatrix& KMatrix::operator=(string other){
	mtrx = MatrixXd::Zero(1, 1);
	if (!KMatrix::matrix_from_string(other, mtrx)){
		KMatrix::is_open_b = false;
	}else{
		KMatrix::is_open_b = true;
	}
	return *this;
}

/*
Assignment operator for Eigen::MatixXd

Parameters:
other - Eigen matrix

Returns this matrix
*/
KMatrix& KMatrix::operator=(MatrixXd other){
	mtrx = other;
	KMatrix::is_open_b = true;
	return *this;
}

/*
Assignment operator for vector of vector of double

Parameters:
rv - vector to assign to matrix

Returns this matrix
*/
KMatrix& KMatrix::operator=(vector<vector<double> > rv){

	//Get number of rows and columns, verify vector is correct dimensions
	int r = rv.size();

	if (r <= 0){
		KMatrix::mtrx = MatrixXd::Zero(1, 1);
		KMatrix::is_open_b = false;
		return *this;
	}

	int c = rv[0].size();
	for (int i = 0 ; i < rv.size() ; i++){
		if (rv[i].size() != c){
			KMatrix::mtrx = MatrixXd::Zero(1, 1);
			KMatrix::is_open_b = false;
			return *this;
		}
	}

	//Populate
	KMatrix::mtrx.resize(r, c);

	for (int rr = 0 ; rr < r ; rr++){
		for (int cc = 0 ; cc < c ; cc++ ){
			KMatrix::mtrx(rr, cc) = rv[rr][cc];
		}
	}

	KMatrix::is_open_b = true;
	return *this;
}

/*-----------------------------------------------------*\
|----------------- MOD/READ FUNCTIONS ------------------|
\*-----------------------------------------------------*/

/*
Get number of rows

Returns number of rows
*/
size_t KMatrix::rows(){
	return KMatrix::mtrx.rows();
}

/*
Get number of columns

Returns number of columns
*/
size_t KMatrix::cols(){
	return KMatrix::mtrx.cols();
}

/*
Return Eigen::MAtrixXd with contents same as this KMatrix

Returns the Eigen::MatrixXd that is this KMatrix
*/
Eigen::MatrixXd KMatrix::to_Eigen(){
	return this->mtrx;
}

/*
Returns a KMatrix representing the selected row of this KMatrix. Non assignable

parameters:
r - row
ec - error code pointer

Returns KMatrix of row. ec = -1 with error.
*/
KMatrix KMatrix::get_row(int r, int* ec){

	 //Verify it is in range
	 if (r+1 > KMatrix::rows() || r < 0){
		 *ec = -1;
		 KMatrix km;
		 return km;
	 }

	 //Create row
	 KMatrix km(1, KMatrix::cols());
	 for (int i = 0 ; i < KMatrix::cols() ; i++){
		 km(0, i) = KMatrix::operator()(r, i);
	 }

	 return km;
 }

/*
Return a vector or doubles representing the selected row of this KMatrix.

Parameters:
r - row
ec - error code pointer

Returns vector representing the selected row. ec = -1 with error
*/
vector<double> KMatrix::get_rowv(int r, int* ec){

	 //Verify it is in range
	 if (r+1 > KMatrix::rows() || r < 0){
		 *ec = -1;
		 vector<double> v;
		 return v;
	 }

	 //Create row
	 vector<double> km;
	 for (int i = 0 ; i < KMatrix::cols() ; i++){
		 km.push_back( KMatrix::operator()(r, i) );
	 }

	 return km;
 }

/*
Returns a KMatrix representing the selected column. Non-assignable

Parameters:
r - column
ec - error code

Returns KMatricx representing column. ec = -1 with error
*/
 KMatrix KMatrix::get_col(int r, int* ec){

	 //Verify it is in range
	 if (r+1 > KMatrix::cols() || r < 0){
		 *ec = -1;
		 KMatrix km;
		 return km;
	 }

	 //Create col
	 KMatrix km(KMatrix::rows(), 1);
	 for (int i = 0 ; i < KMatrix::rows() ; i++){
		 km(i, 0) = KMatrix::operator()(i, r);
	 }

	 return km;
 }

/*
Returns a double vector representing column of KMatrix

Parameters:
r - column
ec - error code.

Returns vector representing column. ec = -1 with error
*/
 vector<double> KMatrix::get_colv(int r, int* ec){

	 //Verify it is in range
	 if (r+1 > KMatrix::cols() || r < 0){
		 *ec = -1;
		 vector<double> v;
		 return v;
	 }

	 //Create row
	 vector<double> km;
	 for (int i = 0 ; i < KMatrix::rows() ; i++){
		 km.push_back( KMatrix::operator()(i, r) );
	 }

	 return km;
 }

/*
Set a row, assigning the contents in 'in'

Parameters:
r - row to which to assign
in - value to assign to row

Returns true if success
*/
 bool KMatrix::set_row(int r, KMatrix& in){

	 //Make sure r does not exceed # rows in matrix
	 if (r < 0 || r+1 > KMatrix::rows()){
		 return false;
	 }

	 //Ensure in is large enough
	 if (in.cols() < KMatrix::cols()){
		 return false;
	 }

	 for (int i = 0 ; i < KMatrix::cols() ; i++){
		 KMatrix::operator()(r, i) = in(0, i);
	 }

	 return true;
 }

/*
Set a row, assigning the contents in 'in'

Parameters:
r - row to which to assign
in - value to assign to row

Returns true if success
*/
 bool KMatrix::set_row(int r, string in){
	 KMatrix km(in);
	 return KMatrix::set_row(r, km);
 }

/*
Set a row, assigning the contents in 'v'

Parameters:
r - row to which to assign
v - value to assign to row

Returns true if success
*/
 bool KMatrix::set_row(int r, vector<double> v){

	 //ensure 'r' in bounds
	 if (r < 0 || r+1 > KMatrix::rows()){
		 return false;
	 }

	 //Insure 'v' large enough
	 if (v.size() < KMatrix::cols()){
		 return false;
	 }

	 //copy
	 for (int i = 0 ; i < KMatrix::cols() ; i++){
		 KMatrix::operator()(r, i) = v[i];
	 }

	 return true;
 }

/*
Set a col, assigning the contents in 'in'

Parameters:
r - col to which to assign
in - value to assign to col

Returns true if success
*/
 bool KMatrix::set_col(int r, KMatrix& in){

	 //Make sure r does not exceed # rows in matrix
	 if (r < 0 || r+1 > KMatrix::cols()){
		 return false;
	 }

	 //Ensure in is large enough
	 if (in.rows() < KMatrix::rows()){
		 return false;
	 }

	 for (int i = 0 ; i < KMatrix::rows() ; i++){
		 KMatrix::operator()(i, r) = in(i, 0);
	 }

	 return true;
 }

/*
Set a col, assigning the contents in 'in'

Parameters:
r - col to which to assign
in - value to assign to col

Returns true if success
*/
bool KMatrix::set_col(int r, string in){
	KMatrix km(in);
	return KMatrix::set_col(r, km);
}

/*
Set a col, assigning the contents in 'v'

Parameters:
r - col to which to assign
v - value to assign to col

Returns true if success
*/
bool KMatrix::set_col(int r, vector<double> v){

	 //ensure 'r' in bounds
	 if (r < 0 || r+1 > KMatrix::cols()){
		 return false;
	 }

	 //Insure 'v' large enough
	 if (v.size() < KMatrix::rows()){
		 return false;
	 }

	//copy
	for (int i = 0 ; i < KMatrix::rows() ; i++){
		KMatrix::operator()(i, r) = v[i];
	}

	return true;
}

/*
Creates a one line string representing the matrix. It can be interpreted by KMatrix string interpreters.

Returns the string equivilent of the matrix.
*/
string KMatrix::get_string(){

	string output;

	//Print matrix
	output = output + "[ ";
	for (int r = 0; r < KMatrix::mtrx.rows() ; r++){

		for (int c = 0; c+1 < KMatrix::mtrx.cols() ; c++){
			output = output + to_string(KMatrix::mtrx(r, c)) + ", ";
		}

		output = output + to_string(KMatrix::mtrx(r, KMatrix::mtrx.cols()-1));
		if (r+1 < KMatrix::mtrx.rows()){
			output = output + "; ";
		}
	}
	output = output + " ]";
	return output;
}

/*-----------------------------------------------------*\
|---------------- ARITHMETIC FUNCTIONS -----------------|
\*-----------------------------------------------------*/

/*
Perform cross product with matrix and rv

Parameters:
rv - right matrix

Returns cross product
*/
KMatrix KMatrix::crossprd(KMatrix& rv){

	Eigen::Vector3d v1;
	Eigen::Vector3d v2;

	try{
		v1(0) = KMatrix::mtrx(0, 0);
		v1(1) = KMatrix::mtrx(0, 1);
		v1(2) = KMatrix::mtrx(0, 2);
		v2(0) = rv(0, 0);
		v2(1) = rv(0, 1);
		v2(2) = rv(0, 2);

	}catch(...){
		cout << "ERROR: Failed to create Vector3d from KMatrix." << endl;
		KMatrix km(1, 1);
		km(1, 1) = -1;
		return km;
	}

	KMatrix km(v1.cross(v2));
	return km;
}

/*
Perform dot product

Parameters:
rv - right matrix

Returns product
*/
double KMatrix::dotprd(KMatrix& rv){

	Eigen::Vector3d v1;
	Eigen::Vector3d v2;

	try{
		v1(0) = KMatrix::mtrx(0, 0);
		v1(1) = KMatrix::mtrx(0, 1);
		v1(2) = KMatrix::mtrx(0, 2);
		v2(0) = rv(0, 0);
		v2(1) = rv(0, 1);
		v2(2) = rv(0, 2);

	}catch(...){
		cout << "ERROR: Failed to create Vector3d from KMatrix." << endl;
		return -1;
	}

	return v1.dot(v2);
}

/*
Transpose Matrix. Does not mutate matrix, just returns new translated matrix.

Returns transposed matrix
*/
KMatrix KMatrix::transpose(){
	KMatrix m;
	m = KMatrix::mtrx.transpose();
	return m;
}

/*
Return conjugate of matrix. Non-mutating

Return new matrix that is conjugate of *this
*/
KMatrix KMatrix::conjugate(){
	KMatrix m;
	m = KMatrix::mtrx.conjugate();
	return m;
}

/*
Calculate conjugate transpose matrix. Non-mutating

Return adjoint matrix
*/
KMatrix KMatrix::adjoint(){
	KMatrix m;
	m = KMatrix::mtrx.adjoint();
	return m;
}

/*
Calculate inverse matrix. Non-mutaing

Return inverse
*/
KMatrix KMatrix::inverse(){
	KMatrix m;
	m = KMatrix::mtrx.inverse();
	return m;
}

/*
Calculate determinant

Returns determinant
*/
double KMatrix::determinant(){
	return KMatrix::mtrx.determinant();
}

/*
Determine if matrix is invertable

Returns true if it can be inverted
*/
bool KMatrix::is_invertable(){
	Eigen::MatrixXd m(KMatrix::rows(), KMatrix::cols());
	m = KMatrix::mtrx;
	FullPivLU<MatrixXd> LUDec(m);
	return LUDec.isInvertible();
}

/*-----------------------------------------------------*\
|----------------- STANDARD FUNCTIONS ------------------|
\*-----------------------------------------------------*/

/*
Print matrix with line return after print complete.

Parameters:
indent - number of indentations to perform before printing matrix
spaces - use spaces instead of tabs for indent. Default false

Void return
*/
void KMatrix::println(int indent, bool spaces){
	KMatrix::print(indent, spaces);
	cout << endl;
}

/*
Print matrix

Parameters:
indent - number of indentations to perform before printing matrix
spaces - use spaces instead of tabs for indent. Default false

Void return
*/
void KMatrix::print(int indent, bool spaces){

	//Choose indentation type (spaces or tabs)
	char dent_char;
	if (spaces){
		dent_char = ' ';
	}else{
		dent_char = '\t';
	}


	int max_abs = 0;
	for (int r = 0; r < KMatrix::mtrx.rows() ; r++){
		for (int c = 0; c+1 < KMatrix::mtrx.cols() ; c++){
			if (fabs(KMatrix::mtrx(r, c) > max_abs)){
				max_abs = fabs(KMatrix::mtrx(r, c));
			}
		}
	}
	max_abs = floor(log10(max_abs)) + 1; //Number of digits used

	//Print matrix
	int gap;
	cout.precision(2);
	for (int r = 0; r < KMatrix::mtrx.rows() ; r++){

		for (int i = 0; i < indent ; i++){
			cout << dent_char;
		}

		cout << "| ";
		for (int c = 0; c+1 < KMatrix::mtrx.cols() ; c++){
			gap = max_abs - (floor(log10(KMatrix::mtrx(r, c))) + 1);
			for (int i = 0 ; i < gap ; i++){
				cout << ' ';
			}
			cout << KMatrix::mtrx(r, c) << ", ";
		}
		gap = max_abs - (floor(log10(KMatrix::mtrx(r, KMatrix::mtrx.cols()-1))) + 1);
		for (int i = 0 ; i < gap ; i++){
			cout << ' ';
		}
		cout << KMatrix::mtrx(r, KMatrix::mtrx.cols()-1) << " |" << endl;
	}

}

/*-----------------------------------------------------*\
|------------------- STATIC FUNCTIONS ------------------|
\*-----------------------------------------------------*/

/*
Return Kmatrix of zeros of dimension 'r'x'c'

Parameters:
r - rows
c - columns

Returns KMatrix
*/
KMatrix KMatrix::Zero(int r, int c){

	KMatrix m(r, c);
	return m;

}

/*
Returns square matrix of side length rc

Parameters:
rc - side length

Return Kmatrix
*/
KMatrix KMatrix::Zero(int rc){

	KMatrix m(rc, rc);
	return m;

}

/*
Returns a Kmatrix of all one value 'val' of dimension 'r'x'c'

Parameters:
r - rows
c - columns

Returns Kmatrix
*/
KMatrix KMatrix::Constant(double val, int r, int c){

	KMatrix m(r, c);

	for (int rr = 0 ; rr < r ; rr++){
		for (int cc = 0 ;cc < c ; cc++){
			m(rr, cc) = val;
		}
	}

	return m;
}

/*
Returns a matrix of values rangin from 'begin' to 'end', incrementing in values of 'step_size'. If it does not fit into 'rows' perfectly, remaining cells filled with 0s

Parameters:
begin - begin value (inclusive)
step_size - size to increment
end - end value (inclusive)
rows - number of rows for matrix. defualt 1
*/
KMatrix KMatrix::Range(double begin, double step_size, double end, int rows){

	int len = floor((end - begin) / step_size + .001) + 1;
	int ncols = ceil((double)len/rows);
	cout << "L: " << len << " R: " << rows << " col: " << ncols << endl;
	KMatrix m(rows, ncols);

	for (int r = 0; r < rows ; r++){
		for (int c = 0 ; c < ncols ; c++){
			if (r*ncols + c >= len){
				break;
			}
			m(r, c) = (r * ncols + c) * step_size + begin;
		}
	}

	return m;
}

/*
Creates a vector of vector of doubles from Kmatrix

Parameters:
km - kmatrix to convert

Returns vector equivilent
*/
vector<vector<double> > KMatrix::KMatrix_to_vector(KMatrix km){

	vector<vector<double> > out;
	vector<double> temp;

	for(int r = 0 ; r < km.rows() ; r++){
		temp.clear();
		for(int c = 0; c < km.cols() ; c++){
			temp.push_back( km(r, c) );
		}
		out.push_back(temp);
	}

	return out;
}

/*-----------------------------------------------------*\
|------------------ PRIVATE FUNCTIONS ------------------|
\*-----------------------------------------------------*/

/*
Creates a KMatrix from a string representing matrix contents.

Parameters:
inpt - input string
out - matrix to populate
print_on - allow function to print to std out? Default false

Returns true on success
*/
bool KMatrix::matrix_from_string(string input, MatrixXd& out, bool print_on){

	int c = -1;
	int r = 0;
	vector<double> values;

	ensure_whitespace(input, ",;");
	vector<string> tokens = parse(input, " ");

	int ticker = 0;
	bool add_line = false;
	bool req_commas = false;
	char phase = 'b';
	for (int i = 0 ; i < tokens.size() ; i++){
		if (isnum(tokens[i])){

			if (req_commas && phase != 'c' && phase != 'n'){
				cout << "ERROR: Failed to create matrix from string - missing comma before token " << i << ", '" << tokens[i] << "'. String:\n\t'" << input << "'" << endl;
				return false;
			}

			values.push_back(strtod(tokens[i]));
			ticker++;
			add_line = true;
			phase = 'v'; //var added
		}else if(tokens[i] == ";"){

			if (phase == 'c'){
				if (print_on) cout << "ERROR: Failed to create matrix from string - semicolon can not follow a comma. String:\n\t'" << input << "'" << endl;
				return false;
			}else if(phase == 'b'){
				if (print_on) cout << "ERROR: Failed to create matrix from string - semicolon can not be first symbol. String:\n\t'" << input << "'" << endl;
				return false;
			}

			if (c == -1){
				c = ticker;
			}else if(c != ticker){
				if (print_on) cout << "ERROR: Failed to create matrix from string:\n\t'" << input << "'" << endl;
				return false;
			}
			phase = 'n'; //new row
			add_line = false;
			r++;
			ticker = 0;
		}else if(tokens[i] == ","){
			if ( r == 0 && ticker == 1){ //begin
				req_commas = true;
				phase = 'c';
			}else if(req_commas && 'v'){
				phase = 'c'; //Ready for next var
			}else{
				if (print_on) cout << "ERROR: Failed to create matrix from string due to invalid comma. String:\n\t'" << input << "'" << endl;
				return false;
			}
		}else{

		}
	}

	if (add_line){
		 c = ticker;
		 r++;
	}

	//Resize and fill array
	out.resize(r, c);
	for (int rr = 0 ; rr < r ; rr++){
		for (int cc = 0 ; cc < c ; cc++){
			out(rr, cc) = values[rr*c + cc];
		}
	}

	return true;
}
