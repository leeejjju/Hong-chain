// On my honor, I pledge that I have neither received nor provided improper assistance in the completion of this assignment.
// Signed: 조다빈 Student Number: 22000711

// postfix.cpp :
//
// The program evaluates a given postfix expression and produces a fully
// parenthesized infix expression. If the postfix expression may be
// evaluated to a numeric value if it is expressed using numerically.
// For simplicity of coding, however, the postfix expression consists
// of single character operands and operators only and may have spaces.
//
// Author: idebtor@gmail.com
// 2020.04.05    Creation
// 2020.10.03    evaluation of numerical postfix expressions
//
#include <iostream>
#include <stack>
#include <cassert>
#include <string>
using namespace std;

#ifdef DEBUG
#define DPRINT(func) func;
#else
#define DPRINT(func) ;
#endif

// change the printStack to use template once you finish part 1 in step 1.
// prints the stack contents from the bottom.
template<typename T>
void printStack(stack<T> orig) {
    stack<T> temp;
    while (!orig.empty()) {
        temp.push(orig.top());
        orig.pop();
    }
    while (!temp.empty()) {
        cout << temp.top() << ' ';
        orig.push(temp.top());
        temp.pop();
    }
}


// returns a fully parenthesized infix expression in string after postfix evaluation.
string evaluate(string tokens) {
    stack<string> st;

    for (char token : tokens) {
        if (isspace(token))
            continue;  // if token is a whitespace, skip it.
        DPRINT(cout << "token: " << token << endl;);

        // current token is a operator,
        // pop two operands and evaluate them and push the result to st.
        if (token == '+' || token == '-' || token == '*' || token == '/') {
            assert (st.size() >= 2);
            string right = st.top();
            st.pop();
            string left = st.top();
            st.pop();
            st.push("(" + left + ' ' + token + ' ' + right + ")");
        }
        else {  // push the operand
            DPRINT(cout << "  push: " << token << endl;);
            // convert token(char type) to a string type and push it to the stack
            string s;
            s = token;
            st.push(s);
        }
    }

    DPRINT(cout << "token exhausted: \n";);
    DPRINT(printStack(st););
    DPRINT(cout << "stack size() should be 1.\n";);

    assert (st.size() == 1);
    string expr = st.top();
    st.pop();
    
    DPRINT(cout << "<evaluate() returns " << expr << endl;);
    return expr;
}

// returns true if the tokens consist of '+', '-', '*', '/', spaces, and
// digits (0 ~ 9), false otherwise.
bool is_numeric(string tokens) {
    for (char token : tokens) {
        if (!isdigit(token) && !isspace(token) && token != '+' && token != '-' && token != '*' && token != '/')
            return false;
    }
    return true;
}

// returns a numeric value after evaluating the postfix evaluation.
double evaluate_numeric(string tokens) {
    stack<double> st;

    for (char token : tokens) {
        if (isspace(token))
            continue;  // if token is a whitespace, skip it.
        DPRINT(cout << "token: " << token << endl;);

        // if token is a operator, evaluate; if a digit(or operand), push it to st.
        if (token == '+' || token == '-' || token == '*' || token == '/') {
            // checking the stack top() for right operand
            double right = st.top();
            st.pop();
            // checking the stack top() for left oprand
            double left = st.top();
            st.pop();
            // compute the expression and push the result
            if (token == '+')
                st.push(left + right);
            else if (token == '-')
                st.push(left - right);
            else if (token == '*')
                st.push(left * right);
            else
                st.push(left / right);
        }
        else { // push the operand (digit) in a value to the stack
            // convert token to a numeric data type and push it the stack
            string s;
            s = token;
            double d = stod(s);
            st.push(d);
        }
    }

    DPRINT(cout << "token exhausted: check the stack, its size() should be 1.\n";);
    DPRINT(printStack(st););

    double expr = st.top();
    st.pop();

    DPRINT(cout << "<evaluate() returns " << expr << endl;);
    return expr;
}

