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

#include "lib/issue/issue.cpp"
#include "lib/util/util.cpp"

#define BUF_SIZE 1024
#define NEW_PORT_BASE 5000

// #define DEBUG

void *handle_clnt(void * arg);
int recv_file(int sock, int s_id);

void register_routine(string repo_owner, string repo_name);
void submission_routine(int s_id, string repo_owner, string repo_name);
void finish_routine(string repo_owner, string repo_name);

int main(int argc, char *argv[]) {

	//check args first
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

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

	//get student_id first
	int s_id;
	if((len = read(clnt_sock, &s_id, sizeof(int))) <= 0) perror("read");
	else printf("sock: %d | student id: %d\n", clnt_sock, s_id);

	int repo_len;
	char buffer[BUF_SIZE];
	if((len = read(clnt_sock, &repo_len, sizeof(int))) <= 0) perror("read");
	else printf("sock: %d | repo_len: %d\n", clnt_sock, repo_len);
	if((len = read(clnt_sock, buffer, repo_len)) <= 0) perror("read");
	char* tmp = buffer;
	string repo_name = basename(buffer);
	string repo_owner = dirname(tmp);
	cout << "sock: " << clnt_sock << " | repo: " << repo_owner << "'s " << repo_name << endl;

	switch(s_id){

	case 0:
		printf("REGISTER\n");

		//recv test_driver.cpp
		if(recv_file(clnt_sock, s_id)) perror("recv_file");
		//recv solution.cpp
		if(recv_file(clnt_sock, s_id)) perror("recv_file");

		register_routine(repo_owner, repo_name);
		close(clnt_sock);
		return NULL;

	case -1:
		printf("FINISH\n");
		finish_routine(repo_owner, repo_name);
		close(clnt_sock);
		return NULL;

	default:

		printf("SUBMISSION: %d\n", s_id);
		//make subdirectory under submissions/
		char cmd[1024] = "mkdir -p submissions";
		system(cmd);
		sprintf(cmd, "mkdir -p submissions/%d", s_id);
		system(cmd);

		//recv submission.cpp
		if(recv_file(clnt_sock, s_id)) perror("recv_file");

		submission_routine(s_id, repo_owner, repo_name);
	}	
		
		

	close(clnt_sock);
	return NULL;


}

void register_routine(string repo_owner, string repo_name){

		//build and test? solution

		//write report

		//make issue
		string github_token;
		cout << "token: ";
		cin >> github_token;
		string title = "This is test title. for your REGISTER :) ";
		string report = "# Test body \n\n --- hello world<br> ";
		create_github_issue(title, report, repo_owner, repo_name, github_token);


}

void finish_routine(string repo_owner, string repo_name){
		//arrange the reports of submissions


		//write report


		//make issue
		string github_token;
		cout << "token: ";
		cin >> github_token;
		string title = "This is test title. for your FINISH request :) ";
		string report = "# Test body \n\n --- hello world<br> ";
		create_github_issue(title, report, repo_owner, repo_name, github_token);



}

void submission_routine(int s_id, string repo_owner, string repo_name){

		//build


		//compare and grading


		//write report



		//make issue
		string github_token;
		cout << "token: ";
		cin >> github_token;
		string title = "This is test title. for your SUBMIT :) ";
		string report = "# Test body \n\n --- hello world<br> ";
		create_github_issue(title, report, repo_owner, repo_name, github_token);





}

