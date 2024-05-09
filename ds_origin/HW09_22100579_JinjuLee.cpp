#include <iostream>
using namespace std;
//#define JUMGUM printf("%d단계 점검\n", jum++);
#define JUMGUM jum++;


//데이타구조 HW09
//22100579 이진주
/*
강의에서 설명한 구조의 linked list를 구현하고, 아래의 main function의 의도대로 동작하도록
관련 function을 작성하시오. (강의 교재 내용 참조)
*/



class node{

    public:
    //data
    string name; 
    double score;
    //link
    node* link;

    void set_data(string newName, double newScore){
        name = newName;
        score = newScore;
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
    double score_sum(); //현재 node들의 점수의 합
    double get_score(string name); //입력된 name의 node의 점수 반환
    int remove_a_node(string name); //입력된 name의 node 삭제 

};



int main(){

   my_list a; //linked list 선언 
   node   tmp; //수행을 위한 node변수 
    int jum = 1;

    // head 위치로 2개의 원소 추가
    tmp.set_data("Kim", 83.5);
    a.add_to_head(tmp);
    tmp.set_data("Lee", 78.2);
    a.add_to_head(tmp);           

    cout << a.num_nodes() << " : " << a.score_sum() << "\n";  // 1단계 점검
    JUMGUM

    // tail 위치로 1개의 원소 추가
    tmp.set_data("Park", 91.3);   
    a.add_to_tail(tmp);

    cout << a.num_nodes() << " : " << a.score_sum() << "\n";  //2단계 점검
    JUMGUM

    // head 위치에서 원소 하나 삭제 
    tmp = a.delete_from_head();
    cout  << tmp.name << " is deleted.\n";   // 3단계 점검
    JUMGUM

    // tail과 head에 원소 하나씩 추가 
    tmp.set_data("Choi", 85.1);   
    a.add_to_tail(tmp);              
    tmp.set_data("Ryu", 94.3);   
    a.add_to_head(tmp);            

    cout << a.num_nodes()<< " : " << a.score_sum() << "\n"; // 4단계 점검
    cout << "Park's score : " << a.get_score("Park")<< "\n";  
    JUMGUM

    if ( a.remove_a_node("Kim") == 1) cout << "Kim is deleted from the list. \n";     // 5단계 점검
    JUMGUM

    cout << a.num_nodes()<< " : " << a.score_sum() << "\n";  // 최종 점검       
    JUMGUM

    return 0;
  }


// ----------------------------------------------------------------------------

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

    //현재 node들의 점수의 합
    double my_list:: score_sum(){
        
        node* t; //i같은 역할
        double sum = 0; //점수 누적할 공간 

        for(t = head; t != NULL; t = t->link){
            //printf("%.1f에 %.1f가 더해짐\n", sum, t->score);
            sum += t->score;
        }

        return sum;

    }

    //입력된 name의 node의 점수 반환
    double my_list:: get_score(string name){

        node* t; //i같은 역할
        double score; //찾은 점수를 저장해둘 공간
        
        for(t = head; t != NULL; t = t->link){
            if(name == t->name){
                score = t->score;
                break;
            }
        }

        return score;
    }

    //입력된 name의 node 삭제, 삭제 성공했으면 1, 실패했으면 0 반환 
    int my_list:: remove_a_node(string name){

        node *t, *p; // t는 건너건너갈 노드포인터, p는 직전 포인터 
   
        for(t = head; (t->name != name)&&(t != NULL); t = t->link){ //head부터, name 일치하거나 null 만나기 전까지 타고타고 이동 
            p = t;
        }

        //for문이 break된 시점에서 (성공적으로 찾았다면) t는 해당 name의 node를 담고있고 p는 그 직전 node를 담고있을 것 
        if(t == NULL) return 0; 
        else{
            p->link = t->link; //이전노드에 현재노드가 가리키던 다음노드 포인터를 연결해주고
            delete(t); //잘 담았으니 삭제 
            return 1; //성공 ^0^*
        }

    }