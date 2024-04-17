#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

#define BUF_SIZE 512
#define QCOV_DIR "gcov/queue"
#define CCOV_DIR "gcov/crash"
#define SRC_FILE "btree.c"



int * queue_arr; //queue's execution count array - index is linenum
int * crash_arr; //crash's execution count array - index is linenum
int max_linenum;
int got_size = 0; //flag : did we got maximum line number? 
int queue_sum = 0; //sum of executed lines of queue arr

int total_fail = 0, total_pass = 0;     // the number of failing/passing execution


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


//read queue's gcov directory and parse them, add datas on queue_arr
//return 0 on success, 1 on failure
int get_queue_data(){
	
	DIR* queue_dir;
	if((queue_dir = opendir(QCOV_DIR)) == NULL) return 1;

	struct dirent* file;
	char filename[BUF_SIZE];

	//scan all the files in directory
	while(file = readdir(queue_dir)){
		if(file->d_type != DT_REG) continue; //ignore entry not file
		
		//on reqular files...
		sprintf(filename, "%s/%s", QCOV_DIR, file->d_name);
		if(D) printf("file: %s\n", filename);
		if(got_size == 0){ //firstly once, estimate max linenum and malloc arrys
			max_linenum = get_last_linenum(filename)+1; //get maximum line number
			queue_arr = (int*)malloc(sizeof(int)*max_linenum); //allocate memory
			got_size = 1;
		}
		if(parse_gcov(filename , queue_arr)) {
			perror("parse_gcov");
			return 1;
		}

        total_pass++;   // increase the number of total passing execution
	}

	return 0;

}


// if it was succeed return 0, otherwise 1
// open file and scanning the line and save the info to temp_crash_arr/crash_arr
int crash_scanning(char * crash_filename) {
    FILE * crash_file;
    char * line = NULL;
    char * num_pos;
    size_t len = 0;
    ssize_t read;
    int a = 0;

    // save the info of executed line
    // 0 is not executed, 1 is executed
    int * temp_crash_arr = (int*)malloc(sizeof(int) * max_linenum); //allocate memory
    memset(temp_crash_arr, 0, sizeof(int) * max_linenum);   // initialize the memory
    

    // file open
    crash_file = fopen(crash_filename, "r");
    // get line and check the line which is excuted
    while ((read = getline(&line, &len, crash_file)) != -1) {
        // if there is no line number, then continue
        if ((num_pos = strstr(line, SRC_FILE)) == NULL) {
            continue;
        }
        // point num_pos to line number (Ex.319)
        num_pos += strlen(SRC_FILE) + 1;

        // tokenize in case (329:5) -> (329)
        strtok(num_pos, ":");

        // check the num_pos is executed and save the info
        if (temp_crash_arr[atoi(num_pos)] == 0) {
            temp_crash_arr[atoi(num_pos)] = 1;
        }
        else {
            continue;
        }
    }

    // apply(add) the temp_crash_arr to crash_arr(global array)
    for (int i = 1; i < max_linenum; i++) {
        crash_arr[i] += temp_crash_arr[i];
    }

    free (temp_crash_arr);

    return 0;
}


// if it was succeed return 0, otherwise 1
// open crash_coverage directory and scanning
int get_crash_data() {
    // open crash_coverage directory
    DIR * dir = opendir(CCOV_DIR);
    struct dirent * crash_dirent;
    char crash_filename[BUF_SIZE];

    crash_arr = (int*)malloc(sizeof(int) * max_linenum); //allocate memory
    memset(crash_arr, 0, sizeof(int) * max_linenum);   // initialize the memory

    // read all files in crash_coverage directory
    while ((crash_dirent = readdir(dir)) != NULL) {
        if (crash_dirent->d_type == DT_REG) {
            sprintf(crash_filename, "%s/%s", CCOV_DIR, crash_dirent->d_name);
            // printf("file: %s\n", crash_filename);

            if (crash_scanning(crash_filename) == 1) {
                fprintf(stderr, "crash scanning error\n");
                return 1;
            }
            
            total_fail++;   // increase the number of total failing execution
        }
    }
    return 0;
}


// determines whether the line ranks in the line_ranking
void cmp(int line, float score, int * line_ranking, float * score_ranking) {
    for (int i = 0; i < 10; i++) {
        if (score >= score_ranking[i]) {
            for (int k = 9; k >= i; k--) {
                if (k != 9) {
                    score_ranking[k+1] = score_ranking[k];
                    line_ranking[k+1] = line_ranking[k];
                }
            }
            score_ranking[i] = score;
            line_ranking[i] = line;
            return ;
        } else {
            continue;
        }
    }
}


// if it was succeed return 0, otherwise 1
// calculate the suspicious score of each line and ranking them(1-10)
void susp() {
    int i;
    float susp_score;
    int line_ranking[10] = {0};
    float score_ranking[10] = {0};

    // calculate the susp_score of line i
    for (i = 1; i < max_linenum; i++) {
        if (crash_arr[i] != 0 && queue_arr[i] != 0) {
            susp_score = (crash_arr[i] / total_fail) / ((crash_arr[i] / total_fail) + (queue_arr[i] / total_pass));
        } else if (crash_arr[i] != 0) {
            susp_score = 1;
        }
        else {
            susp_score = 0;
        }
        cmp(i, susp_score, line_ranking, score_ranking);
    }

    // print ranking table
    printf("line    susp_score\n");
    printf("--------------------\n");
    for (i = 0; i < 10; i++) {
        printf("%d      %.1f\n", line_ranking[i], score_ranking[i]);
    }
}




int main(int argc, char * argv[]) {

    /* queue */ 
    if(get_queue_data()) perror("get_queue_data");

    // //print the queue_arr state
	// for(int i = 0; i < max_linenum; i++) {
	// 	if(queue_arr[i]) printf("queue arry [%03d] : %d\n", i, queue_arr[i]);
	// 	queue_sum += queue_arr[i];
	// }
	// printf("queue_sum: %d\n", queue_sum);


    /* crash */
    if (get_crash_data() == 1) {
        perror("get_crash_data");
    }
    
    // // print the crash_arr state
    // for (int i = 1; i < max_linenum; i++) {
    //     if (crash_arr[i] > 0) {
    //         printf("%d: %d\n", i, crash_arr[i]);
    //     } else {
    //         printf("%d: not executed\n", i);
    //     }
    // }


    susp();



    free (queue_arr);
    free (crash_arr);

    return 0;

}