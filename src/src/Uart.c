#include "MPC5604C_M27V.h"
#include "led.h"


void init_SIU(void)
{

	SIU.PCR[18].R = 0x0600;    // 将PB[2]设置为LINFlex_0.TxDA
	SIU.PCR[19].R = 0x0100;    // 将PB[3]设置为LINFlex_0.RxDA

}
void uartInit(void)
{

	LED_ON(LED0);
	/* enter INIT mode */
	LINFLEX_0.LINCR1.B.INIT   = 1;   // 请求初始化
  	LINFLEX_0.LINCR1.B.SLEEP  = 0;  // 禁止睡眠模式
  	LINFLEX_0.LINCR1.B.BF     = 0;  // 如果ID不匹配不产生中断
	/* wait for the INIT mode */
	//while (0x1000 != (LINFLEX_0.LINSR.R & 0xF000)) {}
	
	LINFLEX_0.UARTCR.B.UART=1;	


	LINFLEX_0.UARTCR.B.TXEN=1;
	LINFLEX_0.UARTCR.B.RXEN=1;
	LINFLEX_0.UARTCR.B.WL=1;    
		
	/* configure for UART mode */
	//INFLEX_0.UARTCR.R = 0x0001; /* set the UART bit first to be able to write the other bits */
	//INFLEX_0.UARTCR.R = 0x0113; /* TX FIFO mode, 8bit data, no parity, Tx enabled, UART mode */
    LINFLEX_0.UARTCR.B.PCE    = 0;  
    LINFLEX_0.UARTCR.B.TDFL   = 0;      
    LINFLEX_0.UARTCR.B.RDFL   = 0; 
        
	LINFLEX_0.LINIBRR.B.DIV_M = 34;
	LINFLEX_0.LINFBRR.B.DIV_F = 12;
	
	LINFLEX_0.LINIER.R        = 0;
	
	LINFLEX_0.UARTSR.B.DRF    = 1; 
	LINFLEX_0.UARTSR.B.DTF    = 1;
	  	
	/* enter NORMAL mode */
	LINFLEX_0.LINCR1.B.INIT   = 0; /* INIT=0 */
	
	init_SIU();	
}



void UART0_TX( char data)
{
  LINFLEX_0.BDRL.B.DATA0 = data;        
  while(LINFLEX_0.UARTSR.B.DTF == 0) {} 
  LINFLEX_0.UARTSR.B.DTF = 1;          
}


void send_string( char *putchar) 
{
  while(*putchar!=0x00)      
  {
    UART0_TX(*putchar++);  
  }
}
void Uart_SendData( char *data,int len)
{
	int index=0;
	int size=len;
	while(index<size)
	{
		UART0_TX(data[index]);
		index+=1;
	}
}



















