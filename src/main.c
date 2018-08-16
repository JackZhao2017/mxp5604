#include "MPC5604C_M27V.h"

#include "sysclock.h"
#include "led.h"
#include "UartPrint.h"
#include "Uart.h"
#include "can.h"



#include "CRC8.h"


#include "UJA1078.h"

void delay(int ms)
{   
   int ii,jj;
   if (ms<1) ms=1;
   for(ii=0;ii<ms;ii++)
     for(jj=0;jj<1500;jj++){}        
}
CAN_RXMSG can_rxmsg;

uint8_t g_speed=0;

void resolverCarspeed(uint8_t *buf)
{
  uint8_t datalen=buf[0];
  uint8_t mode =buf[1];
  uint8_t pid  =buf[2];

  if(mode!=0x41){
    return ;
  }
  if(pid!=0x0d){
    return ;
  }
  g_speed=buf[datalen];	
}

void UartSendWmrmsg(void)
{
    uint8_t i=0;
    uint8_t sum= (uint8_t)getMsgSum();
    UartMwrMsg mMwrMsg;

    for(i=0;i<sum;i++){
        if(GetUartMsgFromBuffer(&mMwrMsg)){
          Uart1_SendData((char  *)&mMwrMsg,mMwrMsg.TYPE+1);
        }
    }

}

int main(void) {

  int flag=0,ret=0;	

  sysclockInit();
  
  ledInit();
  
  uartPrintInit(); 
  
  uartInit();
  
  initCan();
  
  crcInit(LSB,POLY);
  
  msgBufferInit();
 
  UJA1078_Init();
  

  delay(10000);

  UJA1078_GOIN_NOMAL();

  delay(10000);  
  while(1) { 
     	ret = RecieveMsg(&can_rxmsg);
    	if(ret==1)
    	{
    		if(can_rxmsg.ID==0x60a)
            {
                clearBuffer();
                can_rxmsg.DATA[0] = (uint8_t)setReceiveSize(can_rxmsg.DATA[0]+1);
                can_rxmsg.DATA[4] = g_speed;
            }
           	if(PutCanMsgToBuffer(&can_rxmsg)){
            	UartSendWmrmsg();
            	TransmitMsg();
        	  }		
    	}
    	else if (ret==2)
    	{
    		resolverCarspeed(&can_rxmsg.DATA[0]);
    	}
  }
  return 0;
}
