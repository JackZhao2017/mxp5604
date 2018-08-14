
//#include "MPC5604B_M27V.h"
#include "MPC5604C_M27V.h"
#include "led.h"

void ledInit(void)
{
  
  SIU.PCR[15].R = 0x0203;//PA15 
  SIU.PCR[14].R = 0x0203; 
  SIU.PCR[13].R = 0x0203;
  SIU.PCR[12].R = 0x0203;//PA12 
  
  LED_OFF(LED0);
  LED_OFF(LED1);
  LED_OFF(LED2);
  LED_OFF(LED3);	
}

