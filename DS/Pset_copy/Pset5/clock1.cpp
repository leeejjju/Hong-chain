#include <iostream>
#include <iomanip>

struct Clock {
    int hr, min, sec;
};

// increment the time by one second.
void tick(Clock *ptr) {
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
void show(Clock *ptr) {
    std::cout.fill('0');
    std::cout << std::setw(2) << ptr->hr << ":"
              << std::setw(2) << ptr->min << ":"
              << std::setw(2) << ptr->sec << std::endl;
}

void tick(Clock *ptr);
void show(Clock *ptr);

int main (void) {
    Clock clock = {23, 59, 56};

    for (int i = 0; i < 6; i++) {
        tick(&clock);
        show(&clock);
    }
    return 0;
}