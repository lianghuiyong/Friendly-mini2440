//====================================================================
//  实现功能：
//           看门狗中断和复位操作
//       当WTCON[0] 开启复位功能;                实现复位
//       当WTCON[0] 禁止复位，WTCON[2]开启中断;  实现中断
//                                                   by：梁惠涌
//====================================================================
#include "2440addr.h" 
#include "def.h"
#include "2440lib.h"
 
static U8 led_flag=0;
/**************************************************************
                   看门狗初始化函数
**************************************************************/
void watchdog_init(){
    rWTCON=((77<<8)|(1<<5)|(3<<3)|(1<<2)); //预分频值=77;看门狗定时器使能;时钟除数因子=128 ;看门狗中断使能;
    //时钟周期t_watchdog=1/[PCLK/(预分频值+1)/时钟除数因子]=1/[50*1000000/(77+1)/128]=0.0002
    //t_watchdog表示的是看门狗计数计时器WTCNT每减少1所用的时间
    rWTDAT=20000; //WTDAT看门狗数据寄存器,看门狗定时器重载的计数值  
    rWTCNT=20000; //WTCNT看门狗计数寄存器,看门狗定时器当前计数值
}

/**************************************************************
                        看门狗中断函数
**************************************************************/
void __irq watchdog_isr(){
    rSUBSRCPND |=1<<13; //清除次级中断挂起状态
    rSRCPND|=0x1<<9;    //清除中断挂起状态
    rINTPND|=0x1<<9;
   
   //中断LED点亮 
    led_flag++;
    Uart_Printf("\n    -----看门狗中断，点亮LED %d\n",led_flag);
    rGPBCON=0x015400;
    switch(led_flag){
    case 1: rGPBDAT=0xe<<5 ;break;
    case 2: rGPBDAT=0xd<<5 ;break;
    case 3: rGPBDAT=0xb<<5 ;break;
    case 4: rGPBDAT=0x7<<5 ;break;
    }
    if(led_flag==4) led_flag=0;  
    
    rSUBSRCPND &=(~0x1<<13);
    rSRCPND &=(~0x1<<9);   //开中断
    rINTPND &=(~0x1<<9);
}

/**************************************************************
                       看门狗子函数  
**************************************************************/    
void watchdog(){
    static U32 save_WT, save_INTS, save_INT, save_B, save_BD;    
//**********************保护现场************************** 
    save_WT   = rWTCON;
    save_INTS = rINTSUBMSK;
    save_INT  = rINTMSK;  
    save_B    = rGPBCON;
    save_BD   = rGPBDAT;   
//********************************************************  
  
    watchdog_init(); //初始化看门狗寄存器
    
    //选择是否开启复位
    //rWTCON |= (1<<0);//开启复位
    rWTCON |= (0<<0);//禁止复位
    
    rINTSUBMSK &=~(1<<13);//开看门狗次级中断  
    rINTMSK &=~(1<<9);    //开看门狗中断    
    pISR_WDT_AC97=(unsigned)watchdog_isr;
    
    //while(1)
    Delay(100);Uart_Printf("\n\nq:退出\n");
    while(Uart_GetKey() != 'q'){
    }

//**********************还原现场************************** 
    rWTCON    = save_WT;
   rINTSUBMSK = save_INTS;
   rINTMSK    = save_INT;  
   rGPBCON    = save_B;
   rGPBDAT    = save_BD;   
//******************************************************** 
}