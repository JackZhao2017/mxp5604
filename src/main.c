#include "MPC5604C_M27V.h"

#include "sysclock.h"
#include "led.h"
#include "UartPrint.h"
#include "Uart.h"
#include "can.h"


void delay(int ms)
{   
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
     for(jj=0;jj<2000;jj++){}        
}
CAN_RXMSG msg;
int main(void) {
  int flag=0,ret=0;	
  sysclockInit();
  
  ledInit();
  
  uartPrintInit(); 
  
  uartInit();
  
  initCan();
  
  while(1) {
	delay(1000);
	if(flag)
	{
		flag =0;
		LED_OFF(LED0);
		LED_ON(LED1);	
	}
	else
	{
		flag =1 ;
		LED_ON(LED0);
		LED_OFF(LED1);	
	}
	ret = RecieveMsg(&msg);
	if(ret)
	{
		TransmitMsg();
	}
	
	Uart_SendData("start run\r\n",strlen("start run\r\n"));
	OSsend_string("start run\r\n");
  }
  return 0;
}
