// On my honor, I pledge that I have neither received nor provided improper assistance in the completion of this assignment. 
// Signed: 조다빈 Student Number: 22000711

#ifndef CLOCK_H
#define CLOCK_H

struct Clock {
    int hr, min, sec;
};

using pClock = Clock*;

void tick(pClock clk);
void show(pClock clk, char end = '\n');
void runs(pClock clk, char end = '\n');

#endif