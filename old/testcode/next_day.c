#include <stdio.h>
/*년 월 일이 모두 정상적인 값으로 들어오는 것을 가정으로 했습니다.*/
int main(void) {
  int year=0, month=0, day=0;
  int leap_year=0;

  printf("다음날을 알고 싶은 년 월 일을 입력하세요 : ");
  scanf("%d %d %d",&year,&month, &day);

  if(year%400==0){
    leap_year=1;
  }
  else if(year%100==0){
    leap_year=0;
  }
  else if(year%4==0){
    leap_year=1;
  }
  else{
    leap_year=0;
  }
  
  switch(month){
    case 2 : 
      if(leap_year==1){
        if(day==29){
          day=1;
          month++;
        }
        else{
          day++;
        }
      }
      else{
        if(day==28){
          day=1;
          month++;
        }
        else{
          day++;
        }
      }
      break;

    case 4 :
    case 6 :
    case 9 :
    case 11 :
      if(day==30){
          day=1;
          month++;
        }
        else{
          day++;
        }
      break;
    
    case 12 :
      if(day==31){
          day=1;
          month=1;
        }
        else{
          day++;
        }
      break;
    
    default :
      if(day==31){
          day=1;
          month++;
        }
        else{
          day++;
        }
      break;
  }
  printf("%d %d %d",year,month,day);
  return 0;
}