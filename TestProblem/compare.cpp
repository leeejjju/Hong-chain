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
#include <iostream>
#include <signal.h>

using namespace std;


int crash_cnt = 0;
int incorrect_cnt = 0;
int student_id = 0;

void timeout_handler(int signum) 
{
    kill(getpid(), SIGTERM);
}

//return 1 on failure, 0 on success. plz pass the opend file descriptor. 
int save_crash(int student_id, char* input_filepath, int output_fd){

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

//return 1 somthing error
int exec_input(char * sol_exec_path, char * sub_exec_path, char * input_dir_path){
    DIR * dir = opendir(input_dir_path) ;
    if(dir == NULL)
    {
        return 1;
    }

    struct dirent *ent = NULL ;
    struct stat st;

    while (ent = readdir(dir))
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

                char *args[] = {sol_exec_path, NULL};
                alarm(3);
                if(execv(sol_exec_path,args))                                 
                {                                                             
                    perror("exec");                                           
                }                                                             
                exit(1);                                                      
            }

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

                char *args[] = {sub_exec_path, NULL}; 
                alarm(3);
                if(execv(sub_exec_path,args))                                 
                {                                                             
                    perror("exec");                                           
                }                                                             
                exit(1);                                                      
            }                                                                  

            int sol_status;
            waitpid(sol_pid,&sol_status,0);

            int sub_status;
            waitpid(sub_pid,&sub_status,0);
            
            rewind(sub_stdout);
            rewind(sub_stderr);
            rewind(sol_stdout);
            rewind(sol_stderr);

            // if (WIFEXITED(sol_status)) {
            // // 자식 프로세스가 정상 종료되었는지 확인
            //     // printf("sol exited with status %d\n", WEXITSTATUS(sol_status));
            // } 
            // else if (WIFSIGNALED(sol_status)) {
            //     // 자식 프로세스가 시그널로 인해 종료되었는지 확인 (크래시)
            //     printf("sol killed by signal %d\n", WTERMSIG(sol_status));
            //     if (WCOREDUMP(sol_status)) {
            //         printf("sol produced a core dump.\n");
            //     }
            // }

            if (WIFEXITED(sub_status)) {
            // 자식 프로세스가 정상 종료되었는지 확인
                // printf("sub exited with status %d\n", WEXITSTATUS(sub_status));
            } 
            else if (WIFSIGNALED(sub_status)) {
                // 자식 프로세스가 시그널로 인해 종료되었는지 확인 (크래시)
                if((WIFSIGNALED(sol_status) && (WTERMSIG(sub_status) != WTERMSIG(sol_status))) || !WIFSIGNALED(sol_status))
                {
                    //Todo checking timeout using SIGTERM
                    save_crash(student_id, input_file_path, fileno(sub_stderr)); 
                        // printf("call save_crash_routin\n");
                    continue;
                }
                
            }
            



            // 출력이 잘 됐는지 확인 
            char sub_buff[256];
            char sol_buff[256];

            // while(fgets(sub_buff, sizeof(sub_buff), sub_stderr) !=NULL)
            // {
            //     printf("%s\n",sub_buff);
            // }

            //compare sol_output and sub_output
            while(fgets(sub_buff, sizeof(sub_buff), sub_stdout) !=NULL)
            {
                fgets(sol_buff,sizeof(sol_buff), sol_stdout);
                if(strcmp(sol_buff,sub_buff)!=0)
                {
                    rewind(sol_stdout);
                    rewind(sub_stdout);
                    //TODO change incorrect_cnt to student_id
                    save_incorrect(student_id, input_file_path, fileno(sol_stdout), fileno(sub_stdout)); 
                    break;
                }
            }
                                                                              
        }                                                                                                                                      
        else {                                                                
        }                                                                     
        free(input_file_path) ;                                                     
    }                                                                         
                                                                              
    closedir(dir);  
    return 0;                                                          
}                                                                             
int main()                                                                    
{    
    signal(SIGALRM, timeout_handler);
    // exec_input("./solution","./submission_bst1","solution_fuzz_output/default/queue");
    exec_input("./solution","./submission_bst3","solution_fuzz_output/default/queue");

    // exec_input("./testcopy","./test","Testinput");

}                                                                             
                              
