// On my honor, I pledge that I have neither received nor provided improper assistance in the completion of this assignment. 
// Signed: 조다빈 Student Number: 22000711

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include "clockDabeenCho.h"
/*
struct Clock {
    int hr, min, sec;
};

using pClock = Clock*;
*/

#if 0
void tick(pClock ptr);
void show(pClock ptr, char end = '\n');
void runs(pClock ptr, char end = '\n');

int main (void) {
    pClock ptr = new Clock {11, 59, 01};

    runs(ptr);
    
    delete ptr;
    return 0;
}
#endif

// increment the time by one second.
void tick(pClock ptr) {
    ptr->sec++;
    if (ptr->sec == 60) {
        ptr->sec = 0;
        ptr->min++;
    }
    if (ptr->min == 60) {
        ptr->min = 0;
        ptr->hr++;
    }
    if (ptr->hr == 24) 
        ptr->hr = 0;
}

// show the current time in military form.
void show(pClock ptr, char end) {
    std::cout.fill('0');
    std::cout << std::setw(2) << ptr->hr << ":"
              << std::setw(2) << ptr->min << ":"
              << std::setw(2) << ptr->sec << end << std::flush;
}

// ticks and redisplays the time at the same line continuously.
void runs(pClock clk, char end) {
    while (true) {
        sleep(1);

        tick(clk);
        show(clk, end);
    }
}