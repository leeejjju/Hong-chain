#include <iostream>
using namespace std;
#define HSIZE 50

//����Ÿ���� Programming Assignment 12
//22100579 ������

/*
Heap ���� : ���� 11�� ����� Ȯ���Ͽ�, ���� ������ ����ǵ��� ���α׷��� �ϼ��Ͻÿ�.

<���� ���>
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
    void set_data(string newName, int newScore); //����� ����Ÿ ���� 
};

class my_heap{
    element h[HSIZE]; //link list������ �ƴ� array����..!�̹Ƿ� root ������ �̷��� �ʿ� ��
    int csize; //number of nodes

    public:
    my_heap(element a[], int size); //constuctor -���⼭ �� ������? ���Ѿ��ϳ�? 
    void insert_heap(element newNode); //������ ��ġ�� node ���� 
    int h_size(); //��������� node���� ��ȯ 
    element delete_heap(); //�ְ���..? node �����ϰ� ��ȯ 
    bool isFull();
    bool isEmpty();
    void swap(element* a, element* b);

    //new
    double score_sum(); //score�� ���� ��ȯ
    double score_average(); //score�� ��� ��ȯ
    void node_delete_by_name(string keyName); //�Է����� name�� node�� ����
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

//���� �� ���� ��� 
void printHeap(my_heap h1){
    while (h1.h_size() > 0) {
        element temp = h1.delete_heap();
        cout << temp.name << " : " << temp.score << "\n";
   }
   printf("----------------------\n\n");
}




//constuctor
my_heap:: my_heap(element a[], int size){
    //���⼭ ���������� �ִ°� �ƴ϶� insert���� Ȱ���ؼ� ���ʷ� �ִ°� �´µ�
    csize = 0;
    for(int i = 0; i < size; i++){ //array copy
        this->insert_heap(a[i]);
        //cout << a[i].name << " added.\n";//num of nodes is " << csize << endl;
    }
    
} 

//������ ��ġ�� node ���� 
void my_heap:: insert_heap(element newNode){

    //cout << "new node added\n";

    if(isFull()){
        cout << "heap is full :(\n";
        return;
    }

    //�ϴ� ���� �� ��忡 �־��ְ�, �̶� k�� csize
    int k = csize+1; //1�� �ε������� �����ϰ�����ϱ� �̷� ó���ϸ� �Ƿ���? ���� �ε����� 
    h[k] = newNode;
    csize ++; 

    //�ƴϸ�... ������ ��Ʋ�� ���۵ȴ�... �θ����� ��� ������ ���а��� (k���� k/2���� ������ �ο�)
    while(1){
        if(k == 1) return; //��Ʈ���� ���� �� Ż�� 
        //������ �� ũ��? �θ�� �����մϴ� k���� �������ְ�
        if(h[k].score > h[k/2].score){
            swap(&(h[k]), &(h[k/2]));
            k = k/2;
        }else return; //�θ� �� ũ��? ���ڸ��� �����ϰ� ����. 
    }

} 

//��������� node���� ��ȯ 
int my_heap:: h_size(){
    //cout << csize <<endl;
    return csize;
} 

//�ְ���..? node �����ϰ� ��ȯ 
element my_heap:: delete_heap(){

    //cout << "node deleted\n";

    if(isEmpty()){
        cout << "heap is empty :(\n";
        element dumy;
        dumy.set_data("NONE", 0);
        return dumy;
    }


    //���Ͽ����� root��带 ��� �����ϰ�
    element tmp = h[1];
    //������ �װ��� ģ���� root�� �̵���Ű��
    h[1] = h[csize];
    //�Ʒ��� ������ ������ �޾Ƶ��̱�...
    int k = 1; //��Ʈ���� ���ڸ� ã�� �������� ����, #�����ڰ� 
    int c; //ç����...! 
    csize --; 

    while(1){
        
        //�ϴ� (�ڽ��� ���̸�)�ڽĵ鳢�� �������� ������ �̱���� ������ �ڰ��� ����
        if(k*2+1 <= csize) c = (h[k*2].score > h[k*2+1].score)? (k*2): (k*2+1);
        //�ڽ� �ϳ� ������ �׳��� �����ڷ� ���� 
        else if(k*2 <= csize) c = k*2;
        //�� ������ �ڽ� ������ ���ڸ��� ������
        else return tmp;  
        
        //������ ������ ���ƴٸ� �񱳤���, ������ �۴�? ����
        if(h[k].score < h[c].score) swap(&(h[k]), &(h[c]));
        //������ ũ��? ���ڸ��� õ���ΰ���. ����.
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


//���� �ڸ��ٲٴ°� �޼���� ó���ϸ� ������ ������? �ƴԸ��� 
void my_heap:: swap(element* a, element* b){

    element tmp = *a;
    *a = *b;
    *b = tmp;
}

//----------------------------------------------------------

//score�� ���� ��ȯ
double my_heap:: score_sum(){

    double sum = 0;
    for(int i = 1; i < csize+1; i++){
        sum += h[i].score;
        //cout << h[i].name << ", ";
    }
    //cout << "'s added to sum.\n";
    return sum;

}

//score�� ��� ��ȯ
double my_heap:: score_average(){
    return score_sum()/csize;
} 

//�Է����� name�� node�� ����
void my_heap:: node_delete_by_name(string keyName){
    int i;
    for(i = 1; i < csize; i++)
        if(keyName == h[i].name) break;
    //���� Ż�� �������� i�� ������ �̸���..�Žñ��� �ε����̱� �����
    if(i >= csize){ //�ƴԸ��� 
        cout << "cannot find the name";
        return;
    }else{ //i�� root�� �ϴ� delete ����

        if(isEmpty()){
            cout << "heap is empty :(\n";
            return;
        }else{
            //cout << h[i].name << " gonna be deleted.\n";
        }


        //�� ������ ģ���� i�� �̵���Ű��
        h[i] = h[csize];
        csize --; 
        //�Ʒ��� ������ ������ �޾Ƶ��̱�...
        int k = i; //i���� ���ڸ� ã�� �������� ����, #�����ڰ� 
        int c; //ç����...! 
        

        while(1){
            
            //�ϴ� (�ڽ��� ���̸�)�ڽĵ鳢�� �������� ������ �̱���� ������ �ڰ��� ����
            if(k*2+1 <= csize) c = (h[k*2].score > h[k*2+1].score)? (k*2): (k*2+1);
            //�ڽ� �ϳ� ������ �׳��� �����ڷ� ���� 
            else if(k*2 <= csize) c = k*2;
            //�� ������ �ڽ� ������ ���ڸ��� ������
            else return; 
            
            //������ ������ ���ƴٸ� �񱳤���, ������ �۴�? ����
            if(h[k].score < h[c].score) {
                swap(&(h[k]), &(h[c]));
                //printHeap(*this);
                cout << h[k].name << ", " << h[c].name << " swaperd." << endl;
            }
            //������ ũ��? ���ڸ��� õ���ΰ���. ����.
            else return;

        }
    }


} 


//----------------------------------------------------------


//����� ����Ÿ ���� 
void element:: set_data(string newName, int newScore){
    name = newName;
    score = newScore;
} 


