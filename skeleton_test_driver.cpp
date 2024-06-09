#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sstream>
using namespace std;


//************************FIXED SECTION************************
int solution_report_mode, log_mode;
int log_fd;         // for log
char log_file[128]; // for log
int ok_fd;          // for solution_report
char ok_file[128];  // for solution_report
int total_input = 0, total_rep = 0, input_pass = 0, input_fail = 0, rep_pass = 0, rep_fail = 0;     // for solution report

void exit_routine (int exit_mode);
int input_ok(char * input);
int write_ok_report();
//*************************************************************
int rep_ok(/*appropriate parameter set */);
void write_log(/*appropriate parameter set*/);
void print_state(/*appropriate parameter set*/);


//exit when exit_mode after necessary processing
void exit_routine (int exit_mode) {

	//************************FIXED SECTION************************
    // update the solution_report info
    if (exit_mode == 0) {
        total_input++;
        input_fail++;
    }else{
        total_rep++;
        rep_fail++;
    }
	//remove log files on log mod
    if (log_mode == 1) {    
        remove(log_file);
        close (log_fd);
    }
	//write ok_report
    if (write_ok_report() == 1) remove(ok_file);
	//*************************************************************

	//TODO set more routines that need to be complement before exit (if exist)



    exit(exit_mode);

}



// TODO implement the function below
// check if the input parameter meets assignment's assumtion
// call it before using input from stdin
// usage: if(input_ok(input)) exit_routine(0);
int input_ok(char * input) {
	
	//TODO 
    if (/*input meets assignment's assumtion*/) {
	    total_input++;
	    input_pass++;
        return 0;
    }
    else {
        return 1;
    }

}


// TODO implement the function below
// check if the struct is correct
// call it after movement might change the state of structure
// usage: if(rep_ok(/*parameter*/)) exit_routine(1);
int rep_ok(/*appropriate parameter set */) {
 
	//TODO 
    if (/*the state of structure is fine*/){
	    total_rep++;
	    rep_pass++;
	    return 0;
    }
    else {
        return 1;
    }


}




// TODO implement the function below
// write the function flow into log_fd
// call it after every movement that might change the state of structure
void write_log(/*appropriate parameter set*/) {
	if (log_mode == 0) return;

	//TODO write the log into log_fd(file descriptor) using file I/O
    // ** log_fd is already opened. just write the log into log_fd, and do not close log_fd!

	//log might be the function name, parameters, etc... 




}



// TODO implement the function below 
// print state of structure
// call it after the movement that change the state of structure 
void print_state(/*appropriate parameter set*/) {

	//TODO print state of structure into stdout 


}




//************************FIXED SECTION************************
int write_ok_report() {
    if (solution_report_mode == 0) return 0;
    ostringstream oss;

    // Create the formatted string
    oss << "total_input : " << total_input << "\n";
    oss << "total_rep : " << total_rep << "\n";
    oss << "input_pass : " << input_pass << "\n";
    oss << "input_fail : " << input_fail << "\n";
    oss << "rep_pass : " << rep_pass << "\n";
    oss << "rep_fail : " << rep_fail << "\n";
    string report = oss.str();

    // Write the string to the file descriptor
    ssize_t written = 0, written_acc = 0;
    size_t report_len = report.length();

    while (written_acc < report_len) {
        written = write(ok_fd, report.c_str() + written_acc, report_len - written_acc);
        if (written == -1) {
            close (ok_fd);
            return 1; 
        }
        written_acc += written;
    }

    close (ok_fd);
    return 0; 
}
//*************************************************************


int main(int argc, char * argv[]) {

    //************************FIXED SECTION************************
    solution_report_mode = atoi(argv[1]);
    log_mode = atoi(argv[2]);

    if (log_mode == 1) {    
        pid_t pid = getpid();
        sprintf(log_file, "./.log/log_%d", pid);
        log_fd = open(log_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (log_fd == -1) {
            exit(EXIT_FAILURE);
        }
    }

    if (solution_report_mode == 1) {
        pid_t pid2 = getpid();
        sprintf(ok_file, "outputs/ok/temp/ok_%d", pid2);
        ok_fd = open(ok_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (ok_fd == -1) {
            exit(EXIT_FAILURE);
        }
    }
    //*************************************************************

    // TODO implement your test driver
    // using Menu-Driven form (recommended)




    //************************FIXED SECTION************************
    if (write_ok_report() == 1) {
        remove(ok_file); 
    }
    if (log_mode == 1) {  
        close (log_fd);
    }
    //*************************************************************

}
