#include <iostream>
using namespace std;
#define SIZE 50


//데이타구조 HW05
//22100579 이진주
/*세가지 유형의 괄호,  ( ),  { }, [ ] 를 사용하는 수식 표현에서,
괄호 사용의 오류를 점검하는 프로그램을 작성하시오.

예) 
(a + b) - [ c * {x - (a / b) } ]   ==> 정상
(a + b) - [ c * {x - (a / b} } ] ==> 괄호 불일치 ( }
(a + b) - [ c * {x - (a / b) }  ==> 닫는 괄호 누락*/



class mystack{
    private:
    char s[SIZE]; //스택 공간 확보
    char top = 0; //다음에 >>들어와야 할<< 공간의 인덱스!! 

    public:
    void push(char a); //하나 밀어넣기
    char pop(char in); //하나 빼기
    char show(); 
    bool stack_empty(); //비어있는지 체크
    bool stack_full(); //꽉차있는지 체크 
};

int isBracket(char x); //괄호인지, 여는괄호인지 닫는 괄호인지
bool isMatched(char x, char y); //두개의 char 형식 token이 매치되는 괄호인지 검사 
char findExpextedOne(char x); //누락된, 필요한 나머지 괄호 반환


int main(){

    string input;
    int i;
    char x; //token
    mystack s;

    //한 줄 입력받아 저장 
    cout << "input your string >> ";
    cin >> input; 
    

    for (i = 0; i < input.size(); i++ ){ //스택에 배열 속 원소 차례대로 밀어넣기 

      if(isBracket(input[i]) == 1){ //여는괄호는 스택에 넣기 
        s.push(input[i]); 
          
      }else if(isBracket(input[i]) == 2){ //닫는괄호 만나면 매치시도 
        
        if(isMatched(s.show(), input[i])){ //성공시 걍 넘어가고
          s.pop(input[i]);
        }else{ //실패하면 짝 안맞는거... 에러 출력 
          char temp = s.pop(input[i]);
          if(temp == 'N') return 0;
          else {
            printf("Error: mis-matched parenthesis, '%c' is expected.", findExpextedOne(temp));
          }
          return 0;
        }
      }
    }
  

    while ( ! s.stack_empty()){ //스택에 남은거 있남. 있으면 에러 출력     
      printf("Error: An extra parenthesis '%c' is founded.", s.pop(input[i]));
      return 0;
    }
  //(i >= input.size()) ? 'a' : input[i]
        

    //여기까지 문제 없이 왔으면 성공 출력 
    cout << "It's a normal expression.";

    return 0;
}


//하나 밀어넣기
void mystack:: push(char a){ 
    if(!stack_full()){ //꽉차있지 않다면 
        s[top] = a; //현재 대기중인 공간에 인자로 받은 원소 밀어넣기 
        top++; //대기인덱스 한칸 위로 
    }else{
        cout<<"stack is full :(\n";
    }

} 

//하나 뾱하고 빼기
char mystack:: pop(char in){
    if(!stack_empty()){ //비어있지 않다면 
        top--; //대기인덱스 한칸 뒤로 물려서 젤 최근에 들어온놈을 가리키게 하고 
        return s[top]; //그놈 뱉기
    }else{
        printf("Error: An extra parenthesis '%c' is founded.", in);
        return 'N';
    }
  
} 

//비어있는지 체크
bool mystack:: stack_empty(){
    if(top == 0) return true;
    else return false;
} 

//꽉차있는지 체크 
bool mystack:: stack_full(){
    if(top == SIZE - 1) return true;
    else return false;
} 

//top원소 보여주기  
char mystack:: show(){
  return s[top - 1];
} 


//괄호인지, 여는괄호인지 닫는 괄호인지
int isBracket(char x){ 

    if(x == '(' || x == '{' || x == '[') return 1;
    else if(x == ')' || x == '}' || x == ']') return 2;
    else return 0;

}

//두개의 char 형식 token이 매치되는 괄호인지 검사 
bool isMatched(char x, char y){

    if((x == '(' && y == ')')||(x == ')' && y == '('))return true;
    else if((x == '{' && y == '}')||(x == '{' && y == '}'))return true;
    else if((x == '[' && y == ']')||(x == '[' && y == ']'))return true;
    else return false;

}

//누락된, 필요한 나머지 괄호 반환
char findExpextedOne(char x){
  if(x == '(') return ')';
  else if(x == ')') return '(';
  else if(x == '{') return '}';
  else if(x == '}') return '{';
  else if(x == '[') return ']';
  else if(x == ']') return '[';
  return 'N';
}


