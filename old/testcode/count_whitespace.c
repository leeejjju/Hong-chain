#include <stdio.h>
#include <string.h>
/*문장을 하나 입력받은 후에, 이 문장 속에서 white space 문자의 개수를 세라.

단, 주어진 함수를 반드시 recursion을 사용하여 구현한 후, 사용할 것. main() 수정금지!*/

int rSpaces(char* str);

int main(void) {
	char line[100];
	printf("Enter a string > ");
	fgets(line, 100, stdin);
	printf("spaces : %d\n", rSpaces(line));
	return 0;
}

//----------------

int rSpaces(char* str){

  if(strlen(str) == 1){
    return 0;

  }else{
    if(str[0] == ' '){
      //printf("3\n");
      return 1 + rSpaces(&str[1]);
    }else{
      //printf("4\n");
      return rSpaces(&str[1]);
    }
  }
}
//파라미터 : 개수를 세어야 하는 문자열의 시작 포인터
//리턴값 : 이 포인터로부터 문자열의 끝까지 들어있는 공백 문자 갯수
