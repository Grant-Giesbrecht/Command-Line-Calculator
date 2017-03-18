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
