#ifndef _LED_H_
#define _LED_H_


void ledInit(void);

#define LED0 12
#define LED1 13
#define LED2 14
#define LED3 15

#define LED_ON(x);    SIU.GPDO[x].R=0;
#define LED_OFF(x);   SIU.GPDO[x].R=1;

#endif 