#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>


int crash_arr[359];


// if it was succeed return 0, otherwise 1
int scanning(char * crash_filename) {
    FILE * crash_file;
    char * line = NULL;
    char * num_pos;
    size_t len = 0;
    ssize_t read;
    int a = 0;

    // save the info of executed line
    // 0 is not executed, 1 is executed
    int temp_crash_arr[359] = {0};

    // file open
    crash_file = fopen(crash_filename, "r");
    // get line and check the line which is excuted
    while ((read = getline(&line, &len, crash_file)) != -1) {
        // if there is no line number, then continue
        if ((num_pos = strstr(line, "btree.c:")) == NULL) {
            continue;
        }
        // point num_pos to line number (Ex.319)
        num_pos += 8;
        // printf("line1: %s\n", num_pos);

        // tokenize in case (329:5) -> (329)
        strtok(num_pos, ":");
        // printf("line2: %s\n", num_pos);

        // check the num_pos is executed and save the info
        if (temp_crash_arr[atoi(num_pos)] == 0) {
            temp_crash_arr[atoi(num_pos)] = 1;
        }
        else {
            continue;
        }
    }

    // apply(add) the temp_crash_arr to crash_arr(global array)
    for (int i = 1; i < sizeof(crash_arr) / sizeof(int); i++) {
        crash_arr[i] += temp_crash_arr[i];
    }

    return 0;
}

int main(int argc, char * argv[]) {

    // open crash_coverage directory
    DIR * dir = opendir("gcov/crash");
    struct dirent * crash_dirent;
    char crash_filename[256];

    // read all files in crash_coverage directory
    while ((crash_dirent = readdir(dir)) != NULL) {
        if (crash_dirent->d_type == DT_REG) {
            strcpy(crash_filename, "gcov/crash/");
            strcat(crash_filename, crash_dirent->d_name);
            printf("file: %s\n", crash_filename);

            if (scanning(crash_filename) == 1) {
                fprintf(stderr, "scanning error\n");
            }
        }
    }

    // show
    for (int i = 1; i < sizeof(crash_arr) / sizeof(int); i++) {
        if (crash_arr[i] > 0) {
            printf("%d: %d\n", i, crash_arr[i]);
        } else {
            printf("%d: not executed\n", i);
        }
    }

}