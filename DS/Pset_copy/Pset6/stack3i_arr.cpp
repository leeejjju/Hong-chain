// On my honor, I pledge that I have neither received nor provided improper assistance in the completion of this assignment. 
// Signed: 조다빈 Student Number: 22000711

#include <iostream>
using namespace std;

#ifdef DEBUG
#define DPRINT(func) func;
#else
#define DPRINT(func) ;
#endif


struct Stack {
    int *item;
    int N;
    int capacity;
};

using stack = Stack *;
void resize(stack s, int new_capacity);     // prototype of resize()

stack newStack(int capacity = 1) {
    stack s = new Stack;
    s->item = new int[capacity];
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
    if (s->N <= s->capacity / 4)
        resize(s, s->capacity / 2);
}

int top(stack s) {
    return s->item[s->N - 1];
}

void push(stack s, int item) {
    if (s->N >= s->capacity)
        resize(s, s->capacity * 2);
    s->item[s->N++] = item;
}

void printStack(stack s) {
    if (empty(s)) 
        return;
    int x = top(s);
    cout << x << ' ';
    pop(s);
    printStack(s);
    push(s,x);
}

void printStack_fromBottom(stack s) {
    if (empty(s)) 
        return;
    int x = top(s);
    pop(s);
    printStack(s);
    cout << x << ' ';
    push(s, x);
}

void resize(stack s, int new_capacity) {
    int *copied = new int[new_capacity];
    for (int i = 0; i < s->N; i++) 
        copied[i] = s->item[i];
    delete[] s->item;
    s->item = copied;
    s->capacity = new_capacity;
} 


int main () {
    // stack initialization using range-based for
    int list[] = {1, 2, 3, 4, 5, 0, 6, 0, 0, 7, 0, 0, 0, 8};
    // list[] = {"to", "be", "or", "not", "to", "-", "be", \
                    "-", "-", "that", "-", "-", "-", "is"};
    stack s = newStack();

    for (auto item : list) {
        if (item != 0){
            push(s, item);
            DPRINT(cout << "size: " << s->N << " capacity: " << s->capacity << "\n");
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