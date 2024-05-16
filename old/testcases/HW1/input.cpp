#include <stdio.h>
#include <iostream>
#include <string.h>
#define INPUT_SIZE 10
#define BUF_SIZE 2048


void print_state(int* list, int n){

	for(int i = 0; i < n; i++) cout << list[i] << " ";
	cout << "\n";
	return;

}

//return -1 if not num
int to_num(char* tok){

	if((strlen(tok) == 1) && (tok[0] == '0')) return 0; //0 case
	if(atoi(tok) != 0) return atoi(tok);
	return -1;

}

//save valid inputs in list
void generate_input(int* list){

	int cnt = 0;
	char tok[BUF_SIZE];

	while(cnt < INPUT_SIZE){
		cin >> tok;
		if(to_num(tok) != -1){
			list[cnt] = to_num(tok);
			cnt++;
		}
	}
	return;

}

