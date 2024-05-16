// On my honor, I pledge that I have neither received nor provided improper assistance in the completion of this assignment. 
// Signed: 조다빈 Student Number: 22000711

#include <iostream>
#include <vector>
using namespace std;

#ifdef DEBUG
#define DPRINT(func) func;
#else
#define DPRINT(func) ;
#endif

struct Stack {
    vector<string> item;
};

using stack = Stack *;

void free(stack s) {
    delete s;
}

int size(stack s) {
    return s->item.size();
}

int capacity(stack s) {
    return s->item.capacity();
}

bool empty(stack s) {
    return s->item.empty();
}

void pop(stack s) {
    s->item.pop_back();
}

string top(stack s) {
    return s->item.back();
}

void push(stack s, string item) {
    s->item.push_back(item);
}

void printStack(stack s) {
    if (empty(s)) 
        return;
    string x = top(s);
    cout << x << ' ';
    pop(s);
    printStack(s);
    push(s,x);
}

void printStack_fromBottom(stack s) {
    if (empty(s)) 
        return;
    string x = top(s);
    pop(s);
    printStack(s);
    cout << x << ' ';
    push(s, x);
}

int main () {
    // stack initialization using range-based for
    // int list[] = {1, 2, 3, 4, 5, 0, 6, 0, 0, 7, 0, 0, 0, 8};
    string list[] = {"to", "be", "or", "not", "to", "-", "be", \
                    "-", "-", "that", "-", "-", "-", "is"};
    stack s = new Stack;

    for (auto item : list) {
        if (item != "-"){
            push(s, item);
            DPRINT(cout << "size: " << size(s) << " capacity: " << capacity(s) << "\n");
        }
        else {
            cout << top(s) << ' ';
            pop(s);
        }
    }

    cout << "\nsize: " << size(s); // 2
    cout << "\ntop: " << top(s); // is (8)
    cout << "\nstack T: ";
    printStack(s);     // is to (8 1)
    cout << "\nstack B : " ;
    printStack_fromBottom(s);      // to is (1 8)
    cout << "\nHappy Coding";

    free(s);

}
