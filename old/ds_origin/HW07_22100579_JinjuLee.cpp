#include <iostream>
using namespace std;
#define SIZE 50


//데이타구조 HW07
//22100579 이진주

/*Array를 사용한 Circular queue의 구현 예제:
도서정보 = {도서명, 페이지수, 가격} 로 이루어진 원소를 저장하는 circular queue를 구현하고,
다음 main function 이 수행되도록 프로그램을 완성하시오.

< 예상 출력 >
        C++ Programming :  534 : 35000
        Data Structure :  621 : 42000
        JAVA Programming :  485 : 38000
        Logic Design : 532 : 44000
*/

//큐에 타입으로 들어갈 정보 원소 
class element{

    public:
    string s_title; //도서명
    int npages; //페이지수
    int price; //가격

    //정보 설정하는 메서드
    void set_data(string inputTitle, int inputNPage, int inputPrice){
        s_title = inputTitle;
        npages = inputNPage;
        price = inputPrice;
    } 

};

//커스텀 큐~
class my_queue{

    element q[SIZE]; //element를 원소로 저장하는 circular queue 
    int rear; //다음 원소를 >>넣을<< 곳
    int front; //최근에 원소가 들어온 곳

    public:
    my_queue(); //초기화
    void insert_q(element x); //원소 넣기
    element delete_q(); //원소 빼기
    bool empty(); //empty 체크
    bool full(); //full 체크 

};



int main(){
        my_queue  a; //일단 리니어...의 리스트 
        element  tmp; //임시로 담을 원소

        tmp.set_data("C++ Programming", 534, 35000);
        a.insert_q(tmp);

        tmp.set_data("Data Structure", 621, 42000);
        a.insert_q(tmp);

        tmp.set_data("JAVA Programming", 485, 38000);
        a.insert_q(tmp);

        tmp.set_data("Logic Design", 532, 44000);
        a.insert_q(tmp);

        while (!a.empty()){
                tmp = a.delete_q(); //tmp에다가 queue에 들어있는 원소 하나씩 빼서 담고 출력
                cout << tmp.s_title << " : " << tmp.npages  << " : " << tmp.price << "\n";
        }

        return 0;

}


//constructor, 생성자로 초기화 
my_queue:: my_queue(){

    rear = 0;
    front = 0;

}


//원소 넣기
void my_queue:: insert_q(element x){

    if(full()){ //꽉찼는지 체크 
        printf("queue is full :(\n");
        return;
    }

    q[rear] = x; //원소 집어넣고
    rear = (rear+1)%SIZE; //rear인덱스 증가시키되... 원형으로 돌고 돌 수 있도록 처리. 

}


//원소 빼기
element my_queue:: delete_q(){

    if(empty()){ //비었는지 체크 
        printf("queue is empty :(\n");
        return element(); //워라도 내놔야하니까 아무거나... 
    }

    front = (front+1)%SIZE; //하나 증가해서 rear쫓아가기  

    return q[(front-1)%SIZE]; //아맞다 아까 그위치에있던거 반환

}


//empty 체크
bool my_queue:: empty(){

    if(rear == front) return true;
    else return false;

} 


//full 체크 
bool my_queue:: full(){

    if((rear+1)%SIZE == front) return true; //rear의 바로 직전에 도달하믄 full.... 
    else return false;

}

