#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/wait.h>
#include <libgen.h>
#include <sys/types.h>
#include <signal.h>
#include <iostream>

using namespace std;

#define BUF_SIZE 1024

int student_id = 0;


void timeout_handler(int signum) 
{
    kill(getpid(), SIGTERM);
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

//return 1 on failure, 0 on success. plz pass the opend file descriptor. 
int save_crash(int student_id, char* input_filepath, FILE* output_fp, int sub_pid, int crash_cnt){

    char filename[128];
    ssize_t len, read_len, written_len;

    //save input by execute cmd
    char cmd[1024];

    len = sprintf(filename, "crash_input_%03d", crash_cnt);
    filename[len] = 0;
    sprintf(cmd, "mv .log/log_%d submissions/%d/report/log/crash/%s", sub_pid, student_id, filename);
    if (system(cmd)) return 1; //on failure return 1

    len = sprintf(filename, "crash_input_%03d", crash_cnt);
    filename[len] = 0;
    sprintf(cmd, "cp %s submissions/%d/report/crash/%s", input_filepath, student_id, filename);
    cout << "filename: " << filename << endl << "cmd: " << cmd << endl;
    if (system(cmd)) return 1; //on failure return 1


    //save output by fileIO
    char filepath[1024];
    char buffer[1024];
    FILE* fp = 0;

    len = sprintf(filename, "crash_output_%03d", crash_cnt);
    filename[len] = 0;
    sprintf(filepath, "submissions/%d/report/crash/%s", student_id, filename);
    cout << "filename: " << filename << endl << "path: " << filepath << endl;

    if ((fp = fopen(filepath, "w")) == NULL) return 1; //on failure return 1
    while (fgets(buffer, sizeof(buffer), output_fp) !=NULL) {
        fwrite(buffer,1,strlen(buffer), fp);
    }fclose(fp);

    crash_cnt++;
    return 0;

}

//return 1 on failure, 0 on success. plz pass the opend file descriptor.
int save_incorrect(int student_id, char*input_filepath, FILE* sol_fp, FILE* sub_fp,int sol_pid, int sub_pid, int incorrect_cnt){

    char filename[128];
    ssize_t len, read_len, written_len;

    //save input by execute cmd
    char cmd[1024];

    len = sprintf(filename, "sol_log_%03d", incorrect_cnt);
    filename[len] = 0;
    sprintf(cmd, "mv .log/log_%d submissions/%d/report/log/incorrect/%s", sol_pid, student_id, filename);
    // cout << "filename: " << filename << endl << "cmd: " << cmd << endl;
    if (system(cmd)) return 1; //on failure return 1

    len = sprintf(filename, "sub_log_%03d", incorrect_cnt);
    filename[len] = 0;
    sprintf(cmd, "mv .log/log_%d submissions/%d/report/log/incorrect/%s", sol_pid, student_id, filename);
    // cout << "filename: " << filename << endl << "cmd: " << cmd << endl;
    if (system(cmd)) return 1; //on failure return 1

    len = sprintf(filename, "input_%03d", incorrect_cnt);
    filename[len] = 0;
    sprintf(cmd, "cp %s submissions/%d/report/incorrect/%s", input_filepath, student_id, filename);
    // cout << "filename: " << filename << endl << "cmd: " << cmd << endl;
    if (system(cmd)) return 1; //on failure return 1

    //save outputs by fileIO
    char filepath[1024];
    char buffer[1024];
    FILE * fp = NULL;

    len = sprintf(filename, "sol_output_%03d", incorrect_cnt);
    filename[len] = 0;
    len = sprintf(filepath, "submissions/%d/report/incorrect/%s", student_id, filename);
    filepath[len] = 0;
    // cout << "filename: " << filename << endl << "path: " << filepath << endl;

    if ((fp = fopen(filepath, "w")) == NULL) return 1; //on failure return 1
    while (fgets(buffer, sizeof(buffer), sol_fp) !=NULL) {
        fwrite(buffer,1,strlen(buffer), fp);
    }fclose(fp);

    len = sprintf(filename, "sub_output_%03d", incorrect_cnt);
    filename[len] = 0;
    len = sprintf(filepath, "submissions/%d/report/incorrect/%s", student_id, filename);
    filepath[len] = 0;
    // cout << "filename: " << filename << endl << "path: " << filepath << endl;

    if ((fp = fopen(filepath, "w")) == NULL) return 1; //on failure return 1
    while (fgets(buffer, sizeof(buffer), sub_fp) !=NULL) {
        fwrite(buffer,1,strlen(buffer), fp);
    }fclose(fp);

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

//return 1 somthing error
int exec_input(char * sol_exec_path, char * sub_exec_path, char * input_dir_path,int * total_cnt, int * crash_cnt, int * incorrect_cnt, int student_id){
    DIR * dir = opendir(input_dir_path) ;
    if(dir == NULL)
    {
        return 1;
    }

    struct dirent *ent = NULL ;
    struct stat st;

    while ((ent = readdir(dir)))
    {
        
        FILE * sol_stderr = tmpfile();
        FILE * sol_stdout = tmpfile();

        FILE * sub_stderr = tmpfile();
        FILE * sub_stdout = tmpfile();

        char * input_file_path = (char *) malloc(strlen(input_dir_path)+1+strlen(ent->d_name) + 1);
        strcpy(input_file_path, input_dir_path);
        strcat(input_file_path,"/");
        strcat(input_file_path,ent->d_name);
        if(lstat(input_file_path, &st) == -1)
        {
            free(input_file_path) ;
            closedir(dir) ;
            return 1;
        }

        if (!S_ISLNK(st.st_mode) && S_ISREG(st.st_mode))
        {
            
            (*total_cnt)+=1;
            //exec solution
            int sol_pid = fork();

            if( sol_pid < 0 )
            {
                perror("can't create process");
            }
            else if( sol_pid == 0)
            {
                int input_fd = open(input_file_path, O_RDONLY);
                if (input_fd == -1) {
                    perror("open");
                    return 1;
                }
                if (dup2(input_fd, STDIN_FILENO) == -1) {
                    perror("dup2");
                    close(input_fd);
                    return 1;
                }

                if (dup2(fileno(sol_stderr), STDERR_FILENO) == -1) {
                    perror("dup2");
                    return 1;
                }

                if (dup2(fileno(sol_stdout), STDOUT_FILENO) == -1) {
                    perror("dup2");
                    return 1;
                }

                char *args[] = {sol_exec_path, "0", "1",NULL};
                alarm(3);

                if(execv(sol_exec_path,args))                                 
                {                                                             
                    perror("exec");                                           
                }                                                             
                exit(1);                                                      
            }
            int sol_status;
            waitpid(sol_pid,&sol_status,0);
            //exec submission
            int sub_pid = fork();

            if( sub_pid < 0 )
            {
                perror("can't create process");
            }
            else if( sub_pid == 0)
            {
                int input_fd = open(input_file_path, O_RDONLY);
                if (input_fd == -1) {
                    perror("open");
                    return 1;
                }
                if (dup2(input_fd, STDIN_FILENO) == -1) {
                    perror("dup2");
                    close(input_fd);
                    return 1;
                }

                if (dup2(fileno(sub_stderr), STDERR_FILENO) == -1) {
                    perror("dup2");
                    return 1;
                }

                if (dup2(fileno(sub_stdout), STDOUT_FILENO) == -1) {
                    perror("dup2");
                    return 1;
                }
                char *args[] = {sub_exec_path, "0", "1",NULL}; 
                alarm(3);
                if(execv(sub_exec_path,args))                                 
                {                                                             
                    perror("exec");                                           
                }                                                             
                exit(1);                                                      
            }                                                                  

            

            int sub_status;
            waitpid(sub_pid,&sub_status,0);
            
            rewind(sub_stdout);
            rewind(sub_stderr);
            rewind(sol_stdout);
            rewind(sol_stderr);

            if (WIFEXITED(sub_status)) {
            // 자식 프로세스가 정상 종료되었는지 확인
                // printf("sub exited with status %d\n", WEXITSTATUS(sub_status));
                // printf("%s\n",input_file_path);
                // printf("signaled %d\n",WIFSIGNALED(sub_status));
                
            } 
            if (WIFSIGNALED(sub_status)) {
                // 자식 프로세스가 시그널로 인해 종료되었는지 확인 (크래시)
                if(WIFEXITED(sol_status) || (WIFSIGNALED(sol_status) && (WTERMSIG(sub_status) != WTERMSIG(sol_status))) )
                {
                    //Todo checking timeout using SIGTERM
                    save_crash(student_id, input_file_path, sub_stderr, sub_pid,*crash_cnt); 
                    (*crash_cnt)++;
                    free(input_file_path) ;                                                     
                    fclose(sol_stderr);
                    fclose(sol_stdout);                                                  
                    fclose(sub_stderr);                                                  
                    fclose(sub_stdout); 
                    continue;
                }
                
            }
            



            // 출력이 잘 됐는지 확인 
            char sub_buff[256];
            char sol_buff[256];

            // while(fgets(sub_buff, sizeof(sub_buff), sub_stdout) !=NULL)
            // {
            //     printf("%s\n",sub_buff);
            // }

            // compare sol_output and sub_output
            while(fgets(sub_buff, sizeof(sub_buff), sub_stdout) !=NULL)
            {
                fgets(sol_buff,sizeof(sol_buff), sol_stdout);
                if(strcmp(sol_buff,sub_buff)!=0)
                {
                    rewind(sol_stdout);
                    rewind(sub_stdout);
                    //TODO change incorrect_cnt to student_id
                    // printf("save incorrect\n");
                    save_incorrect(student_id, input_file_path, sol_stdout,sub_stdout,sol_pid,sub_pid, *incorrect_cnt); 
                    (*incorrect_cnt)++;
                    break;
                }
            }
                                                                              
        }                                                                                                                                      
        else {                                                                
        }                                                                     
        free(input_file_path);
        fclose(sol_stderr);
        fclose(sol_stdout);                                                  
        fclose(sub_stderr);                                                  
        fclose(sub_stdout);                                                  

    }                                                                         
                                                                              
    closedir(dir);  
    return 0;                                                          
}                                                                             
// int main()                                                                    
// {    
//     signal(SIGALRM, timeout_handler);
    
//     int total_cnt=0;
//     int crash_cnt=0;
//     int incorrect_cnt=0;
//     int student_id = 0;
//     // exec_input("./solution_bst.out","./submission_bst1.out","solution_fuzz_output/default/queue");
//     // exec_input("./solution_bst.out","./submission_bst3.out","solution_fuzz_output/default/queue", &totall_cnt, &crash_cnt, &incorrect_cnt);
//     exec_input("./testcopy.out","./test.out","Testinput", &total_cnt, &crash_cnt, &incorrect_cnt, student_id);
//     printf("totall: %d crash: %d incorrect: %d\n",total_cnt,crash_cnt,incorrect_cnt);
// }                                                                             
                              

