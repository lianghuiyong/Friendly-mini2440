#define	GLOBAL_CLK		1

#include "def.h"
#include <stdio.h>
#include "option.h"
#include "2440addr.h"
#include "2440lib.h"  //函数声明
#include "2440slib.h"
#include "mmu.h"
#include "profile.h"


//功能函数声明 
extern void RTC_Display_TICK_ALM(void);
extern void RTC_time(void);
extern void ADC_read_W1(void);
extern void LCD_Calculator(void);
extern void DMA_UART(void);
extern void watchdog(void);
extern void Uart(void); 
extern void music_player(void);
extern void IIC(void);



void display(){
    Uart_Printf("\n\n\n\n\n");
	Uart_Printf("<><><><><><><><><><><><><><><><><><><><><><><><><><\n");
	Uart_Printf("<            Hello2440 (1--9选择)                 >\n");
	Uart_Printf("< 1: 实时时钟RTC  非中断模式显示                  >\n");
	Uart_Printf("< 2: 实时时钟RTC  闹钟模中断+报警中断             >\n");
	Uart_Printf("< 3: A/D转换器串口输出 LED模拟信号频率            >\n");
	Uart_Printf("< 4: LCD计算器    LCD显示,触摸屏中断              >\n");
	Uart_Printf("< 5: DMA直接存取  实现Uart（串口）通信            >\n");
	Uart_Printf("< 6: 看门狗中断  (4s一次  复位操作需改源码)       >\n");
	Uart_Printf("< 7: 串口传送文件 字符串到开发板,终端显示         >\n");
	Uart_Printf("< 8: 音乐播放器--IIS音频数据传输 UDA1341信号转换  >\n");
	Uart_Printf("< 9: IIC总线控制，EEPROM写入数据并输出到data数组  >\n");
	Uart_Printf("<                                                 >\n");
	Uart_Printf("< By:梁惠涌                     Hello2440 VER 1.0 >\n");
	Uart_Printf("<                 http://blog.csdn.net/muyang_ren >\n");
	Uart_Printf("<><><><><><><><><><><><><><><><><><><><><><><><><><\n");
	Uart_Printf("\n");
    Delay( 500 ) ;
}

			

void Main(void)
{   
    U8 num_i;          
    U32 mpll_val = 0,consoleNum;
    Port_Init();
    mpll_val = (92<<12)|(1<<4)|(1);
	
	//init FCLK=400M,
	ChangeMPllValue((mpll_val>>12)&0xff, (mpll_val>>4)&0x3f, mpll_val&3);
	ChangeClockDivider(14, 12);   //the result of rCLKDIVN [0:1:0:1] 3-0 bit
	cal_cpu_bus_clk();            //HCLK=100M   PCLK=50M
   
/*********************************************
当做串口实验时，屏蔽官方串口初始化函数
**********************************************/
    consoleNum = 0;	  // Uart 0 select for debug.
	Uart_Init( 0,115200 );
	Uart_Select( consoleNum );
	
	Port_Init();
	MMU_Init(); //地址映射初始化
	
	Beep(2000, 100); 
	  
//>>>>>>>>>>>>>>以下是功能代码块入口<<<<<<<<<<<<<<<<<<<
    while(1){
        display();                  //界面
      
        num_i = Uart_GetIntNum_GJ() ;	
       
        switch(num_i){ 
		 case 1:RTC_time();break;                  //不采用中断显示实时时钟(调试成功！)
		 case 2:RTC_Display_TICK_ALM();break;      //闹钟中断显示实时时钟，报警中断(调试成功！)
		 case 3:ADC_read_W1();break;               //模拟信号经过A/D转换器串口输出，LED以模拟信号频率点亮时长(调试成功！)
		 case 4:LCD_Calculator();break;            //计算器--LCD显示,触摸屏中断(调试成功！)
		 case 5:DMA_UART();break;                  //DMA方式实现Uart（串口）通信(调试成功！)
		 case 6:watchdog();break;                  //看门狗中断和复位操作(调试成功！)
		 case 7:Uart();break;                      //实现串口传送文件 字符串到开发板，并显示在串口终端(调试成功！) 
		 case 8:music_player();break;              //音乐播放器--IIS音频数据传输 UDA1341芯片DA转换成耳线口电流信号（调试成功！）
		 case 9:IIC();break;                       //IIC总线控制，EEPROM写入数据并输出到data数组(调试成功！)
         default:Delay(300);Uart_Printf("\n请输入1--9\n"); Delay(1000);
        }
    }
}


