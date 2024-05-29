

//exit with exit_mode after necessary processing
int exit_routine(int exit_mode){

	// ***************************fixed section********************************
	// update solution_report info
	if(exit_mode == 0){
    	total_input++;
    	input_fail++;
	}else{
        total_rep++;
        rep_fail++;
	}
	//remove log files
    if (log_mode == 1) {    
       remove(log_file);
       close (fd);
	}
	// write ok_result
    if (write_ok_report(solution_report_mode, ok_fd, total_input, total_rep, input_pass, input_fail, rep_pass, rep_fail) == 1) remove(ok_file);
	//*************************************************************************
	//TODO set more routine that need to be complement before exit


	exit(exit_mode);

}


// TODO implement the function below
// check if the input parameter meets assignment's assumtion
// call it before using every input from stdin
// usage: if(input_ok(input)) exit_routine(0);
int input_ok(char* input) {

	//if input meets assignment's assumtion
	return 0;

	//if not
    return 1;

}


// TODO implement the function below
// check if the struct is correct
// call it after movement might change the state of structure
// usage: if(rep_ok(/*parameter*/)) exit_routine(1);
int rep_ok(/*appropriate parameter set */) {
 
	//if the state of structure is fine
	return 0;

	//if not
    return 1;

	return{
		0;
	}

}


// TODO implement the function below
// record the function flow on fd(file descriptor)
// call it after movement might change the state of structure
void write_log(char * log_buf) {

	if (log_mode == 0) return;

    size_t log_len = strlen(log_buf);
    ssize_t written = 0, written_acc = 0;

    while (written_acc != log_len) {
        written = write(fd, log_buf + written_acc, log_len - written_acc);
        written_acc += written;
    }
}


// print function checks the status after the command is executed
void print_stack(Node * p) {
    printf("-------------- PRINT STACK ----------------\n");
    while (p != nullptr) {
        printf("%c  ", p->data);
        p = p->next;
    }
   printf("\n-------------- PRINT STACK ----------------\n\n\n");
}


// return 1 on failure, 0 on success
// After saving the parameter values to outputs/ok/temp_result, then execute cal.sh
// This function is called in the following cases:
// (1) Before exit_failure is executed due to a fail in either input_ok or rep_ok.
// (2) Before normal termination in main after both input_ok and rep_ok have passed.
int write_ok_report(int solution_report_mode, int ok_fd, int total_input, int total_rep, int input_pass, int input_fail, int rep_pass, int rep_fail) {
    if (solution_report_mode == 0) {
        return 0;
    }

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
            return 1; // error writing to file
        }
        written_acc += written;
    }
    
    // why...?
    write(ok_fd, " ", 2);

    close (ok_fd);

    return 0; // success
}


int main(int argc, char * argv[]) {

    // Save the Mode received as an argv[]
    // argv[1] : solution_report mode, argv[2] : log mode
    // 0 : turn off, 1: turn on
    int solution_report_mode = atoi(argv[1]);
    int log_mode = atoi(argv[2]);

    Node * stack = nullptr;

    char buf[32];
    scanf("%31s", buf);
    buf[31] = '\0';

    // printf("len: %ld\n", strlen(buf));

    int before_size;    // for rep_ok
    char log_file[128]; // for log
    char log_buf[512];  // for log
    int fd;             // for log
    char ok_file[128];  // for solution_report
    int ok_fd;           // for solution_report
    int total_input = 0, total_rep = 0, input_pass = 0, input_fail = 0, rep_pass = 0, rep_fail = 0;     // for solution report

    
    /* for log*/   
    if (log_mode == 1) {    
        pid_t pid = getpid();
        sprintf(log_file, "./.log/log_%d", pid);
        fd = open(log_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1) {
            exit(EXIT_FAILURE);
        }
    }
    /* for log */


    /* for saving input_ok, rep_ok return value */
    if (solution_report_mode == 1) {
        pid_t pid2 = getpid();
        sprintf(ok_file, "outputs/ok/temp/ok_%d", pid2);
        ok_fd = open(ok_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (ok_fd == -1) {
            exit(EXIT_FAILURE);
        }
    }
    /* for saving input_ok, rep_ok return value */

    for (char * ch = buf; *ch != 0x0; ch++) {
        switch (*ch) {
            case 'a':
                // printf("ch: %c\n", *ch);
                if (*(ch+1) != 0x0) {
                    ch++;
                }

                // check input is correct
               	if(input_ok()) exit_routine(1); 
                // update solution_report info
                total_input++;
                input_pass++;

                before_size = stack_size(stack);
                stack = stack_push(stack, *ch);

                // record log
                sprintf(log_buf, "stack Pushed\n");
                write_log(log_mode, fd, log_buf);
                
                // check struct 
                                // update solution_report info
                total_rep++;
                rep_pass++;

                print_stack(stack);
                break;

            case 'b':
                before_size = stack_size(stack);
                stack = stack_pop(stack);

                // record log
                sprintf(log_buf, "Popping from stack\n");
                write_log(log_mode, fd, log_buf);

                // check struct 
                if ((rep_ok(before_size, stack_size(stack), 1)) == 1) {
                    if (log_mode == 1) {    
                        remove(log_file);
                        close (fd);
                    }
                    // update solution_report info
                    total_rep++;
                    rep_fail++;
                    // write ok_result
                    if (write_ok_report(solution_report_mode, ok_fd, total_input, total_rep, input_pass, input_fail, rep_pass, rep_fail) == 1) {
                        remove(ok_file);
                    }
                    exit(EXIT_FAILURE);
                }

                // update solution_report info
                total_rep++;
                rep_pass++;

                print_stack(stack);
                break;

            default:
                break;
        }
    }   

    if (!stack_empty(stack)) {
        printf("top node data: %c\n", stack_top(stack)->data);
    }

    stack_clear(stack);


    if (solution_report_mode == 1) {    
        // write ok_result
        if (write_ok_report(solution_report_mode, ok_fd, total_input, total_rep, input_pass, input_fail, rep_pass, rep_fail) == 1) {
            remove(ok_file); 
        }
    }
    if (log_mode == 1) {  
        close (fd);
    }
}