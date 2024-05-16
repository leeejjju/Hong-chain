#include <iostream>
using namespace std;
#define SIZE 50


//데이타구조 HW06
//22100579 이진주
/*주어진 string이 Palindrome(좌우 대칭)인지 여부를 판단하는 프로그램 (stack을 사용하여 구현)
영문자만 고려,두 단어 이상의 string에서도 가능하도록, 띄어쓰기(space)는 무시
ex) 'race car' 도 palindrome으로 판단*/

/*
1. 문자열 가공하고 사이즈 반환하는 메서드 구현.
2. parindrom인지 아닌지 판별하는 메서드. 
3. fgets로 띄어쓰기 포함한 문자열 하나 받고, 메서드 써서 가공하기. 부산물로 나온 사이즈 정수 저장하고
4. 가공된 문자열이랑 사이즈 함수에 보내서 결과 받아온거에 맞춰 결과 메세지 출력되도록 ㄱㄱ 
*/


class mystack{
    private:
    char s[SIZE]; //스택 공간 확보
    char top = 0; //다음에 >>들어와야 할<< 공간의 인덱스!! 

    public:
    void push(char a); //하나 밀어넣기
    char pop(); //하나 빼기
    char show(); 
    bool stack_empty(); //비어있는지 체크
    bool stack_full(); //꽉차있는지 체크 
};

bool isAlpha(char x);
int makeStringSimple(string* a); //문자열을 알파벳만 남겨 가공하고 가공된 사이즈 반환 
bool isPalindrom(string a, int size); //스택을 사용하여 입력받은 문자열이 palindrom인지 아닌지 판단 



int main(){

    char x; //token
    string input;
    char newChar[50];

    //한 줄 입력받아 string 형태로 저장 
    cout << "input your string >> ";
    fgets(newChar, 50, stdin);
    input = string(newChar);

    //비교를 위해 알파벳만 남긴 상태로 가공하고 
    int size = makeStringSimple(&input);

    //palindrom인지 판별
    if(isPalindrom(input, size)) cout << "Yes, it's a palindrome.";
    else cout << "No, It's not a palindrom.";

    return 0;
}


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

bool mystack:: stack_empty(){
    if(top == 0) return true;
    else return false;
} //비어있는지 체크

bool mystack:: stack_full(){
    if(top == SIZE - 1) return true;
    else return false;
} //꽉차있는지 체크 


char mystack:: show(){
  return s[top - 1];
} //top원소 보여주기  



//입력받은 char 형식 token이 알파벳인지 아닌지 판단 
bool isAlpha(char x){
    if(x >= 'a' && x <= 'z') return true;
    else if(x >= 'A' && x <= 'Z') return true;
    else return false;

}

//문자열을 알파벳만 남겨 가공하고 가공된 사이즈 반환 
int makeStringSimple(string* a){

    string output = "";
    int count = 0;

    for(int i = 0; i < (*a).size(); i++){
        
        if(isAlpha((*a)[i])){
            output += (*a)[i];
            count ++;
        } 
    }

    (*a) = output;
    return count; 

} 

//스택을 사용하여 입력받은 문자열이 palindrom인지 아닌지 판단 
bool isPalindrom(string a, int size){

    mystack  s; //스택 하나 만들고

    //문자열 반 갈라서 앞쪽만 스택에 밀어넣기
    for(int i = 0; i < a.size()/2; i++){
        s.push(a[i]);
        //printf("%c가 스택에 들어감\n", a[i]);
    }

    //나머지 반이랑 스택에서 뺀 애들이랑 비교해서 parindrom 판별 
    for(int i = (a.size()+1)/2; i < a.size(); i++){
        //printf("%c와 %c을(를) 비교함\n", s.show(), a[i]);
        if(s.pop() != a[i]) return false;
    }

    return true;
} 