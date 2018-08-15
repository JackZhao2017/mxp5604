//#include "MPC5604B_M27V.h"
#include "MPC5604C_M27V.h"

static void initModesAndClock(void) {
	ME.MER.R = 0x0000001D; /* Enable DRUN, RUN0, SAFE, RESET modes */
	/* Initialize PLL before turning it on: */
	/* Use 1 of the next 2 lines depending on crystal frequency: */
	CGM.FMPLL_CR.R   = 0x05400100; /* 8 MHz xtal: Set PLL0 to 64 MHz */
	/*CGM.FMPLL_CR.R = 0x12400100;*/ /* 40 MHz xtal: Set PLL0 to 64 MHz */
	
	ME.RUN[0].R = 0x001F0074; /* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL0 */
	
	ME.RUNPC[0].R = 0x000000FE; /* Peri. Cfg. 1 settings: only run in RUN0 mode */
	//ME.RUNPC[1].R = 0x00000010;
	
	ME.PCTL[68].R = 0x00;
	
	ME.PCTL[48].R = 0x00; /* LINflex0 Uart0 */
	
	ME.PCTL[49].R = 0x00; /* LINflex1 Uart1 */
	
    //ME.PCTL[16].R = 0x00; /* CANflex0  */
	
	/* Mode Transition to enter RUN0 mode: */
	ME.MCTL.R = 0x40005AF0; /* Enter RUN0 Mode & Key */
	ME.MCTL.R = 0x4000A50F; /* Enter RUN0 Mode & Inverted Key */
	
	while (ME.GS.B.S_MTRANS == 1) {} /* Wait for mode transition to complete */
	/* Notes: */
	/* 1. I_TC IRQ could be used here instead of polling */
	/* to allow software to complete other init. */
	/* 2. A timer could be used to prevent waiting forever.*/
	while(ME.GS.B.S_CURRENTMODE != 4){} /* Verify RUN0 is the current mode */
	/* Note: This verification ensures a SAFE mode */
	/* tranistion did not occur. SW could instead */
	/* enable the safe mode tranision interupt */
}


static void initOutputClock(void) {
	CGM.OC_EN.B.EN = 1; /* Output Clock enabled (to go to pin) */
	CGM.OCDS_SC.B.SELDIV = 2; /* Output Clock?¡¥s selected division is 2**2 = 4 */
	CGM.OCDS_SC.B.SELCTL = 1; /* MPC56xxB: Output clock select 16 MHz int RC osc */
	SIU.PCR[0].R = 0x0800; /* MPC56xxB: assign port PA[0] pad to Alt Func 2 */
	/* CLKOUT = 16 MHz IRC/4 = 4MHz */
	CGM.OCDS_SC.B.SELCTL = 0; /* MPC56xxB: Assign output clock to XTAL */
	/* CLKOUT = Fxtal/4 = 2 or 10 MHz for 8 or 40 MHx XTAL*/
	CGM.OCDS_SC.B.SELCTL = 2; /* Assign output clock to FMPLL[0] */
	/* CLKOUT = 64 MHz/4 = 4MHz */
}
static void disableWatchdog(void) {
	SWT.SR.R = 0x0000c520; /* Write keys to clear soft lock bit */
	SWT.SR.R = 0x0000d928;
	SWT.CR.R = 0x8000010A; /* Clear watchdog enable (WEN) */
}


void sysclockInit(void)
{
	initModesAndClock();
	initOutputClock();
	disableWatchdog();
}