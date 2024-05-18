#include <iostream>
#include <string>

int main() {
    // 문자열 입력을 받을 변수 선언
    std::string inputString;
    
    // 사용자에게 입력 안내 메시지 출력
    std::cout << "Enter a string: ";
    
    // 표준 입력으로부터 문자열 입력 받기
    std::getline(std::cin, inputString);
    
    // 입력받은 문자열을 출력
    std::cout << "You entered: " << inputString << std::endl;

    return 0;
}