#include <iostream>
using namespace std;
#define HSIZE 50

//????????? Programming Assignment 11 
//22100579 ??????
/*

Heap ???? ????: 
{????, ????}?? ????? ??????? heap ????, ?????? Key ?????? ????
???? main ????? ????? ???? ??????? ???��???? ???????. 

<???? ??? > //sorting?? ??????? ??? 
Park : 98
Ryu : 94
Kim : 88
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
    void set_data(string newName, int newScore); //????? ????? ???? 

};

class my_heap{
    element h[HSIZE]; //link list?????? ??? array????..!???? root ?????? ????? ??? ?????? ???
    int csize; //number of nodes

    public:
    my_heap(); //constuctor
    void insert_heap(element newNode); //?????? ????? node ???? 
    int h_size(); //????????? node???? ??? 
    element delete_heap(); //???????..? node ??????? ??? 
    bool isFull();
    bool isEmpty();
    void swap(element* a, element* b); //???? ??????��? ?????? ?????? ?????? ??????? ?????? 
};





int main(){

   my_heap   h1; //heap ????
   element   temp; //???????? ??? ???? 

    //??? ????????? heap?? ??? 
   temp.set_data("Kim", 88);
   h1.insert_heap(temp);

   temp.set_data("Lee", 77);
   h1.insert_heap(temp);

   temp.set_data("Park", 98);
   h1.insert_heap(temp);

   temp.set_data("Choi", 74);
   h1.insert_heap(temp);

   temp.set_data("Ryu", 94);
   h1.insert_heap(temp);

   temp.set_data("Cho", 85);
   h1.insert_heap(temp);

   while (h1.h_size() > 0) { //heap?? ??? node ???????? ?????????? ??? 
        temp = h1.delete_heap();
        cout << temp.name << " : " << temp.score << "\n";
   }

   return 0;

}



//constuctor
my_heap:: my_heap(){
    csize = 0;
} 

//?????? ????? node ???? 
void my_heap:: insert_heap(element newNode){

    //cout << "new node added\n";

    if(isFull()){
        cout << "heap is full :(\n";
        return;
    }

    //??? ???? ?? ??? ??????, ??? k?? csize
    int k = csize+1; //1?? ?��??????? ?????????????? ??? ?????? ?????? ???? ?��????? 
    h[k] = newNode;
    csize ++; 

    //????... ?????? ????? ??????... ?��????? ??? ?????? ???��??? (k???? k/2???? ?????? ?��?)
    while(1){
        if(k == 1) return; //??????? ???? ?? ??? 
        //?????? ?? ???? ?��?? ???????? k???? ?????????
        if(h[k].score > h[k/2].score){
            swap(&(h[k]), &(h[k/2]));
            k = k/2;
        }else return; //?��? ?? ???? ??????? ??????? ????. 
    }

} 

//????????? node???? ??? 
int my_heap:: h_size(){
    //cout << csize <<endl;
    return csize;
} 

//???????..? node ??????? ??? 
element my_heap:: delete_heap(){

    //cout << "node deleted\n";

    if(isEmpty()){
        cout << "heap is empty :(\n";
        element dumy;
        dumy.set_data("NONE", 0);
        return dumy;
    }


    //????????? root??? ??? ???????
    element tmp = h[1];
    //?????? ????? ????? root?? ????????
    h[1] = h[csize];
    //????? ?????? ?????? ???????...
    int k = 1; //??????? ????? ??? ???????? ????, #??????? 
    int c; //?????...! 
    csize --; 

    while(1){
        
        //??? (????? ?????)?????? ???????? ?????? ?????? ?????? ????? ????
        if(k*2+1 < csize) c = (h[k*2].score > h[k*2+1].score)? (k*2): (k*2+1);
        //??? ??? ?????? ????? ??????? ???? 
        else if(k*2 < csize) c = k*2;
        //?? ?????? ??? ?????? ??????? ??????
        else return tmp;  
        
        //?????? ?????? ?????? ?????, ?????? ???? ????
        if(h[k].score < h[c].score) swap(&(h[k]), &(h[c]));
        //?????? ???? ??????? ????????. ????.
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


//???? ??????��? ?????? ?????? ?????? ??????? ?????? 
void my_heap:: swap(element* a, element* b){

    element tmp = *a;
    *a = *b;
    *b = tmp;

} 


//????? ????? ???? 
void element:: set_data(string newName, int newScore){
    name = newName;
    score = newScore;
} 

