#ifndef _CAN2UART_H_
#define _CAN2UART_H_

#include "MPC5604C_M27V.h"

#define CANMSG_MAX 20

void msgBufferInit(void);
void clearBuffer(void);
uint32_t setReceiveSize(uint32_t size);
uint8_t PutCanMsgToBuffer(CAN_RXMSG *msg);
uint8_t GetUartMsgFromBuffer(UartMwrMsg *mwrmsg);
uint32_t getMsgSum(void);
#endif