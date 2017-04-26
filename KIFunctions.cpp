#include "KIFunctions.hpp"

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

