//===================================================================
// File Name : 2440lib.c
// Function  : S3C2440 PLL,Uart, LED, Port Init
// Date      : March 20, 2002
// Version   : 0.0
// History
//   0.0 : Programming start (February 20,2002) -> SOP
//===================================================================

#include "def.h"
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"
#include "2440slib.h" 

#include <stdarg.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>



//***************************[ PORTS ]****************************************************
void Port_Init(void)
{
    //CAUTION:Follow the configuration order for setting the ports. 
    // 1) setting value(GPnDAT) 
    // 2) setting control register  (GPnCON)
    // 3) configure pull-up resistor(GPnUP)  

    //32bit data bus configuration  
    //*** PORT A GROUP
    //Ports  : GPA22 GPA21  GPA20 GPA19 GPA18 GPA17 GPA16 GPA15 GPA14 GPA13 GPA12  
    //Signal : nFCE nRSTOUT nFRE   nFWE  ALE   CLE  nGCS5 nGCS4 nGCS3 nGCS2 nGCS1 
    //Binary :  1     1      1  , 1   1   1    1   ,  1     1     1     1
    //Ports  : GPA11   GPA10  GPA9   GPA8   GPA7   GPA6   GPA5   GPA4   GPA3   GPA2   GPA1  GPA0
    //Signal : ADDR26 ADDR25 ADDR24 ADDR23 ADDR22 ADDR21 ADDR20 ADDR19 ADDR18 ADDR17 ADDR16 ADDR0 
    //Binary :  1       1      1      1   , 1       1      1      1   ,  1       1     1      1         
    rGPACON = 0x7fffff; 

    //**** PORT B GROUP
    //Ports  : GPB10    GPB9    GPB8    GPB7    GPB6     GPB5    GPB4   GPB3   GPB2     GPB1      GPB0
    //Signal : nXDREQ0 nXDACK0 nXDREQ1 nXDACK1 nSS_KBD nDIS_OFF L3CLOCK L3DATA L3MODE nIrDATXDEN Keyboard
    //Setting: OUTPUT  OUTPUT   OUTPUT  OUTPUT   OUTPUT   OUTPUT   OUTPUT OUTPUT OUTPUT   OUTPUT    OUTPUT 
    //Binary :   01  ,  01       01  ,   01      01   ,  01       01  ,   01     01   ,  01        01  
    rGPBCON = 0x155555;
    rGPBUP  = 0x7ff;     // The pull up function is disabled GPB[10:0]

    //*** PORT C GROUP
    //Ports  : GPC15 GPC14 GPC13 GPC12 GPC11 GPC10 GPC9 GPC8  GPC7   GPC6   GPC5 GPC4 GPC3  GPC2  GPC1 GPC0
    //Signal : VD7   VD6   VD5   VD4   VD3   VD2   VD1  VD0 LCDVF2 LCDVF1 LCDVF0 VM VFRAME VLINE VCLK LEND  
    //Binary :  10   10  , 10    10  , 10    10  , 10   10  , 10     10  ,  10   10 , 10     10 , 10   10
    rGPCCON = 0xaaaaaaaa;       
    rGPCUP  = 0xffff;     // The pull up function is disabled GPC[15:0] 

    //*** PORT D GROUP
    //Ports  : GPD15 GPD14 GPD13 GPD12 GPD11 GPD10 GPD9 GPD8 GPD7 GPD6 GPD5 GPD4 GPD3 GPD2 GPD1 GPD0
    //Signal : VD23  VD22  VD21  VD20  VD19  VD18  VD17 VD16 VD15 VD14 VD13 VD12 VD11 VD10 VD9  VD8
    //Binary : 10    10  , 10    10  , 10    10  , 10   10 , 10   10 , 10   10 , 10   10 ,10   10
    rGPDCON = 0xaaaaaaaa;       
    rGPDUP  = 0xffff;     // The pull up function is disabled GPD[15:0]

    //*** PORT E GROUP
    //Ports  : GPE15  GPE14 GPE13   GPE12   GPE11   GPE10   GPE9    GPE8     GPE7  GPE6  GPE5   GPE4  
    //Signal : IICSDA IICSCL SPICLK SPIMOSI SPIMISO SDDATA3 SDDATA2 SDDATA1 SDDATA0 SDCMD SDCLK I2SSDO 
    //Binary :  10     10  ,  10      10  ,  10      10   ,  10      10   ,   10    10  , 10     10  ,     
    //-------------------------------------------------------------------------------------------------------
    //Ports  :  GPE3   GPE2  GPE1    GPE0    
    //Signal : I2SSDI CDCLK I2SSCLK I2SLRCK     
    //Binary :  10     10  ,  10      10 
    rGPECON = 0xaaaaaaaa;       
    rGPEUP  = 0xffff;     // The pull up function is disabled GPE[15:0]

	
    //*** PORT F GROUP
    //Ports  : GPF7   GPF6   GPF5   GPF4      GPF3     GPF2  GPF1   GPF0
    //Signal : nLED_8 nLED_4 nLED_2 nLED_1 nIRQ_PCMCIA EINT2 KBDINT EINT0
    //Setting: Output Output Output Output    EINT3    EINT2 EINT1  EINT0
    //Binary :  01      01 ,  01     01  ,     10       10  , 10     10
    rGPFCON = 0x55aa;
    rGPFUP  = 0xff;     // The pull up function is disabled GPF[7:0]

    //*** PORT G GROUP
    //Ports  : GPG15 GPG14 GPG13 GPG12 GPG11    GPG10    GPG9     GPG8     GPG7      GPG6    
    //Signal : nYPON  YMON nXPON XMON  EINT19 DMAMODE1 DMAMODE0 DMASTART KBDSPICLK KBDSPIMOSI
    //Setting: nYPON  YMON nXPON XMON  EINT19  Output   Output   Output   SPICLK1    SPIMOSI1
    //Binary :   11    11 , 11    11  , 10      01    ,   01       01   ,    11         11
    //-----------------------------------------------------------------------------------------
    //Ports  :    GPG5       GPG4    GPG3    GPG2    GPG1    GPG0    
    //Signal : KBDSPIMISO LCD_PWREN EINT11 nSS_SPI IRQ_LAN IRQ_PCMCIA
    //Setting:  SPIMISO1  LCD_PWRDN EINT11   nSS0   EINT9    EINT8
    //Binary :     11         11   ,  10      11  ,  10        10
    rGPGCON = 0xff95ffba;
    rGPGUP  = 0xffff;    // The pull up function is disabled GPG[15:0]

    //*** PORT H GROUP
    //Ports  :  GPH10    GPH9  GPH8 GPH7  GPH6  GPH5 GPH4 GPH3 GPH2 GPH1  GPH0 
    //Signal : CLKOUT1 CLKOUT0 UCLK nCTS1 nRTS1 RXD1 TXD1 RXD0 TXD0 nRTS0 nCTS0
    //Binary :   10   ,  10     10 , 11    11  , 10   10 , 10   10 , 10    10
    rGPHCON = 0x2afaaa;
    rGPHUP  = 0x7ff;    // The pull up function is disabled GPH[10:0]

    // Added for S3C2440, DonGo
    //PORT J GROUP
    //Ports	:  GPJ12    GPJ11     GPJ10	GPJ9  GPJ8      GPJ7	GPJ6  GPJ5	GPJ4  GPJ3  GPJ2  GPJ1  GPJ0
    //Signal : CAMRESET CAMCLKOUT CAMHREF CAMVS CAMPCLKIN CAMD7 CAMD6 CAMD5 CAMD4 CAMD3 CAMD2 CAMD1 CAMD0
    //Setting: Out      Out      CAMHREF CAMVS CAMPCLKIN CAMD7 CAMD6 CAMD5 CAMD4 CAMD3 CAMD2 CAMD1 CAMD0
    //Binary : 01	  01        10      10    10        10    10    10    10    10    10    10    10
    //PU_OFF : 1	  0 		1	    1     1         1     1     1		1	1     1     1     1
    //---------------------------------------------------------------------------------------
    rGPJDAT = (1<<12)|(0<<11);
    rGPJCON = 0x016aaaa;
    rGPJUP	= ~((0<<12)|(1<<11));

    rGPJDAT = (0<<12)|(0<<11);
    rGPJCON = 0x016aaaa;
    rGPJUP	= 0x1fff;//~((1<<12)|(1<<11));

	
    //rGPJCON = 0x02aaaaaa;
    //rGPJUP  = 0x1fff;    // The pull up function is disabled GPH[10:0]

    //External interrupt will be falling edge triggered. 
    rEXTINT0 = 0x22222222;    // EINT[7:0]
    rEXTINT1 = 0x22222222;    // EINT[15:8]
    rEXTINT2 = 0x22222222;    // EINT[23:16]
}


//**************************[ BOARD LED ]*********************************
void Led_Display(int data)
{
          //Active is low.(LED On)
          // GPF7  GPF6   GPF5   GPF4
          //nLED_8 nLED4 nLED_2 nLED_1
//    rGPFDAT = (rGPFDAT & 0xf) | !((data & 0xf)<<4);
    rGPFDAT = (rGPFDAT & ~(0xf<<4)) | ((~data & 0xf)<<4);    
}



//*************************[ MPLL ]*******************************
void ChangeMPllValue(int mdiv,int pdiv,int sdiv)
{
	//Led_Display(0x8);

    rMPLLCON = (mdiv<<12) | (pdiv<<4) | sdiv;
	//Led_Display(0x0);
}



void ChangeClockDivider(int hdivn_val,int pdivn_val)
{
	int hdivn=2, pdivn=0;
	
     // hdivn_val (FCLK:HCLK)ratio hdivn
     // 11           1:1       (0)
     // 12           1:2       (1)
     // 13           1:3       (3) 
     // 14           1:4       (2)
     // pdivn_val (HCLK:PCLK)ratio pdivn
     // 11           1:1       (0)
     // 12           1:2       (1)
	switch(hdivn_val) {
		case 11: hdivn=0; break;
		case 12: hdivn=1; break;
		case 13:
		case 16: hdivn=3; break;
		case 14: 
		case 18: hdivn=2; break;
	}
	
	switch(pdivn_val) {
		case 11: pdivn=0; break;
		case 12: pdivn=1; break;
	}
	
	//Uart_Printf("Clock division change [hdiv:%x, pdiv:%x]\n", hdivn, pdivn);
	rCLKDIVN = (hdivn<<1) | pdivn;
	//Uart_Printf("rCLKDIVN:%x]\n", rCLKDIVN);

	switch(hdivn_val) {
		case 16:		// when 1, HCLK=FCLK/6.
			rCAMDIVN = (rCAMDIVN & ~(3<<8)) | (1<<8); 
		break; 
		case 18: 	// when 1, HCLK=FCLK/8.
			rCAMDIVN = (rCAMDIVN & ~(3<<8)) | (1<<9); 
		break;
	}
	//Uart_Printf("rCAMDIVN:%x]\n", rCAMDIVN);
	
    if(hdivn!=0)
        MMU_SetAsyncBusMode();
    else 
        MMU_SetFastBusMode();
}



//**************************[ UPLL ]*******************************
void ChangeUPllValue(int mdiv,int pdiv,int sdiv)
{
    rUPLLCON = (mdiv<<12) | (pdiv<<4) | sdiv;
}




