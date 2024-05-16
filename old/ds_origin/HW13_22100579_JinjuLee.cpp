#include <iostream>
using namespace std;

//데이타구조 Programming Assignment 13
//22100579 이진주

/*Binary Search Tree 구현 예제
아래의 주어진  main 함수의 의도대로 동작하는 프로그램을 완성하시오.
(강의 내용 참조)
*/

class bst_node{

    public:
    //data
    string s_id;
    string name;
    double score;

    //link
    bst_node* left;
    bst_node* right;

    void set_data(string newID, string newName, double newScore){
        s_id = newID;
        name = newName;
        score = newScore;
        left = NULL;
        right = NULL;
    }

};

class bst_tree{
    bst_node* root;
    int size;

    public:
    bst_tree(){
        root = NULL;
        size = 0;
    }
    bst_node search(string key);
    void insert_node(bst_node x); 
    void delete_node();
    void show_inorder();
    int getSize();
    bool isEmpty();
    

};

void inorder_traverse(bst_node* p);


int main(){

    bst_node temp;
    bst_tree t1;

    temp.set_data("21900013", "Kim", 6.5);      
    t1.insert_node(temp);     
    temp.set_data("21900136", "Lee", 8.8);
    t1.insert_node(temp);
    temp.set_data("21900333", "Park", 9.2);
    t1.insert_node(temp);
    temp.set_data("21800442", "Choi", 7.1);
    t1.insert_node(temp);
    temp.set_data("21900375", "Ryu", 5.4);
    t1.insert_node(temp);
    temp.set_data("21700248", "Cho", 6.3);
    t1.insert_node(temp);

    cout << "\n\n Node List : inorder sequence \n";
    t1.show_inorder();

    string s_key = "21800442";
    temp = t1.search(s_key);

    cout << "\n -- " << s_key << "'s record ---" << endl;
    cout << " Student ID   : "<< temp.s_id << endl;
    cout << " Student Name : "<< temp.name << endl;
    cout << " Score        : " << temp.score << endl;
    
    return 0;
}

 

bst_node bst_tree:: search(string key){

    
    bst_node none;
    none.set_data("00000000", "none", 0);
    if(isEmpty()){
      cout << "tree is empty :(";
      return none;
    } 

    bst_node* t = root;

    while(1){
        if(t == NULL){
            cout << "cannot find the node of the key :(";
            return none;
        } 
        if(t->s_id == key) return *t;
        else if((t->s_id) > key) t = t->left;
        else t = t->right;
    }

}

void bst_tree:: insert_node(bst_node x){
  
  bst_node* tmp = new bst_node;
  *tmp = x;
  tmp->left = NULL;
  tmp->right = NULL;


  if(isEmpty()){
    root = tmp;
    size++;
    return;
  }
  
  bst_node* t = root;
  while(1){
      if(t->s_id == x.s_id){
        cout << "[error: dupplicated key value]\n";
        return;
      }else if(t->s_id > x.s_id){
        if(t->left == NULL){
          t->left = tmp;
          break;
        }else t = t->left;
      }else{
        if(t->right == NULL){
          t->right = tmp;
          break;
        }else t = t->right;
      }
  }
  
  size++;
  //cout << root->name << " is successfully added." << endl;
  //cout << "current size is " << size << endl;
  

}


void bst_tree:: show_inorder(){

  bst_node* t = root;
  inorder_traverse(t);

}

void inorder_traverse(bst_node* p){
  //왼쪽, 본인, 오른쪽 순으로 traversal
  //linked list
  if(p == NULL) return;
  inorder_traverse(p->left);
  cout << p->s_id << " : " << p->name << " : " << p->score<< endl;
  inorder_traverse(p->right);
}




int bst_tree:: getSize(){
    return size;
}


bool bst_tree:: isEmpty(){
    if(size > 0) return false;
    return true;
}


 