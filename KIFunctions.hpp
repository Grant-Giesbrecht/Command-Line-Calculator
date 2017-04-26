#ifndef KIFUNCTIONS_HPP
#define KIFUNCTIONS_HPP

#include "KInterp.hpp"

void define_functions(std::vector<func_id>* functions);

/*----------------------------------------------------------------------
-------------------- INTERPRETED FUNCTIONS GO HERE ---------------------
----------------------------------------------------------------------*/

all_ktype abs(std::vector<all_ktype> args); //double abs(double x)

all_ktype transpose(std::vector<all_ktype> args); //matrix abs(matrix x)

all_ktype inverse(std::vector<all_ktype> args); //matrix inverse(matrix x)

all_ktype transpose(std::vector<all_ktype> args); //matrix transpose(matrix x);

all_ktype inverse(std::vector<all_ktype> args); //matrix inverse(matrix x);

all_ktype crossprd(std::vector<all_ktype> args); //matrix crossprd(matrix a, matrix b);

all_ktype dotprd(std::vector<all_ktype> args); //double dotprd(matrix a, matrix b);

all_ktype conjugate(std::vector<all_ktype> args); //matrix conjugate(matrix x);

all_ktype adjoint(std::vector<all_ktype> args); //matrix adjoint(matrix x);

all_ktype is_invertable(std::vector<all_ktype> args); //bool is_invertable(matrix x);

all_ktype determinant(std::vector<all_ktype> args); //double determinant(matrix x);

#endif

/*----------------------------------------------------------------------
----------------- INTERPRETED FUNCTION WRITING GUIDES ------------------
----------------------------------------------------------------------//



*/
