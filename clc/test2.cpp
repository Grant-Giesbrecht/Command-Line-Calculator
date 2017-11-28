//#include <stdio.h>
//#include <string>
//
//#include "KInterp.hpp"
//#include "KVar.hpp"
//#include "KMatrix.hpp"
//#include "string_manip.hpp"
//#include "KIFunctions.hpp"
//
//using namespace std;
//
//int main(int argc, char const *argv[]) {
//
//
//	// //Test 'inject words'
//	// vector<string> vs;
//	// vs.push_back("zero");
//	// vs.push_back("one");
//	// vs.push_back("two");
//	// vs.push_back("three");
//	// vs.push_back("four");
//	// vs.push_back("five");
//	// vs.push_back("six");
//	// inject_words_into_strvec(vs, "Hello, my name is satan", 3, 4);
//	// for (int i = 0 ; i  < vs.size() ; i++){
//	// 	cout << '>' << vs[i] << '<' << endl;
//	// }
//
//	KVar kv;
//	KMatrix km("1, 2; 3, 4");
//	kv.add_double("double1", 3.1515926535);
//	kv.add_matrix("m1", km);
//	kv.add_string("s1", "Hello");
//	kv.add_bool("b1", true);
//	all_ktype akt;
//	akt.type;
//
//	vector<func_id> functions;
//	define_functions(&functions);
//
//	if (!interpret(" { 5 - (5 - 2) } ", kv, akt, functions)){
//		cout << "Returned type: " << akt.type << endl;
//		cout << "Returned string: " << akt.s << endl;
//	}
//
//	return 0;
//}
