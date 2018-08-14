#include "MPC5604C_M27V.h"

#include "sysclock.h"
#include "led.h"



void delay(int ms)
{   
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
     for(jj=0;jj<2000;jj++){}        
}

int main(void) {
  int flag=0;	
  sysclockInit();
  
  ledInit();
    
  while(1) {
	delay(1000);
	if(flag)
	{
		flag =0;
		LED_ON(LED1);	
	}
	else
	{
		flag =1 ;
		LED_OFF(LED1);	
	}
  }
  return 0;
}
