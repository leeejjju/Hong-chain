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
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

char keywords[9][32] = {"heap-use-after-free", "heap-buffer-overflow", "stack-buffer-overflow", "global-buffer-overflow", "stack-use-after-return", "stack-use-after-scope", "initialization-order-fiasco", "memory leaks", "infinite loop"};

int crash_cnt = 0;
int incorrect_cnt = 0;


// return 1 on failure, 0 on success
int file_content_to_report(const char * filename, string& report_str) {
    ifstream file(filename);
    if (!file.is_open()) {
        return 1; // Failed to open file
    }

    string line;
    while (getline(file, line)) {
        report_str += line + "\n"; // Add each line to the report string
        report_str += "<br>";
    }

    file.close();
    return 0;
}


// return 1 on failure, 0 on success
string write_report(int student_id, string& report_str) {
    char crash_log[128];
    char sub_log[128];
    char sol_log[128];
    int i;
    string buf, error = "ERROR";
    struct stat st1, st2;
    bool pass_flag = true;

    // write "Crash"
    report_str += "## Crash\n";
    // check crash log files and write md
    for (i = 0; i < 9; i++) {
        sprintf(crash_log, "submissions/%d/report/log/crash/crash_log_%d", student_id, i);
        if (stat(crash_log, &st1) == -1) {
            continue;
        }
        pass_flag = false;
        // write markdown
        report_str += "<details><summary>";
        report_str += keywords[i];
        report_str += "</summary>";

        // open crash_log and write to report_str
        if (file_content_to_report(crash_log, report_str) == 1) {
            return "Failed filecontent to string";
        }

        report_str += "</details>";
    }


    // write "Incorrect"
    report_str += "\n\n## Incorrect\n";
    // check incorrect log files and write md
    for (i = 0; i < 3; i++) {
        sprintf(sol_log, "submissions/%d/report/log/incorrect/sol_log_%d", student_id, i);
        sprintf(sub_log, "submissions/%d/report/log/incorrect/sub_log_%d", student_id, i);
        if (stat(sol_log, &st1) == -1 && stat(sub_log, &st2) == -1) {
            continue;
        }
        stat(sub_log, &st2);
        pass_flag = false;

        // write markdown
        report_str += "<details><summary>";
        report_str += "solution_log_" + to_string(i+1);
        report_str += "</summary>";
        // open sol_log and write to report_str
        if (file_content_to_report(sol_log, report_str) == 1) {
            return "Failed file content to string";
        }
        report_str += "</details>";

        // write markdown
        report_str += "<details><summary>";
        report_str += "submission_log_" + to_string(i+1);
        report_str += "</summary>";
        // open sub_log and write to report_str
        if (file_content_to_report(sub_log, report_str) == 1) {
            return "Failed file content to string";
        }
        report_str += "</details>";
    }

    // write grading info

    // write pass/faile
    report_str = (pass_flag ? "# PASS\n" : "# FAIL\n") + report_str;

    // recore the pass/fail to directory for instructor
    char prev_file[128];
    string grade = pass_flag ? "pass" : "fail";
    sprintf(prev_file, "submissions/%d/pass", student_id);
    remove(prev_file);
    sprintf(prev_file, "submissions/%d/fail", student_id);
    remove(prev_file);
    string grade_file = "submissions/" + to_string(student_id) + "/" + grade;
    string cmd = "mkdir " + grade_file;
    const char * cmd_str = cmd.c_str();
    system(cmd_str);

    return report_str;
}



// return 1 on failure, 0 on success
// return a report content as a string which is based on the files inside the submissions/$(sid)/report/log directory
// Save pass/fail information as existence of submissions/$(sid)/pass or fail directory
string create_report(int student_id) {

    // define report_str
    string report_str;
    string error = "ERROR";

    write_report(student_id, report_str);

    return report_str;
}


string finish() {
    DIR * dir = opendir("submissions");
    if(dir == NULL) {
        return "ERROR";
    }

    struct dirent * entry = NULL ;
    struct stat st;
    string score_report, student_dir, pass_file;

    score_report += "## Score Report\n";

    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != DT_DIR || entry->d_name[0] == '.') {
            continue;
        }

        score_report += entry->d_name;
        score_report +=  ": ";

        student_dir = string("submissions/") + entry->d_name;
        pass_file = student_dir + "/pass";
        if (stat(pass_file.c_str(), &st) == 0) {
            score_report += "pass\n";
        }
        else {
            score_report += "fail\n";
        }
    }

    closedir (dir);

    return score_report;
}


string register_issue(string lib_name = "" ) {

    string cmd = string("./build.sh ") + lib_name;
    const char * cmd_str = cmd.c_str();
    system(cmd_str);

    string register_report;

    register_report = "## REGISTER Report\n";

    if (file_content_to_report("outputs/report", register_report) == 1) {
        return "Read register_issue report failed\n";
    }

    return register_report;
}


int main() {
    // cout << create_report(22000711);
    // cout << endl<< endl;
    cout << finish();
    cout << "here";
    // cout << endl;

    // cout << register_issue();
}



















