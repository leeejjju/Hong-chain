#include <iostream>
#include <string>
#include <signal.h>
#include <stdlib.h>

int main() {
    // 시그널 핸들러 설정
    

    // 문자열 입력을 받을 변수 선언
    std::string inputString;
    
    // 사용자에게 입력 안내 메시지 출력
    std::cout << "Enter a string: ";
    
    // 표준 입력으로부터 문자열 입력 받기
    std::getline(std::cin, inputString);
    if(inputString.compare("crash") == 0)
    {
        fprintf(stderr, "here crash\n");
        int * num = NULL;
        *num = 12;
    }

    if(inputString.compare("incorrect") == 0)
    {
        std::cout << "tata" << std::endl;
    }

    if(inputString.compare("incorrect2") == 0)
    {
        std::cout << "tata2" << std::endl;
    }

    if(inputString.compare("roop") == 0)
    {
        while(1);
    }
    
    // 입력받은 문자열을 출력
    std::cout << "You entered: " << inputString << std::endl;

    return 0;
}
