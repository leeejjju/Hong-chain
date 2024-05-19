#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include "db_stack.h"


// if input doesn't meet condition return 1, otherwise return 0
// check if the input meets condition
int input_ok(char * ch) {
    if (!isdigit((unsigned char)*ch)) {
        return 1; // Return false if character is not digit
    }
    return 0;
}

// if the struct isn't correct return 1, otherwise return 0
// check if the struct is correct
// mode: push(0)/pop(1), before_size: before size of execute mode, after_size: after size of execute mode
int rep_ok(int before_size, int after_size, int mode) {
    if (mode == 0) {    // push order
        if ((before_size + 1) != after_size) {
            return 1;
        }
    }
    else {      // pop order
        if (before_size == 0 && after_size == 0) {
            return 0;
        }
        if ((before_size - 1) != after_size) {
            return 1;
        }
    }
    
    return 0;
}

// record the function flow
void write_log(int fd, char * log_buf) {

    size_t log_len = strlen(log_buf);
    ssize_t written = 0, written_acc = 0;

    while (written_acc != log_len) {
        written = write(fd, log_buf + written_acc, log_len - written_acc);
        written_acc += written;
    }
}

// print function checks the status after the command is executed
void print_stack(Node * p) {
    printf("-------------- PRINT STACK ----------------\n");
    while (p != nullptr) {
        printf("%c  ", p->data);
        p = p->next;
    }
   printf("\n-------------- PRINT STACK ----------------\n\n\n");
}


int main(int argc, char * argv[]) {

    Node * stack = nullptr;

    char buf[32];
    scanf("%31s", buf);
    buf[31] = '\0';

    printf("len: %ld\n", strlen(buf));

    int before_size;    // for rep_ok
    char log_file[128]; // for log
    char log_buf[512];  // for log
    int fd;             // for log

    /* for log */   
    pid_t pid = getpid();
    sprintf(log_file, "./.log/log_%d", pid);
    fd = open(log_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    /* for log */

    for (char * ch = buf; *ch != 0x0; ch++) {
        switch (*ch) {
            case 'a':
                // printf("ch: %c\n", *ch);
                if (*(ch+1) != 0x0) {
                    ch++;
                }

                // check input is correct
                if (input_ok(ch) == 1) {
                    fprintf(stderr, "push input_ok failed\n");
                    remove(log_file);
                    close (fd);
                    exit(EXIT_FAILURE);
                }

                before_size = stack_size(stack);
                stack = stack_push(stack, *ch);

                // record log
                sprintf(log_buf, "stack Pushed\n");
                write_log(fd, log_buf);
                
                // check struct 
                if ((rep_ok(before_size, stack_size(stack), 0)) == 1) {
                    fprintf(stderr, "push rep_ok failed\n");
                    remove(log_file);
                    close (fd);
                    exit(EXIT_FAILURE);
                }

                print_stack(stack);
                break;

            case 'b':
                before_size = stack_size(stack);
                stack = stack_pop(stack);

                // record log
                sprintf(log_buf, "Popping from stack\n");
                write_log(fd, log_buf);


                // check struct 
                if ((rep_ok(before_size, stack_size(stack), 1)) == 1) {
                    fprintf(stderr, "pop rep_ok failed\n");
                    remove(log_file);
                    close (fd);
                    exit(EXIT_FAILURE);
                }

                print_stack(stack);
                break;

            default:
                break;
        }
    }   

    if (!stack_empty(stack)) {
        printf("top node data: %c\n", stack_top(stack)->data);
    }

    stack_clear(stack);


    close (fd);
}