
//#include "MPC5604B_M27V.h"
#include "MPC5604C_M27V.h"
#include "led.h"

void ledInit(void)
{
  
  SIU.PCR[15].R = 0x0220;//PA15 
  SIU.PCR[14].R = 0x0202; 
  SIU.PCR[13].R = 0x0220;
  SIU.PCR[12].R = 0x0220;//PA12 
  
  LED_ON(LED0);
  LED_ON(LED1);
  LED_ON(LED2);
  LED_ON(LED3);
  LED_OFF(SOC_SWITCH);
  //LED_ON(SOC_SWITCH);	
}

