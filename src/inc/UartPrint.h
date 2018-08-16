#ifndef _UART_PRINT_H_
#define _UART_PRINT_H_



void uartPrintInit(void);
void OSsend_string(char *putchar); 
void Uart_Printf(const char *fmt,...);//post massage to UartQ
void Uart1_SendData( char *data,int len);
#endif