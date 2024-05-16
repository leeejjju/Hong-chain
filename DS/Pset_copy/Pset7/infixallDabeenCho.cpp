// On my honor, I pledge that I have neither received nor provided improper assistance in the completion of this assignment.
// Signed: 조다빈 Student Number: 22000711

// infix.cpp :
//
// The program evaluates a given infix expression which is fully parenthesized.
// It uses Dijkstra's two-stack algorithm. For simplicity of coding, however,
// the expression is to be fully parenthesized.
// For example:
//   ((3 - 1 ) * 5) + 4
//   2 * ((34 - 4) * 2)
//   1 + (((12 * 2) / 4) - 1)
//
// Author: idebtor@gmail.com
//
#include <iostream>
#include <cassert>
#include <string>
#include <math.h>
using namespace std;

#ifdef DEBUG
#define DPRINT(func) func;
#else
#define DPRINT(func) ;
#endif

#if 0    //////////////////////////////////////////////////////////////////////
// set #if 1, if you want to use this stack using vector instead of STL stack.
// a basic stack functinality only provided for pedagogical purpose only.
#include <vector>
template <typename T>
struct stack {
    vector<T> item;

    bool empty() const {
        return item.empty();
    }
    auto size() const {
        return item.size();
    }
    void push(T const& data) {
        item.push_back(data);
    }
    void pop() {
        if (item.empty())
            throw out_of_range("stack<>::pop(): pop stack");
        item.pop_back();
    }
    T top() const {
        if (item.empty())
            throw out_of_range("stack<>::top(): top stack");
        return item.back();
    }
};
#else  /////////////////////////// using STL stack //////////////////////////
#include <stack>
#endif ///////////////////////////////////////////////////////////////////////

template <typename T>
void printStack(stack<T> orig) {

    // cout << "Step 2: your code here: copy printStack() template version from postfix.cpp\n";
    // cout << "Step 3: rewrite printStack() using recursion. refer to PDF file\n"
    if (orig.empty())
        return;
    T x = orig.top();
    orig.pop();
    printStack(orig);
    cout << x << ' ';
    orig.push(x);
}

// performs arithmetic operations.
double apply_op(double a, double b, char op) {
    switch (op) {
    case '+':
            return a + b;
    case '-':
            return a - b;
    case '*':
            return a * b;
    case '/':
            return a / b;
    case '^':
            return pow((int)a, (int)b);
    }
    cout << "Unsupported operator encountered: " << op << endl;
    return 0;
}

int precedence(char top, char thisOp) {
    if (top == '(')
        return 0;   // top == '('이면 무조건 push thisOp to op_stack
    if (top == '+' || top == '-') {
        if (thisOp == '+' || thisOp == '-')
            return 1;   // 우선순위가 같을 때
        else
            return 0;   // top이 thisOp 보다 우선순위가 낮을 때
    }
    else if (top == '*' || top == '/') {
        if (thisOp == '*' || thisOp == '/')
            return 1;   // 우선순위가 같을 때
        else if (thisOp == '^')
            return 0;   // top이 thisOp 보다 우선순위가 낮을 때
        else
            return 2;   // top이 thisOp 보다 우선순위가 높을 때
    }
    else {      // top == '^' 일 때
        if (thisOp == '^')
            return 1;
        else
            return 2;
    }
}

// there is a bug...
double compute(stack<double>& va_stack, stack<char>& op_stack) {
    double right  = va_stack.top();
    va_stack.pop();
    double left = va_stack.top();
    va_stack.pop();
    char op = op_stack.top();
    op_stack.pop();
    double value = apply_op(left, right, op);

    DPRINT(cout << " va/op_stack.pop: " << value << endl;);
    return value;
}

// returns value of expression after evaluation.
double evaluate(string tokens) {
    DPRINT(cout << ">evaluate: tokens=" << tokens << endl;);
    stack<double>  va_stack;              // stack to store operands or values
    stack<char> op_stack;                 // stack to store operators.
    double value = 0;
    int pdigit = 0;         // pdigit=1 이면 이전 인덱스가 숫자, 0이면 숫자 아님
    double ivalue = 0;
    string s;       // stod를 위해 사용

    for (size_t i = 0; i < tokens.length(); i++) {
        // token is a whitespace, skip it.
        if (isspace(tokens[i]))
            continue;
        DPRINT(cout << " tokens[" << i << "]=" << tokens[i] << endl;);
        
        // token is an opening brace, push to op_stack
        if (tokens[i] == '('){
            op_stack.push(tokens[i]);
            continue;
        }
        
        // current token is a value(or operand), push it to va_stack.
        if (isdigit(tokens[i])) {
            if (pdigit == 1) {          // 그 전 index 문자가 숫자일 때 = 여러자리 숫자일 때
                assert (!isspace(tokens[i - 1]) && "There should be no space between the numbers");      // 1 23 + 2 를 입력한 경우 123으로 인식하지 않고 경고문 출력
                s = tokens[i];
                ivalue = ivalue * 10 + stod(s);
                va_stack.pop();
                va_stack.push(ivalue);
            }
            else {  // 한 자리 숫자일 때
                ivalue = tokens[i] - '0';
                pdigit = 1;
                va_stack.push(ivalue);
                }
        }
        else if (tokens[i] == ')') { // compute it, push the result to va_stack
            while (op_stack.top() != '(')
            {
                assert (va_stack.size() >= 2);
                pdigit = 0;
                value = compute(va_stack, op_stack);
                va_stack.push(value);
            }
            op_stack.pop();
        }
        else { // token is an operator; push it to op_stack.
            pdigit = 0;
            char thisOp = tokens[i];
            while (op_stack.size() != 0 && precedence(op_stack.top(), thisOp) >= 1)
            {
                assert (va_stack.size() >= 2);
                value = compute(va_stack, op_stack);
                va_stack.push(value);
            }
            op_stack.push(tokens[i]);
        }
        
    }
    
    // 전체 식을 모두 분석 후, Op stack 에 남아있는 연산자들을 operand stack에 남아있는 피연산자들에 적용
    while (!op_stack.empty()) {
        assert (va_stack.size() >= 2);
        pdigit = 0;
        value = compute(va_stack, op_stack);
        va_stack.push(value);
    }

    DPRINT(cout << "tokens exhausted: now, check two stacks:" << endl;);
    DPRINT(printStack(va_stack);  cout << endl;);
    DPRINT(printStack(op_stack);  cout << endl;);

    // cout << "your code here: process if !op_stack.empty() \n";
    assert (op_stack.empty());
    // cout << "your code here: post-conditions\n";
    assert (va_stack.size() == 1);
    // cout << "your code here: return & clean-up\n";
    value = va_stack.top();
    va_stack.pop();

    return value;
}
