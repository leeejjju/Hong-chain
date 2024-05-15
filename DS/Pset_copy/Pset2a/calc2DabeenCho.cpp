// On my honor, I pledge that I have neither received nor provided improper assistance in the completion of this assignment. 
// Signed: 조다빈 Student Number: 22000711

#include <iostream>
#include <sstream>
using namespace std;

int add(int a, int b) { return a + b; }
int mul(int a, int b) { return a * b; }
int sub(int a, int b) { return a - b; }
int dvd(int a, int b) { if (b != 0) return a / b; else return 0; }

int get_int(){
    int x;
    do {
        cout << "Enter an integer: ";
        string str;
        getline(cin, str);
        try {
            x = stoi(str);
            break;
        }
        catch (invalid_argument& e) {
            cerr << e.what() << " error occurred. Retry~" << endl;
        }
    } while(true);
    return x;
}

pair<char, int> get_op(string opstr) {
    char op;                                // user's opertor entered
    size_t x;                               // index of op in opstr
    do {
        cout << "Enter an operator( " << opstr << " ): ";
        stringstream ss;
        string str;
        getline(cin, str);
        ss << str;
        ss >> op;           
        x = opstr.find(op);                 // find index of op in opstr
    } while(x == string::npos);             // while op is not found in opstr
    return make_pair(op, x);                // returns an operator and its index
}

int main() {
    int (*fp[])(int, int) = {add, sub, mul, dvd};        // array of function pointers
    string opstr { "+-*/" };                             // operators in string

    int a { get_int() };                      // initialize a with user's input
    pair<char, int> op { get_op(opstr) };     // get an operator and its index
    int b { get_int() };                      // initialize b with user's input

    cout << a << " " << op.first << " " << b << " = " << fp[op.second](a, b) << endl;
    return 0;
}
