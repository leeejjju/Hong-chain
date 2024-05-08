#include <stdio.h>
#include "function.cpp" //solution of submission
#include "input.cpp" //make good input
#define BUF_SIZE 4096
#define PATH_SIZE 1024
//define input/output dirs like this, or using method like DINPUT_DIR=inputs... on compile time to flexability
#define INPUT_DIR inputs
#define OUTPUT_DIR outputs


//It runs for each fuzzing case,,, 
int main(int argc, char** argv){

	//get inputs from stdin (maybe from fuzzer)
	char buffer[BUF_SIZE];
	fgets(buffer, BUF_SIZE, stdin);

	//make good input
	char* inputs = generate_good_input(buffer);

	// ***************** this area is fixed as first part *****************
	//save inputs into file
	char input_path[PATH_SIZE] = INPUT_DUR + "/";
	input_path += /*filename*/; 
	FILE* fp_in = fopen(input_path, "w");
	fwrite(fp, ... );

	// ********************************************************************


	//TODO implement your test driver using input from above
	//maybe need to use print function
	int a = 10;
	printf("result: %d\n", func(a));






	// ***************** this area is fixed as last part *****************
	//save outputs into file

	// ********************************************************************
	return 0;
}




