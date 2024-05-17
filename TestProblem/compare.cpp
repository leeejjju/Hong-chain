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

            if (WIFEXITED(sol_status)) {
            // 자식 프로세스가 정상 종료되었는지 확인
                // printf("sol exited with status %d\n", WEXITSTATUS(sol_status));
            } 
            else if (WIFSIGNALED(sol_status)) {
                // 자식 프로세스가 시그널로 인해 종료되었는지 확인 (크래시)
                printf("sol killed by signal %d\n", WTERMSIG(sol_status));
                if (WCOREDUMP(sol_status)) {
                    printf("sol produced a core dump.\n");
                }
            }

            if (WIFEXITED(sub_status)) {
            // 자식 프로세스가 정상 종료되었는지 확인
                // printf("sub exited with status %d\n", WEXITSTATUS(sub_status));
            } 
            else if (WIFSIGNALED(sub_status)) {
                // 자식 프로세스가 시그널로 인해 종료되었는지 확인 (크래시)
                printf("sub killed by signal %d\n", WTERMSIG(sub_status));
                if (WCOREDUMP(sub_status)) {
                    printf("sub produced a core dump.\n");
                }
            }
            rewind(sub_stdout);
            rewind(sub_stderr);
            rewind(sol_stdout);
            rewind(sol_stderr);



            // 출력이 잘 됐는지 확인 
            char sub_buff[255];
            while(fgets(sub_buff, sizeof(sub_buff), sol_stdout) !=NULL)
            {
                printf("%s\n",sub_buff);
            }
                                                                              
        }                                                                                                                                      
        else {                                                                
        }                                                                     
        free(input_file_path) ;                                                     
    }                                                                         
                                                                              
    closedir(dir);                                                            
}                                                                             
int main()                                                                    
{          
    exec_input("./test","./test","Testinput");
}                                                                             
                              