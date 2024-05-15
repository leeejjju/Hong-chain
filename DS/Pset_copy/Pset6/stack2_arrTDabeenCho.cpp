// On my honor, I pledge that I have neither received nor provided improper assistance in the completion of this assignment. 
// Signed: 조다빈 Student Number: 22000711

#include <iostream>
using namespace std;

template<typename T>
struct Stack {
    T *item;
    int N;
    int capacity;
};

template<typename T>
using stack = Stack<T> *;

template<typename T>
stack<T> newStack(int capacity) {
    stack<T> s = new Stack<T>;
    s->item = new T[capacity];
    s->N = 0;
    s->capacity = capacity;
    return s;
}

template<typename T>
void free(stack<T> s) {
    delete[] s->item;
    delete s;   // stack이 구조체 Stack의 포인터라 new를 사용한 동적 메모리 할당을 하기 때문에 (using stack = Stack *)
}

template<typename T>
int size(stack<T> s) {
    return s->N;
}

template<typename T>
bool empty(stack<T> s) {
    return s->N == 0;   // stack의 index도 0 부터 시작
}

template<typename T>
void pop(stack<T> s) {
    s->N--;
}

template<typename T>
T top(stack<T> s) {
    return s->item[s->N - 1];
}

template<typename T>
void push(stack<T> s, T item) {
    s->item[s->N++] = item;
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
    stack<string> s = newStack<string>(sizeof(list) / sizeof(list[0]));

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

