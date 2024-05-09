#include <iostream>
using namespace std;

//데이타구조 HW03
//22100579 이진주
/*정수 원소를 저장하는 stack을 class를 사용하여 구현하고,
이를 이용하여, 주어진  array의 정수 원소 n개 에서 100보다 큰 값을 역순으로 출력하는 프로그램을 작성하시오. 
(다음 main() 함수가 동작하도록 나머지 부분을 작성)*/

//선언?정의?먼저 해두기 
class mystack{
    private:
    int s[10]; //대충 열칸짜리 스택 공간 확보
    int top; //다음에 >>들어와야 할<< 공간의 인덱스!! 

    public:
    void init(); //초기화
    void push(int a); //하나 밀어넣기
    int pop(); //하나 뾱하고 빼기
    bool stack_empty(); //비어있는지 체크
    bool stack_full(); //꽉차있는지 체크 
};

 
int main(){

    mystack  s1; //스택 하나 만들고 
    int list[5] = { 32, 123, 27, 131, 242 }, i, x; //정수 배열이 할당된 리스트와 기타 변수 선언
    s1.init(); //만든 스택 초기화?

    for (i = 0; i < 5; i++ ){ //스택에 배열 속 원소 차례대로 밀어넣기 
        if (list[i]> 100) //다만, 100보다 큰 원소만
            s1.push( list[i] );
    }

    while ( ! s1.stack_empty()){ //뭔가 들어간 스택을 텅 빌때까지 하나씩 빼냄(후입선출)
        x = s1.pop( ); //변수 x에 한놈 담아서
        cout << x << endl ; //출력
    }

    return 0;

}


void mystack:: init(){
    top = 0;

} //초기화

void mystack:: push(int a){
    if(!stack_full()){ //꽉차있지 않다면 
        s[top] = a; //현재 대기중인 공간에 인자로 받은 원소 밀어넣기 
        top++; //대기인덱스 한칸 위로 
    }else{
        cout<<"stack is full :(\n"; //꽉찼서ㅠㅠ
    }

} //하나 밀어넣기

int mystack:: pop(){
    if(!stack_empty()){ //비어있지 않다면 
        top--; //대기인덱스 한칸 뒤로 물려서 젤 최근에 들어온놈을 가리키게 하고 
        return s[top]; //그놈 뱉기
    }else{
        cout<<"stack is empty :(\n"; //비엇서ㅠㅠ
    }

} //하나 뾱하고 빼기

bool mystack:: stack_empty(){
    if(top == 0) return true;
    else return false;
} //비어있는지 체크

bool mystack:: stack_full(){
    if(top == 10) return true;
    else return false;
} //꽉차있는지 체크 