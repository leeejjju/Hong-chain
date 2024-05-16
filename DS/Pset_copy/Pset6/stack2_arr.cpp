#include <iostream>
using namespace std;

struct Stack {
    string *item;
    int N;
    int capacity;
};

using stack = Stack *;

stack newStack(int capacity) {
    stack s = new Stack;
    s->item = new string[capacity];
    s->N = 0;
    s->capacity = capacity;
    return s;
}

void free(stack s) {
    delete[] s->item;
    delete s;   // stack이 구조체 Stack의 포인터라 new를 사용한 동적 메모리 할당을 하기 때문에 (using stack = Stack *)
}

int size(stack s) {
    return s->N;
}

bool empty(stack s) {
    return s->N == 0;   // stack의 index도 0 부터 시작
}

void pop(stack s) {
    s->N--;
}

string top(stack s) {
    return s->item[s->N - 1];
}

void push(stack s, string item) {
    s->item[s->N++] = item;
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
    stack s = newStack(sizeof(list) / sizeof(list[0]));

    for (auto item : list) {
        if (item != "-")
            push(s, item);
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





