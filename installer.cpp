#include <iostream>
#include <stdio.h>
#include <string>
#include <dirent.h>
#include <sys/types.h>
#include "configuration.hpp"
#include <sys/stat.h>
#include <cstring>

//CXHAIL COMMAND g++ -o inst installer.cpp && ./inst

using namespace std;

int install();
int uninstall();

int main(int argc, char** argv){

	string command;

	string rsc_dir(RESOURCE_DIR);
	string silence_cmd = "1> out.out 2> error.out";

	if (strcmp(argv[argc-1], "UNINSTALL") == 0 || strcmp(argv[argc-1], "-UNINSTALL") == 0 || strcmp(argv[argc-1], "--UNINSTALL") == 0){
		return uninstall();
	}else{
		cout << string(argv[argc-1]) << endl;
		return install();
	}

}

int uninstall(){

	//----------------------------------------------------------------------------------
	//------------------ Delete /usr/local/resources/CLC_3V0/Resources directory

	system("sudo rm /usr/local/resources/CLC_3V0/Resources/* 1> out.out 2> error.outfile");
	system("sudo rmdir /usr/local/resources/CLC_3V0/Resources/ 1> out.out 2> error.outfile");
	system("sudo rmdir /usr/local/resources/CLC_3V0/ 1> out.out 2> error.outfile");
	system("sudo rmdir /usr/local/resources/ 1> out.out 2> error.outfile");

	//----------------------------------------------------------------------------------
	//------------------ Delete executable

	system("sudo rm /usr/bin/clc 1> out.out 2> error.outfile");

	system("rm *.outfile");

	cout << "Uninstall of CLC complete" << endl;

	return 0;
}

int install(){

	//----------------------------------------------------------------------------------
	//------------------ Create /usr/local/resources/CLC_3V0/Resources directory

	cout << "Creating resource directory (Enter password if prompted, ^c to cancel)" << endl;

	system(string( "sudo mkdir /usr/ 1> out.out 2> error.outfile" ).c_str()); //system(string(  ).c_str());
	system(string( "sudo mkdir /usr/local/ 1> out.out 2> error.outfile" ).c_str());
	system(string( "sudo mkdir /usr/local/resources/ 1> out.out 2> error.outfile" ).c_str());
	system(string( "sudo mkdir /usr/local/resources/CLC_3V0/ 1> out.out 2> error.outfile" ).c_str());
	system(string( "sudo mkdir /usr/local/resources/CLC_3V0/Resources/ 1> out.out 2> error.outfile" ).c_str());

	//Verify success
	if (opendir(string(string(RESOURCE_DIR) + "Resources/").c_str()) == NULL){
		cout << "ERROR: Failed to create resource dir. May need to run as superuser (sudo)." << endl;
		return -1;
	}else{
		cout << "Successfully created resource directory:\n\t" + string(RESOURCE_DIR) + "test/Resources/" << endl;
	}

	//----------------------------------------------------------------------------------
	//------------------ Transfer resource files to resource directory

	system("sudo cp ./Resources/* /usr/local/resources/CLC_3V0/Resources/"); //" 1> out.out 2> error.outfile");
	cout << "All files transfered to resource directory" << endl;

	//----------------------------------------------------------------------------------
	//------------------ Compile CLC

	if (system("make") == 0){
		cout << "CLC successfully compiled" << endl;
	}else{
		cout << "ERROR: Failed to compile CLC" << endl;
		return -1;
	}

	//----------------------------------------------------------------------------------
	//------------------ Move executable to /usr/local/bin

	if (system("sudo mv clc /usr/local/bin/clc 1> out.out 2> error.outfile") != 0){
		cout << "ERROR: Failed to move executable to /usr/local/bin" << endl;
	}else{
		cout << "Executable successfully transfered to /usr/local/bin\n\n\tINSTALLATION COMPLETE" << endl;
	}

	//----------------------------------------------------------------------------------
	//------------------ Set permissions so files are editable by all

	system("sudo chmod u=rwx,g=rw,o=rw /usr/local/resources/CLC_3V0/Resources/*");

	system("rm *.outfile");

	return 0;

}
