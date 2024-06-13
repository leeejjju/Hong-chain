#include <stdio.h> 
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <fcntl.h>
#include <limits.h>
#include <libgen.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <iostream>
using namespace std;

#include "lib_piece/issue/issue.cpp"
#include "report.cpp"
#include "lib_piece/compare/compare.cpp"
#define BUF_SIZE 1024
#define NEW_PORT_BASE 5000
#define ERROR_NUM 10 
#define INFINITIROOP 9 
// #define DEBUG

void *handle_clnt(void * arg);
int register_routine(string repo_owner, string repo_name);
int submission_routine(int s_id, string repo_owner, string repo_name);
int finish_routine(string repo_owner, string repo_name);

char keywords[ERROR_NUM][32] = {"heap-use-after-free", "heap-buffer-overflow", "stack-buffer-overflow", "global-buffer-overflow", "stack-use-after-return", "stack-use-after-scope", "initialization-order-fiasco", "memory leaks","SEGV","INFINITIROOP"}; //from https://github.com/google/sanitizers/wiki/AddressSanitizer
int seed_cnt;

int main(int argc, char *argv[]) {

	//signal setting
    signal(SIGALRM, timeout_handler);

	//check args first
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}else printf("Start with port %s", argv[0]);

	//setting for socket programming
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	int serv_sock, clnt_sock;
	pthread_t t_id;
	serv_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET; 
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_adr.sin_port = htons(atoi(argv[1])); //port from argv
	int enable = 1;
    if (setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
        perror("setsockopt(SO_REUSEADDR) failed");
        close(serv_sock);
        exit(EXIT_FAILURE);
    }
	if (bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr)) == -1) perror("bind");
	if (listen(serv_sock, 5) == -1) perror("listen");

	while (1) {
		//accpet new client and make new thread for hadle it
		clnt_adr_sz = sizeof(clnt_adr);
		clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, (socklen_t*)&clnt_adr_sz);
		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock);
		pthread_detach(t_id);
		
		#ifdef DEBUG
		printf("Connected client IP: %s [%d]\n", inet_ntoa(clnt_adr.sin_addr), atoi(argv[1]));
    	#endif
	}

	close(serv_sock);
	return 0;

}


//handle one student connection from github action.
void *handle_clnt(void * arg) {


	int clnt_sock = *((int*)arg);
	int len;

	//get repository information
	int repo_len;
	char buffer[BUF_SIZE];
	if((len = read(clnt_sock, &repo_len, sizeof(int))) <= 0) perror("read");
	else printf("sock: %d | repo_len: %d\n", clnt_sock, repo_len);
	if((len = read(clnt_sock, buffer, repo_len)) <= 0) perror("read");
	char* tmp = buffer;
	string repo_name = basename(buffer);
	string repo_owner = dirname(tmp);

	//get student_id 
	int s_id;
	if((len = read(clnt_sock, &s_id, sizeof(int))) <= 0) perror("read");
	else printf("sock: %d | student id: %d\n", clnt_sock, s_id);
	cout << "sock: " << clnt_sock << " | repo: " << repo_owner << "'s " << repo_name << endl;

	switch(s_id){
	case 0:
		printf("REGISTER\n");
		//make subdirectory for seeds
		system("mkdir -p seeds");

		//recv test_driver.cpp
		if(recv_file(clnt_sock, s_id)) perror("recv_file");
		//recv main_driver.cpp
		if(recv_file(clnt_sock, s_id)) perror("recv_file");
		//recv solution.cpp
		if(recv_file(clnt_sock, s_id)) perror("recv_file");
		//recv seeds
		if((len = read(clnt_sock, &seed_cnt, sizeof(int))) <= 0) perror("read");
		for(int i = 0; i < seed_cnt; i++){
			if(recv_file(clnt_sock, s_id)) perror("recv_file");
		}

		register_routine(repo_owner, repo_name);
		break;

	case -1:
		printf("FINISH\n");

		finish_routine(repo_owner, repo_name);
		break;

	default:
		printf("SUBMISSION: %d\n", s_id);
		//make subdirectory under submissions/
		char cmd[1024];
		sprintf(cmd, "mkdir -p submissions/%d", s_id);
		system(cmd);

		//recv submission.cpp
		if(recv_file(clnt_sock, s_id)) perror("recv_file");

		submission_routine(s_id, repo_owner, repo_name);
		break;

	}	

	close(clnt_sock);
	return NULL;

}

int register_routine(string repo_owner, string repo_name){


	//build 
	//TODO 

	//fuzzing

	//write report
	string title = "REGISTER report";
	string report = register_issue();

	//make issue
	string github_token;
	cout << "token: ";
	cin >> github_token;
	create_github_issue(title, report, repo_owner, repo_name, github_token);

	return 0;

}

int submission_routine(int s_id, string repo_owner, string repo_name){
	ssize_t len;
	char cmd[1024];
	char submission_exec_path[1024];
	char submission_corpus_path[1024];
    int total_cnt = 0;
    int crash_cnt = 0;
    int incorrect_cnt = 0;
    int check_crash[ERROR_NUM] = {0};
    
	//build
	sprintf(cmd, "make submission SID=%d",s_id );
	if (system(cmd)) {
		perror("system");
		return 1; //on failure return 1
	}

	//fuzz
	sprintf(cmd, "make fz_submission SID=%d",s_id );
	if (system(cmd)){
		perror("system");
		return 1; //on failure return 1
	}


	/* TODO : CRITICAL SECTION */
	setenv("ASAN_OPTIONS", "abort_on_error=1", 1);
	
	sprintf(submission_exec_path, "submissions/%d/submission.out",s_id );
	//solution queue
	exec_input("./solution.out", submission_exec_path,"outputs/default/queue", &total_cnt, &crash_cnt, &incorrect_cnt, s_id, check_crash);

	//submission queue
	sprintf(submission_corpus_path, "submissions/%d/outputs/default/queue",s_id );
	exec_input("./solution.out", submission_exec_path, submission_corpus_path, &total_cnt, &crash_cnt, &incorrect_cnt, s_id, check_crash);

	//submission crash
	sprintf(submission_corpus_path, "submissions/%d/outputs/default/crash",s_id );
	exec_input("./solution.out", submission_exec_path, submission_corpus_path, &total_cnt, &crash_cnt, &incorrect_cnt, s_id, check_crash);

    printf("totall: %d crash: %d incorrect: %d\n",total_cnt,crash_cnt,incorrect_cnt);
	setenv("ASAN_OPTIONS", "abort_on_error=0", 1);
	/***************************/

	//write report
	string title = "SUBMIT report";
	string report = create_report(s_id, total_cnt, crash_cnt, incorrect_cnt);

	//make issue
	string github_token;
	cout << "token: ";
	cin >> github_token;
	create_github_issue(title, report, repo_owner, repo_name, github_token);

	return 0;
}

int finish_routine(string repo_owner, string repo_name){

	//arrange the reports of submissions
	//TODO

	//write report
	string title = "FINISH report";
	string report = finish();

	//make issue
	string github_token;
	cout << "token: ";
	cin >> github_token;
	create_github_issue(title, report, repo_owner, repo_name, github_token);
	
	return 0;

}

