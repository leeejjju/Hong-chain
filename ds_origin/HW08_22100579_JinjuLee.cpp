#include <iostream>
using namespace std;

//데이타구조 HW08
//22100579 이진주
//'주어진 array의 n개 정수 원소 중 가장 큰 값을 찾는  프로그램'을 작성하고자 한다.
//아래의 main() 함수에서 find_max() 를 recursive function으로 구현하여 프로그램을 완성하시오.

//예상 출력 > 84

 
int find_max(int a[], int size); //주어진 array의 n개 정수 원소 중 가장 큰 값을 찾는  프로그램/recursive function


int main(){

    int   a[10] = { 24, 12, 33, 27, 41, 15, 84, 52, 16, 72 };
    int  m;

    m = find_max(a, 10);

    cout << m;

    return 0;
}


//주어진 array의 n개 정수 원소 중 가장 큰 값을 찾는  프로그램/recursive function
int find_max(int a[], int size){

    //(size-1)번째 원소랑 (size-2)번째 원소 비교해서 더 큰놈 반환하고 size 인자로 하나씩 적게 주자. size가 0이 들어오면 자기 자신 리턴. 
    //                   ^이게 size인자를 하나씩 줄인 자기 자신 call해서 얻을 값

    if(size == 1) return a[size-1];
      
    else {
      int num = find_max(a, size-1);
      //printf("%d와 %d를 비교함\n", a[size-1], num);
      return (a[size-1] > num)? a[size-1] : num;

    }
  
} 