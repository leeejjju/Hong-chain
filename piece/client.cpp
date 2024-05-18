#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <fcntl.h>
#include <signal.h>
#include <dirent.h>
#include <sys/stat.h>
#include <limits.h>
#include <arpa/inet.h>
#define BUF_SIZE 1024
#define DEBUG

void send_file(char* filepath);

char serv_ip[PATH_MAX];
int new_port;

int s_id;
char* commit_msg;

// uploads all files and subdirs to server
// receive the result from server and print it
// usage: ./execution IP PORT DIR
int main(int argc, char *argv[]){

	//check args first
	if (argc != 4) {
		printf("Usage : %s <IP> <port> <student_id>\n", argv[0]);
		exit(1);
	 } 

	 commit_msg = argv[3];
	 //s_id = atoi(argv[3]);
     strcpy(serv_ip, argv[1]);

	//setting for socket programming
	struct sockaddr_in serv_addr;
	pthread_t snd_thread, rcv_thread;
	void * thread_return;

	int sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(atoi(argv[2]));
	if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) perror("connect");

	//4byte studentname, 4byte filesize, nbyte submission file << hope

	//send student id first
	int len;
	if((len = write(sock, &commit_msg, sizeof(int))) <= 0) perror("write");
	else printf("send student id: %d (%d byte)\n", s_id, len);
   
   	//send submission.cpp file
	//TODO get filesize
	int filesize = 999;                     
	if((len = write(sock, &filesize, sizeof(int))) <= 0) perror("write");
	else printf("send file size: %d (%d bytes)\n", filesize, len);

    //send file contents
    char buf[BUF_SIZE];
	char filepath[128] = "../submission.cpp";

	int fd;
	if((fd = open(filepath, O_RDONLY)) <= 0) perror("open");
	while (len = read(fd, buf, BUF_SIZE)){
        buf[len] = 0;
		write(sock, buf, len);
	}

	close(fd);
    shutdown(sock, SHUT_WR); //for recv... 




	//TODO wait enough...

	//TODO and recv report file

	//TODO make artifact?? idk how to do it......


    close(sock);
    return 0;

}


//connect new sock, read file and send to server
void send_file(char* filepath){

   //connect again with new port
    struct sockaddr_in serv_addr;
	int send_sock = socket(PF_INET, SOCK_STREAM, 0);
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(serv_ip);
	serv_addr.sin_port = htons(new_port);
	if (connect(send_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) perror("connect");

	//TODO get filesize
	int filesize = 999;                     
    int len = 0;

	if((len = write(send_sock, &filesize, sizeof(int))) <= 0) perror("writei");
	else printf("[send_file] send file size: %d (%d bytes)\n", filesize, len);

    //send file contents
    char buf[BUF_SIZE];
	int fd;
	if((fd = open(filepath, O_RDONLY)) <= 0) perror("cannot open file: may the file not exist\n");
	
	while (len = read(fd, buf, BUF_SIZE)){
        buf[len] = 0;
		write(send_sock, buf, len);
	}
	close(fd);
    shutdown(send_sock, SHUT_WR);

}

