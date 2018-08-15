#ifndef _CAN_H_
#define _CAN_H_

#include "MPC5604C_M27V.h"

typedef  struct
{
	uint32_t  ID; /* Received message ID */
	uint32_t  LENGTH; /* Recieved message number of data bytes */
	uint8_t   DATA[8];  /* Received message data string*/
	uint32_t  TIMESTAMP;
	uint32_t  dummy;
	uint32_t  CODE; 
}CAN_RXMSG;

typedef struct {
	uint8_t  SYNC;
	uint8_t  TYPE;
	uint8_t  ID;
	uint8_t  Data[8];
	uint8_t  CRC8;
}UartMwrMsg;

#define  MWRMSG      0x0b
#define  MSY_SYNC    0x55

void initCan(void);

void TransmitMsg (void);

uint8_t RecieveMsg (CAN_RXMSG *msg);

#endif