#include <iostream>
using namespace std;
#define HSIZE 50

//데이타구조 Programming Assignment 12
//22100579 이진주

/*
Heap 구현 : 과제 11의 결과를 확장하여, 다음 내용이 수행되도록 프로그램을 완성하시오.

<예상 출력>
Number of nodes : 6
Score Sum = 516
Score average = 86
-- After the deletion operation. --
Number of nodes : 5
Score Sum = 428
Score average = 85.6
Park : 98
Ryu : 94
Cho : 85
Lee : 77
Choi : 74
*/

class element{
    public:
    //data
    string name;
    double score; //key..?

    //method
    void set_data(string newName, int newScore); //노드의 데이타 설정 
};

class my_heap{
    element h[HSIZE]; //link list형식이 아닌 array형식..!이므로 root 포인터 이런거 필요 없
    int csize; //number of nodes

    public:
    my_heap(element a[], int size); //constuctor -여기서 힙 구성도? 시켜야하나? 
    void insert_heap(element newNode); //적절한 위치로 node 삽입 
    int h_size(); //현재시점의 node갯수 반환 
    element delete_heap(); //최고값의..? node 삭제하고 반환 
    bool isFull();
    bool isEmpty();
    void swap(element* a, element* b);

    //new
    double score_sum(); //score의 총합 반환
    double score_average(); //score의 평균 반환
    void node_delete_by_name(string keyName); //입려받은 name의 node를 삭제
};

void printHeap(my_heap h1);



int main(){

   element   a[10] = {{"Kim",88}, {"Lee", 77}, {"Park", 98}, {"Choi", 74}, {"Ryu",94}, {"Cho", 85}};
   my_heap   h1(a, 6);

   cout << "Number of nodes : " << h1.h_size() << endl;
   cout << "Score Sum = " << h1.score_sum() << endl;
   cout << "Score average = " << h1.score_average() << endl;
   
   /*
   printf("$$origin heap\n");
   printHeap(h1);
   h1.delete_heap();
   printf("$$heap-1\n");
   printHeap(h1);
   h1.delete_heap();
   printf("$$heap-2\n");
   printHeap(h1);
   */

   h1.node_delete_by_name("Kim");

   cout << "\n-- After the deletion operation. --\n\n";
   cout << "Number of nodes : " << h1.h_size() << endl;
   cout << "Score Sum = " << h1.score_sum() << endl;
   cout << "Score average = " << h1.score_average() << endl;

   while (h1.h_size() > 0) {
        element temp = h1.delete_heap();
        cout << temp.name << " : " << temp.score << "\n";
   }

   return 0;
}

//현재 힙 상태 출력 
void printHeap(my_heap h1){
    while (h1.h_size() > 0) {
        element temp = h1.delete_heap();
        cout << temp.name << " : " << temp.score << "\n";
   }
   printf("----------------------\n\n");
}




//constuctor
my_heap:: my_heap(element a[], int size){
    //여기서 무지성으로 넣는게 아니라 insert문을 활용해서 차례로 넣는게 맞는듯
    csize = 0;
    for(int i = 0; i < size; i++){ //array copy
        this->insert_heap(a[i]);
        //cout << a[i].name << " added.\n";//num of nodes is " << csize << endl;
    }
    
} 

//적절한 위치로 node 삽입 
void my_heap:: insert_heap(element newNode){

    //cout << "new node added\n";

    if(isFull()){
        cout << "heap is full :(\n";
        return;
    }

    //일단 제일 끝 노드에 넣어주고, 이때 k는 csize
    int k = csize+1; //1번 인덱스부터 시작하고싶으니까 이래 처리하면 되려나? 본인 인덱스임 
    h[k] = newNode;
    csize ++; 

    //아니면... 끝없는 배틀이 시작된다... 부모한테 계속 도전장 내밀거임 (k번과 k/2번의 웅장한 싸움)
    while(1){
        if(k == 1) return; //루트까지 가면 걍 탈출 
        //본인이 더 크다? 부모랑 스왑합니당 k값도 조정해주고
        if(h[k].score > h[k/2].score){
            swap(&(h[k]), &(h[k/2]));
            k = k/2;
        }else return; //부모가 더 크다? 그자리에 만족하고 리턴. 
    }

} 

//현재시점의 node갯수 반환 
int my_heap:: h_size(){
    //cout << csize <<endl;
    return csize;
} 

//최고값의..? node 삭제하고 반환 
element my_heap:: delete_heap(){

    //cout << "node deleted\n";

    if(isEmpty()){
        cout << "heap is empty :(\n";
        element dumy;
        dumy.set_data("NONE", 0);
        return dumy;
    }


    //리턴용으로 root노드를 어따 보관하고
    element tmp = h[1];
    //삭제될 그곳의 친구를 root로 이동시키고
    h[1] = h[csize];
    //아래의 끝없는 도전을 받아들이기...
    int k = 1; //루트부터 제자리 찾아 내려가는 여정, #가보자고 
    int c; //챌린저...! 
    csize --; 

    while(1){
        
        //일단 (자식이 둘이면)자식들끼리 예선전을 벌여서 이긴놈만이 도전의 자격을 가짐
        if(k*2+1 <= csize) c = (h[k*2].score > h[k*2+1].score)? (k*2): (k*2+1);
        //자식 하나 있으면 그놈을 도전자로 지정 
        else if(k*2 <= csize) c = k*2;
        //더 도전할 자식 없으믄 그자리에 땅땅땅
        else return tmp;  
        
        //도전자 선정을 마쳤다면 비교ㄱㄱ, 본인이 작다? 스왑
        if(h[k].score < h[c].score) swap(&(h[k]), &(h[c]));
        //본인이 크다? 그자리가 천직인거임. 리턴.
        else return tmp;

    }
    return tmp;

} 

bool my_heap:: isFull(){
    if(csize >= HSIZE-1 ) return true;
    return false;
}

bool my_heap:: isEmpty(){
    if(csize == 0) return true;
    return false;
}


//둘이 자리바꾸는거 메서드로 처리하면 편하지 않으까? 아님말구 
void my_heap:: swap(element* a, element* b){

    element tmp = *a;
    *a = *b;
    *b = tmp;
}

//----------------------------------------------------------

//score의 총합 반환
double my_heap:: score_sum(){

    double sum = 0;
    for(int i = 1; i < csize+1; i++){
        sum += h[i].score;
        //cout << h[i].name << ", ";
    }
    //cout << "'s added to sum.\n";
    return sum;

}

//score의 평균 반환
double my_heap:: score_average(){
    return score_sum()/csize;
} 

//입려받은 name의 node를 삭제
void my_heap:: node_delete_by_name(string keyName){
    int i;
    for(i = 1; i < csize; i++)
        if(keyName == h[i].name) break;
    //루프 탈출 시점에서 i는 동일한 이름의..거시기의 인덱스이길 희망함
    if(i >= csize){ //아님말구 
        cout << "cannot find the name";
        return;
    }else{ //i를 root로 하는 delete ㄱㄱ

        if(isEmpty()){
            cout << "heap is empty :(\n";
            return;
        }else{
            //cout << h[i].name << " gonna be deleted.\n";
        }


        //젤 꼬랑지 친구를 i로 이동시키고
        h[i] = h[csize];
        csize --; 
        //아래의 끝없는 도전을 받아들이기...
        int k = i; //i부터 제자리 찾아 내려가는 여정, #가보자고 
        int c; //챌린저...! 
        

        while(1){
            
            //일단 (자식이 둘이면)자식들끼리 예선전을 벌여서 이긴놈만이 도전의 자격을 가짐
            if(k*2+1 <= csize) c = (h[k*2].score > h[k*2+1].score)? (k*2): (k*2+1);
            //자식 하나 있으면 그놈을 도전자로 지정 
            else if(k*2 <= csize) c = k*2;
            //더 도전할 자식 없으믄 그자리에 땅땅땅
            else return; 
            
            //도전자 선정을 마쳤다면 비교ㄱㄱ, 본인이 작다? 스왑
            if(h[k].score < h[c].score) {
                swap(&(h[k]), &(h[c]));
                //printHeap(*this);
                cout << h[k].name << ", " << h[c].name << " swaperd." << endl;
            }
            //본인이 크다? 그자리가 천직인거임. 리턴.
            else return;

        }
    }


} 


//----------------------------------------------------------


//노드의 데이타 설정 
void element:: set_data(string newName, int newScore){
    name = newName;
    score = newScore;
} 


