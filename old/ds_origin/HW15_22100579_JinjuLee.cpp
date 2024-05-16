#include <iostream>
using namespace std;

//데이타구조 Programming Assignment 15
//22100579 이진주

//6개의 sort algorithm을 적용한 function 구현
//아래의 main() 함수가 동작하도록 구성하시오. 

class s_record{
      //data
      string s_id;
      string name;
      double score;

      public:
      s_record(){};
      s_record(string nID, string nName, double nScore){ //constructor
            s_id = nID;
            name = nName;
            score = nScore;
      }
      string getID()
      {return s_id;}
      string getName()
      {return name;}
      double getScore()
      {return score;}
};


//6 sorting methods
void insertion_sort(s_record s[], int n);
void bubble_sort(s_record s[], int n);
void selection_sort(s_record s[], int n);
void quick_sort(s_record s[], int left, int right);
void merge_sort(s_record s[], int n);
void heap_sort(s_record s[], int n);

void merge_pass(s_record a[], s_record b[], int n, int s);
void merge(s_record a[], s_record b[], int n1, int n2, int n3, int n4);
void adjust2(s_record s[], int a, int b); //adjust the heap(customized)
void swap(s_record* a, s_record* b); //swap two records
void show_thelist(s_record s[], int n); //show the list


int main(){
      s_record s_list[12] = { 
            {"21900013", "Kim ", 6.5}, {"21900136", "Lee ", 8.8 }, {"21900333", "Park", 9.2 }, 
            { "21800442", "Choi", 7.1}, {"21900375", "Ryu ", 5.4  }, {"21700248", "Cho ", 6.3 },
            { "21700302", "Jung", 8.3}, {"21800255", "Han ", 6.9  }, {"21800369", "Kang", 6.3 },
            { "21900401", "Yang", 9.1}, {"21800123", "Moon", 8.1  }, {"21700678", "Seo ", 7.9 } 
      };
      int    n = 12;

      //insertion_sort(s_list, n);
      //bubble_sort(s_list, n);
      //selection_sort(s_list, n);

      //quick_sort(s_list, 0, n-1);
      merge_sort(s_list, n);
      //heap_sort(s_list, n); 

      cout << "\n< The result of the sorting >" << endl;
      show_thelist(s_list, n);

      return 0;
      

}


//6 sorting methods
//insertion sort
void insertion_sort(s_record s[], int n){
      s_record tmp;
      int index = 1;

      for(int i = 1; i < n ; i++){ //start index
            index = i;
            tmp = s[index];
            while(index > 0 && (tmp.getID() < s[index-1].getID())){
                  s[index] = s[index-1];
                  index --;
            }
            s[index] = tmp;
      }

}

//bubble sort
void bubble_sort(s_record s[], int n){

      for(int i = n; i >= 0 ; i--){ //range
            for(int j = 1; j < i; j++){ //sorting circles range
                  if(s[j-1].getID() > s[j].getID()){
                        swap(s[j-1], s[j]);
                  } 
            }
      }

}

//selection sort
void selection_sort(s_record s[], int n){

      for(int i = 0; i < n; i++){ //start index
            //find min value
            s_record* min = &s[i];
            for(int j = i+1; j < n; j++){ 
                  if(min->getID() > s[j].getID()) min = &s[j];
            }

            swap(s[i], *min);
            //cout << i+1 <<"st min value is " << s[i].getID() << ", " <<  s[i].getName() << endl;
      }
}


//quick sort
void quick_sort(s_record s[], int left, int right){


      if(left >= right) return; //return if size of list is under 1
      s_record pivot = s[left]; //set the pivot as first record
      int i = left, j = right+1; //index for compare

      do{
            do{ i++; }while((i <= right) && (s[i].getID() < pivot.getID())); //go forward till found bigger value then pivot
            do{ j--; }while((j >= left) && (s[j].getID() > pivot.getID())); //go backward till found less value then pivot
            if(i < j) swap(s[i], s[j]); //swap swap

      }while(i < j);
      
      swap(s[j], s[left]); //swap pivot and middle value
      quick_sort(s, left, j-1); //recursivly call
      quick_sort(s, j+1, right);


}



//heap sort
void heap_sort(s_record s[], int n){

      int i;
      int size = n-1;
      //init heap
      for(i = (size-1)/2; i >= 0; i--){
            adjust2(s, i, size);
      }

      //sorting
      for(int i = size-1; i >= 0; i--){
            swap(s[0], s[i+1]);
            adjust2(s, 0, i);
      }

}


//adjust the heap(customized, about the heap starts from index 0.)
/*
parent = (k-1)/2 
left child = 2*k + 1 
right child = 2*k + 2
*/
void adjust2(s_record s[], int k, int n){

      string tmpkey;
      int child;
      s_record tmp;

      tmp = s[k]; //save the root record
      tmpkey = s[k].getID(); 
      child = 2*k + 1; //init child

      while(child <= n){
            //select bigger child to compare with parent
            if((child < n) && (s[child].getID() < s[child+1].getID())) child ++;
            //compare root and child
            if(tmpkey > s[child].getID()) break; //작으면 거기로 결정. 거기가 어딘데? 현재 child의 부모위치. 
            else{
                  s[(child-1)/2] = s[child]; //끌어올리고 내려가. child를 부모위치로 승격시키고 다음 도전자 선정. 
                  child = child*2 + 1 ;
            }
      }
      s[(child-1)/2] = tmp;
}




//merge sort
void merge_sort(s_record s[], int n){

      //cout << "[merge sort called!]\n";
      int size = 1;
      s_record b[n];

      while(size < n){
            //cout << "doing merge sort 1...\n";
            merge_pass(s, b, n-1, size);
            size = size* 2;
            //cout << "doing merge sort 2...\n";
            merge_pass(b, s, n-1, size);
            size = size* 2;
            
      }

}

void merge_pass(s_record a[], s_record b[], int n, int s){
      //cout << "[merge pass called! with size " << s << "]\n";
      int i, j;

      //doing merge about standard size(s)
      for(i = 0; i < (n - 2*s + 1); i += 2*s){

            merge(a, b, i, i+s-1, i+s, i+2*s-1);
      } 

      //cout << "[pass] finished one pase~\n";
      //if the lenth of remain list is bigger than s, merge others. 
      if(i+s <= n){
            merge(a, b, i, i+s-1, i+s, n);

      }else{ //if not, just copy b to a 

            for(j = i; j <= n; j++){
                  b[j] = a[j]; 
            }
      } 
      //cout << "[pass] one merge_pass done.\n";
      
}

void merge(s_record a[], s_record b[], int n1, int n2, int n3, int n4){

      int i = n1, j = n3, k = n1, t;
      //cout << "[merge called! n4 is " << n1<<n2<<n3<<n4 << "] \n";

      //merge (a to b)
      while((i <= n2) && (j <= n4)){

            if(a[i].getID() <= a[j].getID()) {
                  b[k++] = a[i++];
            }else {
                  b[k++] = a[j++];
            }
      }
      //cout << "[merge] current ik is " << i << k << endl;
      //move remain records at a to b
      if(i > n2){
            for(t = j; t <= n4; t++){
                  b[t] = a[t]; 
            }
      }else{
            for(t = i; t <= n2; t++){
                  b[k+t-i] = a[t];
                  
            } 
      }
      //cout << "[merge] succesfully done, " << n1 << n2 << n3 << n4 << endl;
      

}



//swap two records
void swap(s_record* a, s_record* b){
      s_record tmp = *a;
      *a = *b;
      *b = tmp;
}

//show the list
void show_thelist(s_record s[], int n){
      for(int i = 0; i < n; i++){
            cout << s[i].getID() << " : " << s[i].getName() << " : " << s[i].getScore() << endl;
      }
      cout << endl;
}

