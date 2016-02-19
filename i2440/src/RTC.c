//====================================================================
//  实现功能：
//          通过RTC实时时钟，串口输出开发板系统时间
//          “XXXX年XX月XX日--XX:XX:XX”,
//                                                   by：梁惠涌
//====================================================================

#include "2440addr.h"     //引脚宏定义
#include "def.h"          // U8 U32宏定义
#include "2440lib.h"      //使用Uart_Printf,Dalay声明，Uart_Printf定义在2440lib.c文件
#include <stdio.h>
struct Time{          //RTC时间结构
    U32 year;
    U8 month;
    U8 day;
    U8 week;
    U8 hour;
    U8 min;
    U8 sec;
}time_rtc;

void RTC_init()
{
    rRTCCON |=0x01;
    
    rBCDSEC    =0x0;
    rBCDMIN    =0x01;
    rBCDHOUR   =0x01;
    rBCDDAY    =0x4;    //星期
    rBCDDATE   =0x12;
    rBCDMON    =0x6;
    rBCDYEAR   =0x14;
    
    rRTCCON &=~0x01;
}

void read_for_RTC()
{
    rRTCCON |=1;
    
    time_rtc.year  =0x2000+rBCDYEAR;
    time_rtc.month =rBCDMON;
    time_rtc.day   =rBCDDATE;
    time_rtc.week  =rBCDDAY;
    time_rtc.hour  =rBCDHOUR;
    time_rtc.min   =rBCDMIN;
    time_rtc.sec=rBCDSEC;
    
    rRTCCON &=~1; 
}


void RTC_time()     //RTC_time功能子main函数
{
    RTC_init();
    read_for_RTC();
    Uart_Printf("\n RTC time: %x年%2x月%02x日--%02x:%02x:%02x   星期%d",time_rtc.year,time_rtc.month,time_rtc.day,time_rtc.hour,time_rtc.min,time_rtc.sec,time_rtc.week);
    Delay(1000);
    //while(1)
    Delay(100);Uart_Printf("\n\nq:退出\n");
    while(Uart_GetKey() != 'q'){
     read_for_RTC();
     Uart_Printf("\n RTC time: %x年%2x月%02x日--%02x:%02x:%02x   星期%d",time_rtc.year,time_rtc.month,time_rtc.day,time_rtc.hour,time_rtc.min,time_rtc.sec,time_rtc.week);
     Delay(1000);
    }
}














