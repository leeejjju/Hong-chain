#include <iostream>
using namespace std;
#define SIZE 50
#define EOS '$'

//데이타구조 HW04
//22100579 이진주
/*Infix expression을 Postfix expression으로 변환하는 프로그램을 작성하시오.
test case 예)
입력 :  a*(b+c)
출력 :  abc+*  */


class mystack{
    private:
    char s[SIZE]; //스택 공간 확보
    char top; //다음에 >>들어와야 할<< 공간의 인덱스!! 

    public:
    void init(); //초기화
    void push(char a); //하나 밀어넣기
    char pop(); //하나 뾱하고 빼기
    char show(); //top원소 보여주기 
    bool stack_empty(); //비어있는지 체크
    bool stack_full(); //꽉차있는지 체크 
};

int getPreced(char x);
bool isAlpha(char x);


int main(){

    mystack  s; //스택 하나 만들고 
    int i;
    char x; //token
    string input, output="";
    s.init(); //만든 스택 초기화?
    s.push(EOS);

    cout << "input your infix formula >> ";
    cin >> input; 

    for (i = 0; i < input.size(); i++ ){ //스택에 배열 속 원소 차례대로 밀어넣기 

      if(input[i] == '('){
        s.push(input[i]); //여는괄호는 스택에 넣기 
          
      }else if(input[i] == ')'){
        while(1){
          if (s.show() == '(') { //여는괄호 나오면 
            s.pop(); //그놈 버리고
            break; //루프 끗.
          }else {
            cout << s.pop(); //꺼내서 출력 
          }
        }
          
      }else if(input[i] >= 'a' && input[i] <= 'z'){
        cout << input[i]; //피연산자는 그대로 출력
          
      }else{ //연산자는? 
        while(1){
          if(getPreced(s.show()) >= getPreced(input[i])){//최상위원소 우선순위가 본인보다 높은 한 
            cout << s.pop(); //꺼내서 출력
          }else{//낮아지면?
            s.push(input[i]); //스택에 넣고
            break; //루프 종료 
          }
        }
      }
    }
  

    while ( ! s.stack_empty()){ //스택을 텅 빌때까지 하나씩 빼냄
      x = s.pop( ); //변수 x에 한놈 담아서
      if(x != '$') cout << x; //출력
        
    }

    return 0;
}


void mystack:: init(){
    top = 0;

} //초기화

void mystack:: push(char a){
    if(!stack_full()){ //꽉차있지 않다면 
        s[top] = a; //현재 대기중인 공간에 인자로 받은 원소 밀어넣기 
        top++; //대기인덱스 한칸 위로 
    }else{
        cout<<"stack is full :(\n"; //꽉찼서ㅠㅠ
    }

} //하나 밀어넣기

char mystack:: pop(){
    if(!stack_empty()){ //비어있지 않다면 
        top--; //대기인덱스 한칸 뒤로 물려서 젤 최근에 들어온놈을 가리키게 하고 
        return s[top]; //그놈 뱉기
    }else{
        cout<<"stack is empty :(\n"; //비엇서ㅠㅠ
    }

} //하나 뾱하고 빼기

char mystack:: show(){
  return s[top - 1];
} //top원소 보여주기  

bool mystack:: stack_empty(){
    if(top == 0) return true;
    else return false;
} //비어있는지 체크

bool mystack:: stack_full(){
    if(top == SIZE - 1) return true;
    else return false;
} //꽉차있는지 체크 



bool isAlpha(char x){
    if(x >= 'a' && x <= 'z') return true;
    else if(x >= 'A' && x <= 'Z') return true;
    else return false;

}

int getPreced(char x){
    if((x == '+') || (x == '+')) return 1;
    else if((x == '*') || (x == '/') || (x == '%')) return 2;
    else return 0; //$, (

} //우선순위 반환 