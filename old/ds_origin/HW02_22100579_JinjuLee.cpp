#include <iostream>
using namespace std;

//데이타구조 2주차2 과제 
/*무게 데이터를 연산할 수 있도록 class type 구현
- kg과 gram단위를 분리하여 연산 가능, ‘덧셈’과 ‘크기 비교 연산’ 구현
- 다음 main( ) 함수가 동작할 수 있도록 나머지 부분 작성
*/

class weight{
    public:
        int wkg; //키로그램 단위 무게
        int wg; //그램 단위 무게
        void set_weight(int kg, int g); //무게 설정
        int get_weight(); //인스턴트의 무게 반환

};

bool less_than(weight w1, weight w2); // 두 무게의 크기를 비교하는 함수 
weight add_weight(weight w1, weight w2); //두 무게 더해서 새로운 인스턴트에 넣고 반환.

int main(){
    weight  w1, w2, w3;  // weight라는 class object 3개 생성

    w1.set_weight(3, 400);  // w1을 3kg 400 g으로
    w2.set_weight(2, 700);  // w2을 2kg 700 g으로
    w3 = add_weight(w1, w2); // w1과 w2을 더하여 w3에 à w3는 6kg 100g

    cout << w3.get_weight( ) << "grams\n"; // w3의 값 ‘6100 grams’ 출력

    if ( less_than(w1, w2) ) // w1이 w2보다 작은 값인가 판단
        cout << "yes.\n";
    else
        cout << "no. \n";   // 작지 않으므로 ‘no’ 출력
}


void weight :: set_weight(int kg, int g){
    wkg = kg;
    wg = g;
} //무게 설정

int weight :: get_weight(){
    return (wkg*1000 + wg);
} //인스턴트의 무게 반환

weight add_weight(weight w1, weight w2){
    weight w3;
    
    w3.wkg = w1.wkg + w2.wkg;
    w3.wg = w1.wg + w2.wg;

    if(w3.wg >= 1000){
        w3.wkg ++;
        w3.wg -= 1000;
    }

    return w3;
} //두 무게 더해서 새로운 인스턴트에 넣고 반환 


bool less_than(weight w1, weight w2){
    //앞에놈이 더 크면 true, 아니면 false
    if (w1.get_weight() > w2.get_weight()){
        return true;
    }else{
        return false;
    }

} // 두 무게의 크기를 비교하는 함수 



