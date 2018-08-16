#include "MPC5604C_M27V.h"
#include "UJA1078.h"

#define MISO  36
#define MOSI  37
#define CS    35
#define CLK   34

//#define INT   76
//#define RET   

#define CS_H  SIU.GPDO[CS].R=1
#define CS_L  SIU.GPDO[CS].R=0

#define CLK_H SIU.GPDO[CLK].R=1
#define CLK_L SIU.GPDO[CLK].R=0

 
#define MOSI_H SIU.GPDO[MOSI].R=1
#define MOSI_L SIU.GPDO[MOSI].R=0


#define R_MISO SIU.GPDI[MISO].R


#define DELAY_TIME 300

#define DELAY() spi_io_delay()

static void spi_io_delay(void)
{
	int k=0;
	for (k = 0; k <= DELAY_TIME; k++)
	{
		
	}
}


void init_port(void)
{
   SIU.PCR[MOSI].R = 0x0202;
   SIU.PCR[MISO].R = 0x0200; 
   SIU.PCR[CLK].R  = 0x0202;
   SIU.PCR[CS].R   = 0x0202;
}


void UJA1078_Init(void)
{	
	init_port();
	CLK_L;
	MOSI_L;
	
	DELAY();
	CS_L;
	DELAY();
		
}

static void UJA1078_WR_ST(void)
{
	CLK_L;
	MOSI_L;
	
	DELAY();
	CS_L;
	DELAY();	
}

static void UJA1078_WR_STOP(void)
{
	
	DELAY();
	CS_H;
	DELAY();	
}
static void UJA1078_WRITE(unsigned short data,int bitlen)
{
	int i=0;
	for(i=0;i<bitlen;i++)
	{
		CLK_H;
		DELAY();
		if(data&0x8000)
		{
			MOSI_H;
		}
		else
		{
			MOSI_L;
		}		
		data = data<<1;
		DELAY();
		CLK_L;
		DELAY();
	}
}
static unsigned short UJA1078_READ(unsigned short cmd,int bitlen)
{
	int i=0;
	unsigned short data =0 ;
	for(i=0;i<bitlen;i++)
	{
		CLK_H;
		DELAY();
		if(R_MISO)
		{
			data|=0x0001;	
		}
		else
		{
			data&0xfffe;	
		}
		if(cmd&0x8000)
		{
			MOSI_H;
		}
		else
		{
			MOSI_L;
		}
		data=data<<1;
		cmd=cmd<<1;
		
		DELAY();
		CLK_L;
		DELAY();
	}
	return (data);
}
unsigned short g_mode_register=0;


#define RO_MODE_REGISTER  0x2D00


void UJA1078_GOIN_NOMAL(void)
{
	UJA1078_WR_ST();
	
	//UJA1078_WRITE();
	g_mode_register = UJA1078_READ(RO_MODE_REGISTER ,16);
	
	UJA1078_WR_STOP();	
}





