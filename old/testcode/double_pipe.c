/*

	This program reads an integer expression from the user and computes the result using a child process.
	The parent and child processes communicate with each other through PIPES.
	Implement this program following the instructions.
	Append "[parent]" and "[child]" to all messages in Parent() and Child() respectively.

	This program does not terminate cleanly because of bugs.
	Find and fix the bugs so that the program terminates as the following example.

	This problem can be easy if you clearly understand the behavior of pipes!
	Otherwise, it can be challenging.

	Example)
		pipes.p2c = (3 4)
		pipes.c2p = (5 6)
		[parent] Input an integer binary expressions(type "quit" to finish.): 5 + 3
		[parent] 5 + 3 = 8
		[parent] Input an integer binary expressions(type "quit" to finish.): 7 * 2
		[parent] 7 * 2 = 14
		[parent] Input an integer binary expressions(type "quit" to finish.): 9/2
		[parent] 9/2 = 4
		[parent] Input an integer binary expressions(type "quit" to finish.): quit
		[parent] Terminating Parent.
		[child] pipes->p2c[0] was broken, read_size = 0
		[child] The child is doing a very important job!
		[child] Done.
		[child] Terminating Child.
		Bye!

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <wait.h>
#include <sys/types.h>

#define _DEBUG

#define TRUE 1
#define FALSE 0

#define READ_END 0
#define WRITE_END 1

#define MAX_LEN 128

typedef struct {
	int p2c[2];
	int c2p[2];
} DoublePipe;


void* Parent(void *arg);
void* Child(void *arg);
int Evaluate(char *expression);

int main()
{
	DoublePipe pipes;
	if(pipe(pipes.p2c) == -1 || pipe(pipes.c2p) == -1){
		printf("Failed to create pipe!\n");
		return -1;
	}
	printf("pipes.p2c = (%d %d)\n", pipes.p2c[0], pipes.p2c[1]);
	printf("pipes.c2p = (%d %d)\n", pipes.c2p[0], pipes.c2p[1]);

	pid_t child_pid = fork();
	switch(child_pid){
	case -1:
		printf("Failed to create a child process!\n");
		break;

	case 0:			// child
		Child(&pipes);
		exit(0);	// for safety
		break;

	default:		// parent
		Parent(&pipes);
		wait(NULL);	// DO NOT MODIFY THIS LINE
		break;
	};

	printf("Bye!\n");				// this message MUST be displayed.

	return 0;
}

void* Parent(void *arg)
{
	DoublePipe *pipes = (DoublePipe *)arg;

	char expression[MAX_LEN] = "";
	char result[MAX_LEN] = "";

	close(pipes->p2c[READ_END]);
	close(pipes->c2p[WRITE_END]);
	
	while(1){
		printf("[parent] Input an integer binary expressions(type \"quit\" to finish.): ");
		fgets(expression, MAX_LEN, stdin);
		expression[strlen(expression) - 1] = 0;

		if(expression[0] != 0){
			if(strcmp(expression, "quit") == 0)		// DO NOT MODIFY THIS LINE!
				break;

			int written_size = write(pipes->p2c[WRITE_END], expression, strlen(expression) + 1);
			if(written_size <= 0){
				printf("[parent] Failed to write into pipe. written_size = %d\n", written_size);
				break;
			}

			int read_size = read(pipes->c2p[READ_END], result, MAX_LEN);
			if(read_size <= 0){
				printf("[parent] Failed to read from pipe. read_size = %d\n", read_size);
				break;
			}

			printf("[parent] %s = %s\n", expression, result);
		}
	}

	close(pipes->p2c[WRITE_END]);
	close(pipes->c2p[READ_END]);

	printf("[parent] Terminating Parent.\n");	// this message MUST be displayed.

	return NULL;
}

void* Child(void *arg)
{
	DoublePipe *pipes = (DoublePipe *)arg;

	char expression[MAX_LEN] = "";
	char result[MAX_LEN] = "";

	close(pipes->p2c[WRITE_END]);
	close(pipes->c2p[READ_END]);
	
	while(1){
		// if the pipe is broken, read() returns zero.
		int read_size = read(pipes->p2c[READ_END], expression, MAX_LEN);
		if(read_size <= 0){			// [IMPORTANT] Child should terminate by this condition.
			printf("[child] pipes->p2c[0] was broken, read_size = %d\n", read_size);
			break;
		}

		int v = Evaluate(expression);
		sprintf(result, "%d", v);
		write(pipes->c2p[WRITE_END], result, strlen(result) + 1);
	}

	close(pipes->p2c[READ_END]);
	close(pipes->c2p[WRITE_END]);
	
	printf("[child] The child is doing a very important job!\n");	// this message MUST be displayed.
	sleep(1);
	printf("[child] Done.\n");

	printf("[child] Terminating Child.\n");	// this message MUST be displayed.

	return NULL;
}

int Evaluate(char *expression)
{
	int op1 = 0, op2 = 0;
	char op = 0;
	int result = 0;

	int ret = sscanf(expression, "%d %c %d", &op1, &op, &op2);
	if(ret == 3){
		switch(op){
		case '+':
			result = op1 + op2;
			break;

		case '-':
			result = op1 - op2;
			break;

		case '*':
			result = op1 * op2;
			break;

		case '/':
			if(op2 != 0)
				result = op1 / op2;
			break;
		}
	}

	return result;
}

