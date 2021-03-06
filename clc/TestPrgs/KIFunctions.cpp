#include "KIFunctions.hpp"

#include <ctgmath>
#include <cmath>

using namespace std;

/*
Adds interpreted functions to the list of functions to be recognized by the interpreter.

functions - pointer to vector of functions recognized by interpreter

Void return
*/
void define_functions(vector<func_id>* functions){

	func_id temp;

	/*------------------------------------------------------------
	----- Add interpreted functions to the list of functions -----
	------------------------------------------------------------*/

	// To add a function, the basic syntax below can be followed:
	//
	// 	temp.function = YOUR_FUNCTION'S_NAME_HERE;
	// 	temp.identified = "NAME_OF_FUNCTION_AS_RECOGNIZED_BY_THE_INTERPRETER"; //Can be different from the actual function name, but for clarity it is recommended they match
	// 	functions->push_back(temp);
	//

	//Add function double abs(double x) to list
	temp.function = abs;
	temp.identifier = "abs";
	functions->push_back(temp);
    
    //Add trig functions to list
    temp.function = sin;
	temp.identifier = "sin";
	functions->push_back(temp);
    temp.function = cos;
	temp.identifier = "cos";
	functions->push_back(temp);
    temp.function = tan;
	temp.identifier = "tan";
	functions->push_back(temp);
    temp.function = csc;
	temp.identifier = "csc";
	functions->push_back(temp);
    temp.function = sec;
	temp.identifier = "sec";
	functions->push_back(temp);
    temp.function = cot;
	temp.identifier = "cot";
	functions->push_back(temp);

    //Add inverse trig functions to list
    temp.function = asin;
	temp.identifier = "asin";
	functions->push_back(temp);
    temp.function = acos;
	temp.identifier = "acos";
	functions->push_back(temp);
    temp.function = atan;
	temp.identifier = "atan";
	functions->push_back(temp);
    temp.function = acsc;
	temp.identifier = "acsc";
	functions->push_back(temp);
    temp.function = asec;
	temp.identifier = "asec";
	functions->push_back(temp);
    temp.function = acot;
	temp.identifier = "acot";
	functions->push_back(temp);

    //Add matrix functions to list
    temp.function = transpose;
    temp.identifier = "transpose";
    functions->push_back(temp);
    temp.function = inverse;
    temp.identifier = "inverse";
    functions->push_back(temp);
    temp.function = crossprd;
    temp.identifier = "crossprd";
    functions->push_back(temp);
    temp.function = dotprd;
    temp.identifier = "dotprd";
    functions->push_back(temp);
    temp.function = conjugate;
    temp.identifier = "conjugate";
    functions->push_back(temp);
    temp.function = adjoint;
    temp.identifier = "adjoint";
    functions->push_back(temp);
    temp.function = is_invertable;
    temp.identifier = "is_invertable";
    functions->push_back(temp);
    temp.function = determinant;
    temp.identifier = "determinant";
    functions->push_back(temp);

}

/*
Function for interpreted function 'double abs(double x)'

double abs(double x):
	Calculates the absolute value of x

	x - double whose absolute value to calculate

	returns absolute value of x
*/
all_ktype abs(vector<all_ktype> args){

	all_ktype ret;

	//Verify args
	if (args.size() != 1 || args[0].type != 'd'){
		ret.s = "ERROR: Function 'abs()' requires a single double value as an argument.";
		ret.type = 'e';
		return ret;
	}

	//Create return variable
	ret.d = abs(args[0].d); //Calculate abs()
	ret.type = 'd';

	return ret;
}

/*
Function for interpreted function 'double sin(double x)'

double sin(double x):
	Calculates the sin of x

	x - double whose sin to calculate

	returns sin of x
*/
all_ktype sin(vector<all_ktype> args){

	all_ktype ret;

	//Verify args
	if (args.size() != 1 || args[0].type != 'd'){
		ret.s = "ERROR: Function 'abs()' requires a single double value as an argument.";
		ret.type = 'e';
		return ret;
	}

	//Create return variable
	ret.d = sin(args[0].d); //Calculate abs()
	ret.type = 'd';

	return ret;
}

/*
Function for interpreted function 'double cos(double x)'

double cos(double x):
	Calculates the cos of x

	x - double whose cos to calculate

	returns cos of x
*/
all_ktype cos(vector<all_ktype> args){

	all_ktype ret;

	//Verify args
	if (args.size() != 1 || args[0].type != 'd'){
		ret.s = "ERROR: Function 'abs()' requires a single double value as an argument.";
		ret.type = 'e';
		return ret;
	}

	//Create return variable
	ret.d = cos(args[0].d); //Calculate abs()
	ret.type = 'd';

	return ret;
}

/*
Function for interpreted function 'double tan(double x)'

double tan(double x):
	Calculates the tan of x

	x - double whose tan to calculate

	returns tan of x
*/
all_ktype tan(vector<all_ktype> args){

	all_ktype ret;

	//Verify args
	if (args.size() != 1 || args[0].type != 'd'){
		ret.s = "ERROR: Function 'abs()' requires a single double value as an argument.";
		ret.type = 'e';
		return ret;
	}

	//Create return variable
	ret.d = tan(args[0].d); //Calculate abs()
	ret.type = 'd';

	return ret;
}

/*
Function for interpreted function 'double csc(double x)'

double csc(double x):
	Calculates the csc of x

	x - double whose csc to calculate

	returns csc of x
*/
all_ktype csc(vector<all_ktype> args){

	all_ktype ret;

	//Verify args
	if (args.size() != 1 || args[0].type != 'd'){
		ret.s = "ERROR: Function 'abs()' requires a single double value as an argument.";
		ret.type = 'e';
		return ret;
	}

	//Create return variable
	ret.d = 1/sin(args[0].d);
	ret.type = 'd';

	return ret;
}

/*
Function for interpreted function 'double sec(double x)'

double sec(double x):
	Calculates the sec of x

	x - double whose sec to calculate

	returns sec of x
*/
all_ktype sec(vector<all_ktype> args){

	all_ktype ret;

	//Verify args
	if (args.size() != 1 || args[0].type != 'd'){
		ret.s = "ERROR: Function 'abs()' requires a single double value as an argument.";
		ret.type = 'e';
		return ret;
	}

	//Create return variable
	ret.d = 1/cos(args[0].d);
	ret.type = 'd';

	return ret;
}

/*
Function for interpreted function 'double cot(double x)'

double cot(double x):
	Calculates the csc of x

	x - double whose cot to calculate

	returns cot of x
*/
all_ktype cot(vector<all_ktype> args){

	all_ktype ret;

	//Verify args
	if (args.size() != 1 || args[0].type != 'd'){
		ret.s = "ERROR: Function 'abs()' requires a single double value as an argument.";
		ret.type = 'e';
		return ret;
	}

	//Create return variable
	ret.d = 1/tan(args[0].d);
	ret.type = 'd';

	return ret;
}

/*
Function for interpreted function 'double asin(double x)'

double asin(double x):
	Calculates the arcsine of x

	x - double whose arcsin to calculate

	returns arcsin of x
*/
all_ktype asin(vector<all_ktype> args){

	all_ktype ret;

	//Verify args
	if (args.size() != 1 || args[0].type != 'd'){
		ret.s = "ERROR: Function 'abs()' requires a single double value as an argument.";
		ret.type = 'e';
		return ret;
	}

	//Create return variable
	ret.d = asin(args[0].d);
	ret.type = 'd';

	return ret;
}

/*
Function for interpreted function 'double acos(double x)'

double acos(double x):
	Calculates the arccosine of x

	x - double whose arccosine to calculate

	returns arccos of x
*/
all_ktype acos(vector<all_ktype> args){

	all_ktype ret;

	//Verify args
	if (args.size() != 1 || args[0].type != 'd'){
		ret.s = "ERROR: Function 'abs()' requires a single double value as an argument.";
		ret.type = 'e';
		return ret;
	}

	//Create return variable
	ret.d = acos(args[0].d);
	ret.type = 'd';

	return ret;
}

/*
Function for interpreted function 'double atan(double x)'

double atan(double x):
	Calculates the arctan of x

	x - double whose arctan to calculate

	returns arctan of x
*/
all_ktype atan(vector<all_ktype> args){

	all_ktype ret;

	//Verify args
	if (args.size() != 1 || args[0].type != 'd'){
		ret.s = "ERROR: Function 'abs()' requires a single double value as an argument.";
		ret.type = 'e';
		return ret;
	}

	//Create return variable
	ret.d = atan(args[0].d);
	ret.type = 'd';

	return ret;
}

/*
Function for interpreted function 'double acsc(double x)'

double acsc(double x):
	Calculates the arccosecant of x

	x - double whose acsc to calculate

	returns acsc of x
*/
all_ktype acsc(vector<all_ktype> args){

	all_ktype ret;

	//Verify args
	if (args.size() != 1 || args[0].type != 'd'){
		ret.s = "ERROR: Function 'abs()' requires a single double value as an argument.";
		ret.type = 'e';
		return ret;
	}

	//Create return variable
	ret.d = asin(1/args[0].d);
	ret.type = 'd';

	return ret;
}

/*
Function for interpreted function 'double asec(double x)'

double asec(double x):
	Calculates the arcsecant of x

	x - double whose asec to calculate

	returns asec of x
*/
all_ktype asec(vector<all_ktype> args){

	all_ktype ret;

	//Verify args
	if (args.size() != 1 || args[0].type != 'd'){
		ret.s = "ERROR: Function 'abs()' requires a single double value as an argument.";
		ret.type = 'e';
		return ret;
	}

	//Create return variable
	ret.d = acos(1/args[0].d);
	ret.type = 'd';

	return ret;
}

/*
Function for interpreted function 'double acot(double x)'

double acot(double x):
	Calculates the arccotangent of x

	x - double whose acot to calculate

	returns acot of x
*/
all_ktype acot(vector<all_ktype> args){

	all_ktype ret;

	//Verify args
	if (args.size() != 1 || args[0].type != 'd'){
		ret.s = "ERROR: Function 'abs()' requires a single double value as an argument.";
		ret.type = 'e';
		return ret;
	}

	//Create return variable
	ret.d = atan(1/args[0].d);
	ret.type = 'd';

	return ret;
}

all_ktype transpose(std::vector<all_ktype> args){ //matrix abs(matrix x)
    
    all_ktype ret;
    
    //Verify args
    if (args.size() != 1 || args[0].type != 'm'){
        ret.s = "ERROR: Function 'transpose()' requires a single matrix value as an argument.";
        ret.type = 'e';
        return ret;
    }
    
    //Create return variable
    ret.km = args[0].km.transpose();
    ret.type = 'm';
    
    return ret;
}

all_ktype inverse(std::vector<all_ktype> args){ //matrix inverse(matrix x)

    all_ktype ret;
    
    //Verify args
    if (args.size() != 1 || args[0].type != 'm'){
        ret.s = "ERROR: Function 'inverse()' requires a single matrix value as an argument.";
        ret.type = 'e';
        return ret;
    }
    
    //Create return variable
    ret.km = args[0].km.inverse();
    ret.type = 'm';
    
    return ret;
}

all_ktype crossprd(std::vector<all_ktype> args){ //matrix crossprd(matrix a, matrix b);
    
    all_ktype ret;
    
    //Verify args
    if (args.size() != 2 || args[1].type != 'm' || args[1].type != 'm'){
        ret.s = "ERROR: Function 'crossprd()' requires two matrix values as arguments.";
        ret.type = 'e';
        return ret;
    }
    
    //Create return variable
    ret.km = args[0].km.crossprd(args[1].km);
    ret.type = 'm';
    
    return ret;
}

all_ktype dotprd(std::vector<all_ktype> args){ //double dotprd(matrix a, matrix b);
    
    all_ktype ret;
    
    //Verify args
    if (args.size() != 2 || args[0].type != 'm' || args[1].type != 'm'){
        ret.s = "ERROR: Function '()' requires two matrix values as arguments.";
        ret.type = 'e';
        return ret;
    }
    
    //Create return variable
    ret.d = args[0].km.dotprd(args[1].km);
    ret.type = 'd';
    
    return ret;
}

all_ktype conjugate(std::vector<all_ktype> args){ //matrix conjugate(matrix a);
    
    all_ktype ret;
    
    //Verify args
    if (args.size() != 1 || args[0].type != 'm'){
        ret.s = "ERROR: Function 'conjugate()' requires a single matrix value as an argument.";
        ret.type = 'e';
        return ret;
    }
    
    //Create return variable
    ret.km = args[0].km.conjugate();
    ret.type = 'm';
    
    return ret;
}

all_ktype adjoint(std::vector<all_ktype> args){ //matrix adjoint(matrix x);
    
    all_ktype ret;
    
    //Verify args
    if (args.size() != 1 || args[0].type != 'm'){
        ret.s = "ERROR: Function '()' requires a single matrix value as an argument.";
        ret.type = 'e';
        return ret;
    }
    
    //Create return variable
    ret.km = args[0].km.adjoint();
    ret.type = 'm';
    
    return ret;
}

all_ktype is_invertable(std::vector<all_ktype> args){ //bool is_invertable(matrix x);
    
    all_ktype ret;
    
    //Verify args
    if (args.size() != 1 || args[0].type != 'm'){
        ret.s = "ERROR: Function 'is_invertable()' requires a single matrix value as an argument.";
        ret.type = 'e';
        return ret;
    }
    
    //Create return variable
    ret.b = args[0].km.is_invertable();
    ret.type = 'b';
    
    return ret;
}

all_ktype determinant(std::vector<all_ktype> args){ //double determinant(matrix x);
    
    all_ktype ret;
    
    //Verify args
    if (args.size() != 1 || args[0].type != 'm'){
        ret.s = "ERROR: Function '()' requires a single matrix value as an argument.";
        ret.type = 'e';
        return ret;
    }
    
    //Create return variable
    ret.d = args[0].km.determinant();
    ret.type = 'd';
    
    return ret;
}

