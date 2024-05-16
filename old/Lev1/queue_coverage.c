#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <dirent.h>
#define BUF_SIZE 512
#define COV_DIR "gcov/queue"


int* queue_arry; //queue's execution count array - index is linenum
int max_linenum;
int got_size = 0; //flag : did we got maximum line number? 
int queue_sum = 0; //sum of executed lines of queue arry


//get the total line numbers of gcov file
//return the number, return -1 on failure
int get_last_linenum(char* filename){

	//open file
	FILE* fp;
	if((fp = fopen(filename, "r")) == NULL) return 1;

	char* line = NULL;
	ssize_t size;
	while(getline(&line, &size, fp) > 0); //for get the last line
	
	char* tok = strtok(line, ":");
	tok = strtok(NULL, ";"); //get the line number

	return atoi(tok);

}


//parse the gcov file and save datas into array
//return 0 on success, 1 on failure
int parse_gcov(char* filename, int* arry){

	//open file
	FILE* fp;
	if((fp = fopen(filename, "r")) == NULL) return 1;

	//loop per line
	ssize_t len = 0, size = 0;
	char* line = NULL;
	int count;
	while((len = getline(&line, &size, fp)) > 0){
		if(ferror(fp)) return 1;
		char* tok = strtok(line, ":");
		if(count = atoi(tok)){ //if executed(count is non-zero, ='-' | ='###'
			tok = strtok(NULL, ":"); //get the line number and
			arry[atoi(tok)]++; //add it
			//TODO if we want... arry[atoi(tok)] += count;
			if(D) printf("line #%-3d : executed in %d times\n", atoi(tok), count);
		}
	}

	//release resources
	free(line);
	fclose(fp);

	return 0;
}


//read queue's gcov directory and parse them, add datas on queue_arry
//return 0 on success, 1 on failure
int get_queue_data(){
	
	DIR* queue_dir;
	if((queue_dir = opendir(COV_DIR)) == NULL) return 1;

	struct dirent* file;
	char filename[BUF_SIZE];

	//scan all the files in directory
	while(file = readdir(queue_dir)){
		if(file->d_type != DT_REG) continue; //ignore entry not file
		
		//on reqular files...
		sprintf(filename, "%s/%s", COV_DIR, file->d_name);
		if(D) printf("file: %s\n", filename);
		if(got_size == 0){ //firstly once, estimate max linenum and malloc arrys
			max_linenum = get_last_linenum(filename)+1; //get maximum line number
			queue_arry = (int*)malloc(sizeof(int)*max_linenum); //allocate memory
			got_size = 1;
		}
		if(parse_gcov(filename , queue_arry)) {
			perror("parse_gcov");
			return 1;
		}
	}

	return 0;

}

int main(int argc, char** argv){


	if(get_queue_data()) perror("get_queue_data");
	
	
	//print the queue_arry state
	for(int i = 0; i < max_linenum; i++) {
		if(queue_arry[i]) printf("queue arry [%03d] : %d\n", i, queue_arry[i]);
		queue_sum += queue_arry[i];
	}
	printf("queue_sum: %d\n", queue_sum);

	return 0;
}







