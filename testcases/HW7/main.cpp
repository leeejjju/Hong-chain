#include <stdio.h>
#include "submission.cpp" //solution of submission
#include "input.cpp" //make good input
#define PATH_SIZE 1024

//It runs for each fuzzing case,,, 
int main(){

	//TODO make good input using input generator

	my_queue  a; //일단 리니어...의 리스트 
    element  tmp; //임시로 담을 원소
	int cmd = 1;
	string s_title;
	int npages, price;

	//TODO implement your test driver using input from above
	//maybe need to use print function
	while(cmd){

		cout << "1: insert\n2: delete\n0:quit\ncmd -> ";
		cin >> cmd;

		switch(cmd){
		case 1:
			cin >> s_title;
			cin >> npages;
			cin >> price;
        	tmp.set_data("C++ Programming", 534, 35000);
    		a.insert_q(tmp);
			print_state(a);
			break;
		case 2:
    		a.delete_q();
			print_state(a);
			break;
		case 0:
			break;
		default:
			cout << "invalid cmd\n";
		}

	}

	return 0;


}




