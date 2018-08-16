#include "can.h"
#include "Uart.h"
#include "Can2Uart.h"
#include "CRC8.h"

typedef struct
{
	uint32_t getindex;
	uint32_t putindex;
	uint32_t sum;
	CAN_RXMSG MSGBUFFER[CANMSG_MAX];	
}MSG_BUFFER_tag;

MSG_BUFFER_tag g_msgBuffer;


void msgBufferInit(void)
{
	memset(&g_msgBuffer,0,sizeof(MSG_BUFFER_tag));
}
void clearBuffer(void)
{
	g_msgBuffer.putindex=0;
	g_msgBuffer.getindex=0;
	g_msgBuffer.sum=0;
}
uint32_t getMsgSum(void)
{
	return g_msgBuffer.sum;
}
uint32_t setReceiveSize(uint32_t size)
{
	if(size>CANMSG_MAX-1)
	{
		size= CANMSG_MAX-1;
	}
	g_msgBuffer.sum=size;
	return (g_msgBuffer.sum-1);
}
uint8_t PutCanMsgToBuffer(CAN_RXMSG *msg)
{
	memcpy(&g_msgBuffer.MSGBUFFER[g_msgBuffer.putindex],msg,sizeof(CAN_RXMSG));
	g_msgBuffer.putindex+=1;
	if(g_msgBuffer.putindex>=g_msgBuffer.sum)
		return 1;
	return 0;
}
static void convertmsg(CAN_RXMSG *canmsg,UartMwrMsg *mwrmsg)
{
	memcpy(mwrmsg->Data,canmsg->DATA,8);
	mwrmsg->SYNC=MSY_SYNC;
	mwrmsg->TYPE=MWRMSG;
	mwrmsg->ID=(uint8_t)(canmsg->ID&(0xff));
	mwrmsg->CRC8=crc8_creator((uint8_t*)(&mwrmsg->TYPE),0,MWRMSG-1);	
}


uint8_t GetUartMsgFromBuffer(UartMwrMsg *mwrmsg)
{
	if(g_msgBuffer.getindex>=g_msgBuffer.sum)
		return 0;
	convertmsg(&g_msgBuffer.MSGBUFFER[g_msgBuffer.getindex],mwrmsg);
	g_msgBuffer.getindex+=1;
	return 1;
}

 