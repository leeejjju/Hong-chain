#include <iostream>
using namespace std;
#define SIZE 50
#define TEST printf("생존 %d번째\n ", ++s);

//데이타구조 HW07
//22100579 이진주

/*과제 03번 ~ 07번까지 본인에 제출한 과제 중 한 개를 선택하여,
Stack (또는 queue) 를 Linked List 를 사용하여 구현한 버전으로 수정(재작성)하여 제출
HW7: 
도서정보 = {도서명, 페이지수, 가격} 로 이루어진 원소를 저장하는 circular queue(->linked list)를 구현하고,
다음 main function 이 수행되도록 프로그램을 완성하시오.

< 예상 출력 >
    C++ Programming :  534 : 35000
    Data Structure :  621 : 42000
    JAVA Programming :  485 : 38000
    Logic Design : 532 : 44000
*/



//linked list의 node 
class node{

    public:
    //data
    string s_title; //도서명
    int npages; //페이지수
    int price; //가격

    //link
    node* link;

    void set_data(string inputTitle, int inputNPage, int inputPrice){
        s_title = inputTitle;
        npages = inputNPage;
        price = inputPrice;
    } 
};


class my_list{

    node* head; //첫 원소를 가리킬 node 포인터 타입
    node* tail; //마지막 원소를 가리킬 node 포인터 타입
    
    public:
    my_list(); //생성자, construnctor
    void add_to_head(node x); //head에 node 추가
    void add_to_tail(node x); //tail에 node 추가
    node delete_from_head(); //head에서 node 지우고 삭제된놈 반환   

    int num_nodes(); //현재 node의 갯수
    int remove_a_node(string s_title); //입력된 name의 node 삭제 
    void print_nodes(int size); //현재 연결된 노드들을 모두 출력 

};



int main(){
        int s = 0;
  
        my_list bookList; //노드를 연결할 linked list 선언 
        node tmp; //임시로 담을 원소 


        //책 정보들 담아주기 
        tmp.set_data("C++ Programming", 534, 35000);
        bookList.add_to_head(tmp);

        tmp.set_data("Data Structure", 621, 42000);
        bookList.add_to_tail(tmp);

        tmp.set_data("JAVA Programming", 485, 38000);
        bookList.add_to_tail(tmp);

        tmp.set_data("Logic Design", 532, 44000);
        bookList.add_to_tail(tmp);

        //담긴 노드들(책 정보들) 출력해주기 
        bookList.print_nodes(bookList.num_nodes()); 


        return 0;

}



    //생성자, construnctor
    my_list:: my_list(){
        head = NULL;
        tail = NULL;
    } 

    //head에 node 추가
    void my_list:: add_to_head(node x){

        node* t;
        t = new node; //새로운 node공간 만들어주고

        (*t) = x; //data 넣어주고

        t->link = head; //head가 원래 가리키던 놈을 link원소랑 연결시켜주고
        head = t; //head가 이놈을 가리키게 조정

        if(tail == NULL) tail = t; //비어있다가 들어온 첫 원소면 tail에도 추가 

    }

    //tail에 node 추가
    void my_list:: add_to_tail(node x){
        
        node* t;
        t = new node; //새로운 node공간 만들어주고

        (*t) = x; //data 넣어주고
        t->link = NULL;

        tail->link = t;//tail 가리키던...원래 마지막이었던 친구가 얘를 가리키게 연결시켜주고
        tail = t; //tail도 얘 가리키게 조정

        if(head == NULL) head = t; //비어있다가 들어온 첫 원소면 head에도 추가 

    }

    //head에서 node 지우고 삭제된놈 반환   
    node my_list:: delete_from_head(){

        node* t; //copy해둘 임시변수 선언 
        node temp;

        t = head; //잃어버리지 않도록 copy해두기 
        temp = (*head);

        head = t->link; //다음원소였던 친구가 head랑 연결되도록

        delete(t); //지워버리기~
        return temp; //복사해둔 아까 지운놈이 가지고있던 node 반환 

    }

    //현재 node의 갯수
    int my_list:: num_nodes(){
        node* t; //i같은 역할
        int count = 0; //숫자세기용~

        for(t = head; t != NULL; t = t->link){ //t가 head부터 시작해서 다음노드로 옮겨다니면서 null 만나기 전까지 카운트 누적 
            count ++;
        }

        return count;
    }


    //입력된 title의 node 삭제, 성공시 1, 실패시 0 반환 
    int my_list:: remove_a_node(string s_title){

        node *t, *p; // t는 건너건너갈 노드포인터, p는 break시점 직전의 포인터를 저장할 공간  
   
        for(t = head; (t->s_title != s_title)&&(t != NULL); t = t->link){ //head부터, name 일치하거나 null 만나기 전까지 타고타고 이동 
            p = t;
        }

        //for문이 break된 시점에서 (성공적으로 찾았다면) t는 해당 name의 node를 담고있고 p는 그 직전 node를 담고있을 것 
        if(t == NULL) return 0; //못찾았으면 null까지 닿았을테니 0 반환 
        else{
            p->link = t->link; //이전노드에 현재노드가 가리키던 다음노드 포인터를 연결해주고
            delete(t); //잘 담았으니 삭제 
            return 1; //성공 ^0^*
        }

    }

    //현재 연결된 노드들을 모두 출력 
    void my_list:: print_nodes(int size){

        node* t; //i같은 역할, 임시 포인터 변수 
        for(t = head; t != NULL; t = t->link){ //t가 head부터 시작해서 다음노드로 옮겨다니면서 null 만나기 전까지 카운트 누적 
            cout << t->s_title << " : " << t->npages  << " : " << t->price << "\n";
        }

    } 