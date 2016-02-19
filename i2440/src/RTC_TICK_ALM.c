//=====================================================================
//  实现功能：
//          RTC开节拍中断、闹钟中断。
//          节拍中断——串口输出时间XXXX年XX月XX日XX时XX分XX秒 和 LED闪亮
//          闹钟中断——beep声  和 LED亮  5秒         
//                                                      by：梁惠涌
//=====================================================================

#include "2440addr.h"     //引脚宏定义
#include "def.h"          // U8 U32宏定义
#include "2440lib.h"      //使用Uart_Printf,Dalay声明，Uart_Printf定义在2440lib.c文件
#include "option.h"
#include "mmu.h"

#define LED_OFF  (0x0f<<5)
#define LED_ON   (~0x0f<<5)

struct Time{          //RTC时间结构
    U32 year;
    U8 month;
    U8 day;
    U8 week;
    U8 hour;
    U8 mi;
    U8 sec;
}ttime_rtc;

void RTC_set()
{
    rRTCCON |=0x01;

    rBCDSEC    =0x0;   //14年6月12日1点1分0秒 星期4
    rBCDMIN    =0x01;
    rBCDHOUR   =0x01;
    rBCDDAY    =0x4;    //星期
    rBCDDATE   =0x12;
    rBCDMON    =0x6;
    rBCDYEAR   =0x14;

    rRTCCON &=~0x01;
}

void Led_Init(){
    rGPBCON=0x015400;  //GPB5 GPB6 GPB7 GPB8 初始化为输出
    rGPBDAT=LED_OFF;   //熄灭状态
}

//从RTC读取值
void read_for_rtc()
{
    rRTCCON |=0x01;  //RTCCON只控制BCD寄存器，ALM数据寄存器就不需要读写控制了

    ttime_rtc.year  =0x2000+rBCDYEAR;
    ttime_rtc.month =rBCDMON;
    ttime_rtc.day   =rBCDDATE;
    ttime_rtc.week  =rBCDDAY;
    ttime_rtc.hour  =rBCDHOUR;
    ttime_rtc.mi    =rBCDMIN;
    ttime_rtc.sec   =rBCDSEC;

    rRTCCON &=~0x01;
}

void ALM_set(){      // 闹钟赋值
   

    rALMYEAR =0x14;
    rALMMON  =0x06;
    rALMDATE =0x12;
    rALMHOUR =0x01;
    rALMMIN  =0x01;
    rALMSEC  =0x03;
    
}

void RTC_display(){
    Uart_Printf("\n");
    Uart_Printf("\n<><><><><><><><><><><><><><><><><><><><><><><><><><><><>\n");
	Uart_Printf("**************HELLO RTC闹钟中断、节拍中断*****************\n\n");
	Uart_Printf("#rTICNT:   (1<<7) | 127   允许节拍使能\n",rTICNT);
	Uart_Printf("#rRTCALM:  0b1000001      允许闹钟使能 秒精度闹钟\n",rRTCALM);
    Uart_Printf("#rINTMSK:  ~(0x1<<8)      开启INT_TICK中断源服务\n");
	Uart_Printf("#rINTMSK:  ~(0x1<<30)     开启INT_RTC 中断源服务\n");
	Uart_Printf("#rSRCPND    rINTPND        中断控制器清除相应位再关闭\n");
	Uart_Printf("#\n#通过RTC节拍中断，串口输出开发板系统时间\n");	  
    Uart_Printf("#XXXX年XX月XX日XX时XX分XX秒 | LED闪亮\n");
    Uart_Printf("#并且可设置闹钟，闹钟到时，beep声|LED亮\n");	
    Uart_Printf("<><><><><><><><><><><><><><><><><><><><><><><><><><><><>\n\n\n\n");		
}

void __irq tick_interrupt(){
    rSRCPND|=0x1<<8;   //清除中断挂起状态
    rINTPND|=0x1<<8;
    
	//中断功能代码块
	
	
	rGPBDAT =~(rGPBDAT>>5)<<5 ;	//LED灯 亮闪状态切换(这样运算不会影响到蜂鸣器)
	read_for_rtc();
    Uart_Printf("\n  RTC time: %x年%2x月%02x日--%02x:%02x:%02x   星期%d",ttime_rtc.year,ttime_rtc.month,ttime_rtc.day,ttime_rtc.hour,ttime_rtc.mi,ttime_rtc.sec,ttime_rtc.week);
	 
    //rTICNT  &= ~(1<<7); //当只使用一次中断的时候关闭模块使能
    rSRCPND &=(0x1<<8);
    rINTPND &=(0x1<<8);
    
     }

void __irq alm_interrupt(){   //中断入口函数
    
    rSRCPND|=0x1<<30;
    rINTPND|=0x1<<30;
    
	//中断功能代码块
    
    Uart_Printf("\n\n ************************************* ");
    Uart_Printf("\n           闹钟时刻！5秒 ");
    Uart_Printf("\n ************************************* \n");
    rGPBDAT=LED_ON;        //点亮LED
    Beep(2000, 4000);      //  由于产生中断会有一秒的时间，所以实际是用了4+1秒这个中断
    
    
                           //  注意点（一）
    rSRCPND &=(0x1<<30);   //  当产生中断的时候，必须在开了中断功能后关闭中断清除
    rINTPND &=(0x1<<30);   //  而且关闭中断清除的控制必须在此中断入口函数由此中断控制
                           //  (rSRCPND rINTPND置零,包括没用到的rSUBSRCPND)都应该在中断
                           //  入口函数里操作，而且入口函数最后也别忘了关闭中断清除
                           //  （rSRCPND rINTPND,rSUBSRCPND置零）
                           //  如果没有关闭中断，将无限实现闹钟中断功能，其他中断不能进行
     }


void RTC_TICK(){
    
    rTICNT |= (1<<7) | 127; //节拍中断使能 计时寄节拍设置1s
    
                          //  注意点（二）
	rINTMSK &= ~(0x1<<8); //  中断屏蔽开启可服务(此中断使能不能放在中断入口函数内，因
	                      //  为中断使能并不是中断的操作，而是进中断前的一个控制)
	                      
    pISR_TICK=(unsigned)tick_interrupt; //告诉中断处理中断函数的入口地址                                                   	
	}
	
void RTC_ALM(){

	rRTCALM = 0x41;        //全局闹钟使能，秒闹钟使能(0b1000001)
	
	rINTMSK &= ~(0x1<<30);
	pISR_RTC =(unsigned)alm_interrupt;
	 		
	}
	
void RTC_Display_TICK_ALM()   //    子main函数
{
    static U32 save_E, save_PE, save_INT, save_TIC, save_RAL, save_RC; 
//**********************保护现场************************** 
    save_E   = rGPECON;  
    save_PE  = rGPEUP;
    save_INT = rINTMSK;
    save_TIC = rTICNT;
    save_RAL = rRTCALM;
    save_RC  = rRTCCON;
//********************************************************
    
    //MMU_EnableICache();这一点至关重要， 不要关闭mmu，否则中断不能正常使用
	MMU_Init();
	Led_Init();        //led1 2 3 4输出，初始化熄灭状态
    RTC_display();     //打印相应信息
    
    RTC_set();         //rtc赋值
    ALM_set();         // ALM闹钟赋值    
 
    RTC_TICK();        //节拍中断
    RTC_ALM();         //闹钟中断      
    
    //while(1)
    Delay(100);Uart_Printf("\n\nq:退出\n");
    while(Uart_GetKey() != 'q'){           
    }
    
    
//**********************还原现场**************************       
    rGPEUP  = save_PE;  
    rGPECON = save_E;
    rINTMSK = save_INT;
    rTICNT  = save_TIC;
    rRTCALM = save_RAL;
    rRTCCON = save_RC;
//********************************************************  
}

