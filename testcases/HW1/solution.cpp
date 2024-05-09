#include <stdio.h>
#include <iostream>
using namespace std;


/*
	this src code is for solution or submission of assignments.
	define functions to implement your assignments on here.
	the functions defined here will be using in test driver(main.c)
*/
void swap(int* a, int* b){
	int temp;
	
	temp = *a;
	*a = *b;
	*b = temp;
}


//bubble sort
void bubble_sort(int list[], int num){
	int i, j;
	int temp;
	
	for(i = 0; i < num; i++){ 
		for(j = 0; j < num-i; j++){ //max?? ????????  
			if(j+1 < num-i && list[j] > list[j+1]){
				swap(&list[j], &list[j+1]);
			}
		}
	}
	return;	
}


