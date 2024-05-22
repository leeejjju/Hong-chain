#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
using namespace std;
#define BUF_SIZE 1024


//script that make directory if not exist... 
//above variables are need to be maintained. (where..?)
int crash_cnt = 0;
int incorrect_cnt = 0;

//return 1 on failure, 0 on success. plz pass the opend file descriptor. 
int  save_crash(int student_id, char* input_filepath, int output_fd){

	char filename[128];
	ssize_t len, read_len, written_len;

	//save input by execute cmd
	char cmd[1024];

	len = sprintf(filename, "crash_input_%03d", crash_cnt);
	filename[len] = 0;
	sprintf(cmd, "cp %s submissions/%d/report/crash/%s", input_filepath, student_id, filename);
	cout << "filename: " << filename << endl << "cmd: " << cmd << endl;
    if (system(cmd)) return 1; //on failure return 1


	//save output by fileIO
	char filepath[1024];
	char buffer[1024];
	int fd = 0;

	len = sprintf(filename, "crash_output_%03d", crash_cnt);
	filename[len] = 0;
	sprintf(filepath, "submissions/%d/report/crash/%s", student_id, filename);
	cout << "filename: " << filename << endl << "path: " << filepath << endl;

    if ((fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) return 1; //on failure return 1
    while ((read_len = read(output_fd, buffer, sizeof(buffer))) > 0) {
        written_len = write(fd, buffer, read_len);
        if (written_len != read_len) {
            close(fd);
            return 1;
        }
    }close(fd);

	crash_cnt++;
	return 0;

}

//return 1 on failure, 0 on success. plz pass the opend file descriptor.
int save_incorrect(int student_id, char*input_filepath, int sol_fd, int sub_fd){

	char filename[128];
	ssize_t len, read_len, written_len;

	//save input by execute cmd
	char cmd[1024];

	len = sprintf(filename, "input_%03d", incorrect_cnt);
	filename[len] = 0;
	sprintf(cmd, "cp %s submissions/%d/report/incorrect/%s", input_filepath, student_id, filename);
	cout << "filename: " << filename << endl << "cmd: " << cmd << endl;
    if (system(cmd)) return 1; //on failure return 1

	//save outputs by fileIO
	char filepath[1024];
	char buffer[1024];
	int fd = 0;

	len = sprintf(filename, "sol_output_%03d", incorrect_cnt);
	filename[len] = 0;
	len = sprintf(filepath, "submissions/%d/report/incorrect/%s", student_id, filename);
	filepath[len] = 0;
	cout << "filename: " << filename << endl << "path: " << filepath << endl;

	if ((fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) return 1; //on failure return 1
    while ((read_len = read(sol_fd, buffer, sizeof(buffer))) > 0) {
        written_len = write(fd, buffer, read_len);
        if (written_len != read_len) {
            close(fd);
            return 1;
        }
    }close(fd);

	len = sprintf(filename, "sub_output_%03d", incorrect_cnt);
	filename[len] = 0;
	len = sprintf(filepath, "submissions/%d/report/incorrect/%s", student_id, filename);
	filepath[len] = 0;
	cout << "filename: " << filename << endl << "path: " << filepath << endl;

	if ((fd = open(filepath, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) return 1; //on failure return 1
    while ((read_len = read(sub_fd, buffer, sizeof(buffer))) > 0) {
        written_len = write(fd, buffer, read_len);
        if (written_len != read_len) {
            close(fd);
            return 1;
        }
    }close(fd);

	incorrect_cnt++;
	return 0;

}



//return 1 if they are same, return 0 if not the same. pass the &index to save index of submission crash.
int are_they_same(int sol_fd, int sub_fd, int* index){

	// https://github.com/google/sanitizers/wiki/AddressSanitizer
	char keywords[8][32] = {"heap-use-after-free", "heap-buffer-overflow", "stack-buffer-overflow", "global-buffer-overflow", "stack-use-after-return", "stack-use-after-scope", "initialization-order-fiasco", "memory leaks" };
	int sol_idx = -1, sub_idx = -1;
	char buffer[512]; //TODO if sanitizer is only one in stderr, the keyword surely in first 512 texts. but are there other cases? 
	int len;

	len = read(sol_fd, buffer, sizeof(buffer));	
	buffer[len] = 0; 
	for(int i = 0; i < 8; i++){
		if(strstr(buffer, keywords[i]) != nullptr){
			sol_idx = i;
			break;
		}
	}

	len = read(sub_fd, buffer, sizeof(buffer));	
	buffer[len] = 0; 
	for(int i = 0; i < 8; i++){
		if(strstr(buffer, keywords[i]) != nullptr){
			sub_idx = i;
			break;
		}
	}

	*index = sub_idx;
	if(sol_idx != -1 && sol_idx == sub_idx) return 1; //if same
	else return 0; // if not

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

/*
int main(void){

	//save_incorrect(22100679, " ", "SUBMEOW/MEOW", "SUBMEOW/MEOW3");
	char tmp[10] = "testinput";

	int fd1, fd2, fd3;
	fd1 = open(tmp, O_RDONLY); // 예시로 임시 파일을 연다
	fd2 = open(tmp, O_RDONLY); // 예시로 임시 파일을 연다
	fd3 = open(tmp, O_RDONLY); // 예시로 임시 파일을 연다
    

	if(save_crash(22100579, tmp, fd1)) perror("save_crash");
	if(save_incorrect(22100579, tmp, fd2, fd3)) perror("save_crash");

	close(fd1);
	close(fd2);
	close(fd3);

	int err_idx;
	int solfd, subfd;
	solfd = open("solerr", O_RDONLY);
	subfd = open("suberr", O_RDONLY);
	if(are_they_same(solfd, subfd, &err_idx)) cout << "they are same. idx: " << err_idx << endl;
	else cout << "they are not the same. idx: " << err_idx << endl;

}
*/
