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

#define BUF_SIZE 1024
#define NEW_PORT_BASE 5000

// #define DEBUG

void *handle_clnt(void * arg);
int recv_file(int sock, int s_id);

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

	switch(s_id){

	case 0:
		printf("REGISTER\n");

		//recv test_driver.cpp
		if(recv_file(clnt_sock, s_id)) perror("recv_file");
		//recv solution.cpp
		if(recv_file(clnt_sock, s_id)) perror("recv_file");

		//build and test? solution
		//TODO system("sh build.sh");
		//write report
		//make issue 
		close(clnt_sock);
		return NULL;

	case -1:
		printf("FINISH\n");
		//arrange the reports of submissions
		//write report
		//make issue
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

		//build
		//compare and grading
		//write report
		//make issue

	}	
		
		

	close(clnt_sock);
	return NULL;


}


//save file from sock
int recv_file(int sock, int s_id) {

    uint32_t filename_len;
    char filename[BUF_SIZE-128];
    char filepath[BUF_SIZE];
    uint32_t file_size;

    ssize_t recv_len;


	//get filename_len
    if ((recv_len = recv(sock, &filename_len, sizeof(filename_len), 0)) <= 0) {
        std::cerr << "Failed to receive filename length: " << recv_len << std::endl;
        return 1;
    }

	//get filename
    recv_len = recv(sock, filename, filename_len, 0);
    if (recv_len != filename_len) {
        std::cerr << "Failed to receive filename" << std::endl;
        return 1;
    }
    else filename[filename_len] = 0;
	if(s_id) sprintf(filepath, "submissions/%d/%s", s_id, basename(filename));
	else strcpy(filepath, basename(filename));

	//get file contents and save them into new file
	int fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd <= 0) return 1;


    // 파일 크기 수신
    recv_len = recv(sock, &file_size, sizeof(file_size), 0);
    if (recv_len != sizeof(file_size)) {
        std::cerr << "Failed to receive file size" << std::endl;
		close(fd);
        return 1;
    }

    // 파일 데이터 수신 및 저장
    uint32_t total_recv_len = 0;
	char tok[1];
    while (total_recv_len < file_size) {
        if ((recv_len = recv(sock, &tok, 1, 0)) <= 0) {
            std::cerr << "Error receiving file data" << std::endl;
			close(fd);
            return 1;
        }
		write(fd, tok, recv_len);
        total_recv_len += recv_len;
    }

    // 파일 닫기
	close(fd); //done writing
	printf("succesfully recved and saved file: %s\n", filepath);

    return 0;
}


