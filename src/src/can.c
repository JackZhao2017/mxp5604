#include "can.h"
#include "MPC5604C_M27V.h"
#include "led.h"


uint32_t  RxLENGTH;              /* Received message buffer code */

static char request_buf[]={0x02, 0x01 ,0x0D, 0x00, 0x00, 0x00, 0x00, 0x00};

static  void init_port(void)
{
  SIU.PCR[16].R = 0x0624;         /* MPC56xxB: Config port B0 as CAN0TX, open drain */
  SIU.PCR[17].R = 0x0100;         /* MPC56xxB: Configure port B1 as CAN0RX */	
}




void initCan(void)
{
  uint8_t   i;

  CAN_0.MCR.R = 0x5000003F;       /* Put in Freeze Mode & enable all 64 msg bufs */
  
  //CAN_0.CR.R = 0x04DB0006;        /* Configure for 8MHz OSC, 100KHz bit time */
   CAN_0.CR.B.PRESDIV=1;
   CAN_0.CR.B.RJW=3;
   CAN_0.CR.B.PSEG1=1;
   CAN_0.CR.B.PSEG2=1;
   CAN_0.CR.B.PROPSEG=2;
  
  for (i=0; i<64; i++) {
    CAN_0.BUF[i].CS.B.CODE = 0;   /* Inactivate all message buffers */
  } 
  CAN_0.BUF[0].CS.B.CODE = 8;     /* Message Buffer 0 set to TX INACTIVE */
  
  CAN_0.BUF[3].CS.B.IDE=0;
  CAN_0.BUF[3].ID.B.STD_ID=0X7e8;
  CAN_0.BUF[3].CS.B.CODE = 4; 
  
  CAN_0.BUF[4].CS.B.IDE=0;
  CAN_0.BUF[4].ID.B.STD_ID=0X60a;
  CAN_0.BUF[4].CS.B.CODE = 4; 

  CAN_0.BUF[5].CS.B.IDE=0;
  CAN_0.BUF[5].ID.B.STD_ID=0X60b;
  CAN_0.BUF[5].CS.B.CODE = 4; 

  CAN_0.RXGMASK.R = 0x1FFFFFFF;

  LED_ON(LED2);

  init_port();

  CAN_0.MCR.R = 0x0000003F;       /* Negate FlexCAN 0 halt state for 64 MB */	
}

void TransmitMsg (void) {
  uint8_t	i;
                                   /* Assumption:  Message buffer CODE is INACTIVE */
  
  CAN_0.BUF[0].CS.B.IDE = 0;           /* Use standard ID length */
  CAN_0.BUF[0].ID.B.STD_ID = 0X7df;      /* Transmit ID is 555 */
  CAN_0.BUF[0].CS.B.RTR = 0;           /* Data frame, not remote Tx request frame */
  CAN_0.BUF[0].CS.B.LENGTH = 8 ; /* # bytes to transmit w/o null */
  for (i=0; i<sizeof(request_buf); i++) {
    CAN_0.BUF[0].DATA.B[i] = request_buf[i];      /* Data to be transmitted */
  }
  CAN_0.BUF[0].CS.B.SRR = 1;           /* Tx frame (not req'd for standard frame)*/
  CAN_0.BUF[0].CS.B.CODE =0xC;         /* Activate msg. buf. to transmit data frame */ 
  
}

uint8_t  RecieveMsg (CAN_RXMSG *msg) {
  uint8_t j;


  //while (CAN_0.IFRL.B.BUF04I == 0) {};  /* Wait for CAN 1 MB 4 flag */
  while (CAN_0.IFRL.R == 0) {};
  
  if(CAN_0.IFRL.B.BUF03I==1)
  {
  	
    msg->CODE   = CAN_0.BUF[3].CS.B.CODE;    /* Read CODE, ID, LENGTH, DATA, TIMESTAMP */
  	msg->ID     = CAN_0.BUF[3].ID.B.STD_ID;
  	RxLENGTH = CAN_0.BUF[3].CS.B.LENGTH;
  	for (j=0; j<RxLENGTH; j++) { 
    	msg->DATA[j] = CAN_0.BUF[3].DATA.B[j];
  	}
  	msg->LENGTH=8;
  	msg->TIMESTAMP = CAN_0.BUF[3].CS.B.TIMESTAMP; 
  	msg->dummy = CAN_0.TIMER.R;                /* Read TIMER to unlock message buffers */  
  	CAN_0.BUF[3].CS.B.TIMESTAMP=  CAN_0.TIMER.R;  
  	CAN_0.IFRL.R = 0x00000008;            /* Clear CAN 1 MB 4 flag */
  	return 2;
  }
  
  if(CAN_0.IFRL.B.BUF04I==1)
  {
  	
    msg->CODE   = CAN_0.BUF[4].CS.B.CODE;    /* Read CODE, ID, LENGTH, DATA, TIMESTAMP */
  	msg->ID     = CAN_0.BUF[4].ID.B.STD_ID;
  	RxLENGTH= CAN_0.BUF[4].CS.B.LENGTH;
  	for (j=0; j<RxLENGTH; j++) { 
    	msg->DATA[j] = CAN_0.BUF[4].DATA.B[j];
  	}
  	msg->LENGTH=8;
  	msg->TIMESTAMP = CAN_0.BUF[4].CS.B.TIMESTAMP; 
  	msg->dummy = CAN_0.TIMER.R;                /* Read TIMER to unlock message buffers */  
  	CAN_0.BUF[4].CS.B.TIMESTAMP=  CAN_0.TIMER.R;  
  	CAN_0.IFRL.R = 0x00000010;            /* Clear CAN 1 MB 4 flag */
  	return 1;
  }
  if(CAN_0.IFRL.B.BUF05I==1)
  {
  	
    msg->CODE   = CAN_0.BUF[5].CS.B.CODE;    /* Read CODE, ID, LENGTH, DATA, TIMESTAMP */
  	msg->ID     = CAN_0.BUF[5].ID.B.STD_ID;
  	RxLENGTH = CAN_0.BUF[5].CS.B.LENGTH;
  	for (j=0; j<RxLENGTH; j++) { 
    	msg->DATA[j] = CAN_0.BUF[5].DATA.B[j];
  	}
  	msg->LENGTH=8;
  	msg->TIMESTAMP = CAN_0.BUF[5].CS.B.TIMESTAMP; 
  	msg->dummy = CAN_0.TIMER.R;                /* Read TIMER to unlock message buffers */  
  	CAN_0.BUF[5].CS.B.TIMESTAMP=  CAN_0.TIMER.R;  
  	CAN_0.IFRL.R = 0x00000020;            /* Clear CAN 1 MB 4 flag */
  	return 1;
  }
  CAN_0.IFRL.R = 0x00000001;
  return 0;
}