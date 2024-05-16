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

template<typename T>
struct Stack {
    vector<T> item;
};

template<typename T>
using stack = Stack<T> *;

template<typename T>
void free(stack<T> s) {
    delete s;
}

template<typename T>
int size(stack<T> s) {
    return s->item.size();
}

template<typename T>
int capacity(stack<T> s) {
    return s->item.capacity();
}

template<typename T>
bool empty(stack<T> s) {
    return s->item.empty();
}

template<typename T>
void pop(stack<T> s) {
    s->item.pop_back();
}

template<typename T>
T top(stack<T> s) {
    return s->item.back();
}

template<typename T>
void push(stack<T> s, T item) { 
    s->item.push_back(item); 
}

template<typename T>
void printStack(stack<T> s) {
    if (empty(s)) 
        return;
    T x = top(s);
    cout << x << ' ';
    pop(s);
    printStack(s);
    push(s,x);
}

template<typename T>
void printStack_fromBottom(stack<T> s) {
    if (empty(s)) 
        return;
    T x = top(s);
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
    stack<string> s = new Stack<string>;

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
