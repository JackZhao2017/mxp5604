#include "MPC5604C_M27V.h"

#include "UartPrint.h"
#include "led.h"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>


static   char  pUartBuf[200];

static void init_SIU(void)
{

	SIU.PCR[38].R = 0x0600;    // 将PB[2]设置为LINFlex_0.TxDA
	SIU.PCR[39].R = 0x0100;    // 将PB[3]设置为LINFlex_0.RxDA

}
void uartPrintInit(void)
{
	/* enter INIT mode */
	LINFLEX_1.LINCR1.B.INIT   = 1;   // 请求初始化
  	LINFLEX_1.LINCR1.B.SLEEP  = 0;  // 禁止睡眠模式
  	LINFLEX_1.LINCR1.B.BF     = 0;  // 如果ID不匹配不产生中断
	/* wait for the INIT mode */
	//while (0x1000 != (LINFLEX_0.LINSR.R & 0xF000)) {}
	
	LINFLEX_1.UARTCR.B.UART=1;	


	LINFLEX_1.UARTCR.B.TXEN=1;
	LINFLEX_1.UARTCR.B.RXEN=1;
	LINFLEX_1.UARTCR.B.WL=1;    
		
	/* configure for UART mode */
//INFLEX_0.UARTCR.R = 0x0001; /* set the UART bit first to be able to write the other bits */
//INFLEX_0.UARTCR.R = 0x0113; /* TX FIFO mode, 8bit data, no parity, Tx enabled, UART mode */
    LINFLEX_1.UARTCR.B.PCE    = 0;  
    LINFLEX_1.UARTCR.B.TDFL   = 0;      
    LINFLEX_1.UARTCR.B.RDFL   = 0; 
        
	LINFLEX_1.LINIBRR.B.DIV_M = 34;
	LINFLEX_1.LINFBRR.B.DIV_F = 12;
	
	LINFLEX_1.LINIER.R        = 0;
	
	LINFLEX_1.UARTSR.B.DRF    = 1; 
	LINFLEX_1.UARTSR.B.DTF    = 1;
	  	
	/* enter NORMAL mode */
	LINFLEX_1.LINCR1.B.INIT   = 0; /* INIT=0 */
	
	memset(pUartBuf,0,sizeof(pUartBuf));
	
	init_SIU();		
}

static void UART1_TX(char data)
{
  LINFLEX_1.BDRL.B.DATA0 = data;        
  while(LINFLEX_1.UARTSR.B.DTF == 0) {} 
  LINFLEX_1.UARTSR.B.DTF = 1;         
}

#define putch(x)  UART1_TX(x)

void OSsend_string(char *putchar) 
{
  while(*putchar!=0x00)      
  {
    UART1_TX(*putchar++);  
  }
}

void Uart_Printf(const char *fmt,...)//post massage to UartQ
{  

    va_list ap;  
    va_start(ap,fmt);     //将ap指向第一个实际参数的地址
	vsprintf((char *)pUartBuf,fmt,ap);
    va_end(ap); 
    OSsend_string(pUartBuf); 
}



