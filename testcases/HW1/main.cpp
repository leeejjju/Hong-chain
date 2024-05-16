#include <stdio.h>
#include "submission.cpp" //solution of submission
#include "input.cpp" //make good input
#define PATH_SIZE 1024

//It runs for each fuzzing case,,, 
int main(){

	//TODO make good input using input generator
	int list[10];
	generate_input(list);

	//TODO implement your test driver using input from above
	//maybe need to use print function
	bubble_sort(list, 10); 
	print_state(list, 10);	

	return 0;
}




