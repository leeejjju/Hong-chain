#include <iostream>
#include <stack>
using namespace std;

void printStack(stack<string> s) {
        // brute-force version
        /*
        stack<string> t;
        while (!s.empty()) {
            cout << s.top() << ' ';
            t.push(s.top());
            s.pop();
        }
       while (!t.empty()) {
        s.push(t.top());
        t.pop();
    }*/

    // recursion version
    if (s.empty()) 
        return;
    string x = s.top();
    cout << x << ' ';
    s.pop();
    printStack(s);
    s.push(x);
}   

void printStack_fromBottom(stack<string> s) {
    // brute-force version
    /*
    stack<string> t;
    while (!s.empty()) {
        t.push(s.top());
        s.pop();
    }
    while (!t.empty()) {
        cout << t.top() << ' ';
        t.pop();
    }
    */
   
   // recursion version
    if (s.empty()) 
        return;
    string x = s.top();
    s.pop();
    printStack(s);
    cout << x << ' ';
    s.push(x);
}

int main () {
    // stack initialization using range-based for
    // int list[] = {1, 2, 3, 4, 5, 0, 6, 0, 0, 7, 0, 0, 0, 8};
    string list[] = {"to", "be", "or", "not", "to", "-", "be", \
                    "-", "-", "that", "-", "-", "-", "is"};
    
    stack<string> s;
    for (auto item : list) {
        if (item != "-")
            s.push(item);
        else {
            cout << s.top() << ' ';
            s.pop();
        }
    }

    cout << "\nsize: " << s.size(); // 2
    cout << "\ntop: " << s.top(); // is (8)
    cout << "\nstack T: ";
    printStack(s);     // is to (8 1)
    cout << "\nstack B : " ;
    printStack_fromBottom(s);      // to is (1 8)
    cout << "\nHappy Coding";
}