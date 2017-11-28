/*

*/

/*
Eigen Help: http://eigen.tuxfamily.org/dox/group__TutorialMatrixArithmetic.html

Operator Overloading: http://stackoverflow.com/questions/4421706/operator-overloading
Copy-and-Swap Idiom: http://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
*/

#include <iostream> //Standard includes
#include <stdio.h>
#include <string>
#include <vector>

#include "Libraries/Eigen/Eigen" //Eigen

#ifndef KMATRIX_HPP
#define KMATRIX_HPP

class KMatrix{
public:

	//Initialization functions
	KMatrix();
	KMatrix(int r, int c);
	KMatrix(std::string init);
	KMatrix(Eigen::MatrixXd init);
	KMatrix(std::vector<std::vector<double> > init);
	KMatrix(Eigen::Vector3d init);
	KMatrix(const KMatrix& other); //Copy-constructor
	~KMatrix();
	bool is_open();

	//Operator Overloads
	friend void swap(KMatrix& first, KMatrix& second);
	KMatrix& operator+=(const KMatrix& rv);
	KMatrix& operator*=(const KMatrix& rv);
	KMatrix& operator-=(const KMatrix& rv);
	// KMatrix& operator/=(const KMatrix& rv);
	KMatrix& operator=(KMatrix rh);
	double& operator()(int r, int c);
	KMatrix& operator=(std::string rv);
	KMatrix& operator=(Eigen::MatrixXd rv);
	KMatrix& operator=(std::vector<std::vector<double> > rv);

	//Access/Modify Functions
	size_t rows();
	size_t cols();
	KMatrix get_row(int i, int* ec = NULL);
	std::vector<double> get_rowv(int i, int* ec = NULL);
	KMatrix get_col(int i, int* ec = NULL);
	std::vector<double> get_colv(int i, int* ec = NULL);
	bool set_row(int r, KMatrix& in);
	bool set_row(int r, std::string in);
	bool set_row(int r, std::vector<double> v);
	bool set_col(int c, KMatrix& in);
	bool set_col(int c, std::string in);
	bool set_col(int c, std::vector<double> v);
    void add_double(double d);
    void multiply_double(double d);
    void power_double(double d);
	Eigen::MatrixXd to_Eigen();
	std::string get_string();

	//Arithmetic Functions
	KMatrix crossprd(KMatrix& rv);
	double dotprd(KMatrix& rv);
	KMatrix transpose();
	KMatrix conjugate();
	KMatrix adjoint(); //Conjugate transpose
	KMatrix inverse();
	bool is_invertable();
	KMatrix pseudoinverse(); //Moore-penrose //DO THIS
	double determinant();

	//Standard Functions
	void print(int indent=0, bool spaces=false);
	void println(int indent=0, bool spaces=false);

	//Static Functions
	static KMatrix Zero(int r, int c);
	static KMatrix Zero(int rc);
	static KMatrix Constant(double val, int r, int c);
	static KMatrix Range(double start, double step_size, double end, int rows=1);
	static std::vector<std::vector<double> > KMatrix_to_vector(KMatrix km);

private:

	//Objects
	Eigen::MatrixXd mtrx;

	//Variables
	bool is_open_b;

	//Private functions
	bool matrix_from_string(std::string input, Eigen::MatrixXd& mtx, bool prin_on=false);
};

KMatrix operator+(KMatrix lv, const KMatrix& rv);
KMatrix operator-(KMatrix lv, const KMatrix& rv);
KMatrix operator*(KMatrix lv, const KMatrix& rv);
// KMatrix operator/(KMatrix lv, const KMatrix& rv);

#endif
