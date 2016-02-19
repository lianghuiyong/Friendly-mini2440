//====================================================================
//  实现功能：
//          通过开发板上的滑动变阻器控制的模拟信号经过A/D转换器
//          转换为数字信号用uart_printf()打印在串口上。并且滑动
//          变阻器同时控制指示灯led的闪烁速度(时长)。
//                                                   by：梁惠涌
//====================================================================

#include "2440addr.h"     //引脚宏定义
#include "def.h"          // U8 U32宏定义
#include "2440lib.h"      //使用Uart_Printf,Dalay声明，Uart_Printf定义在2440lib.c文件

/* 
当PRSCEN为1时，设置PRSCVL的值将会决定AD转换的时间。ADC使用的时钟是系统的PCLK
，由于main函数设置mpll为400MHZ,所以PCLK = 50MHz，设置PRSCVL的值为49，那么：

A/D 转换的频率 = 50MHz / （49+1） = 1 MHz

A/D 转换的时间 = 1 /（1MHz / 5cycles）=5 uS
*/
/********ADC********/
#define PRSC_EN     0x1           //允许预分频
#define PRSCVL      0x19          //预分频值
#define SEL_MUX     ~0x7          //模拟通道选择AIN0通道
#define STDBM       0x0           //正常工作模式
#define READ_START  0x0           //读数时不进行A/D转换
#define ADC_STAR    0x1           //ADC开启

/********LED*********/
#define LED1_out  (1<<(5*2))
#define LED2_out  (1<<(6*2))
#define LED3_out  (1<<(7*2))
#define LED4_out  (1<<(8*2))
#define LED4_ON   (0x0E<<8)       //LED1+LED2+LED3+LED4 ON
#define LED_OFF   (0x0F<<5)

void ADC_display(){
    Uart_Printf("\n");
    Uart_Printf("\n");
    Uart_Printf("\n");
	Uart_Printf("---------HELLO ADC\n");
	Uart_Printf("PRSC_EN:  %5x  允许预分频\n",PRSC_EN);
	Uart_Printf("PRSCVL:   %5x  预分频值\n",PRSCVL);
	Uart_Printf("SEL_MUX:   ~0x7  模拟通道选择AIN0通道\n",SEL_MUX);
    Uart_Printf("STDBM:    %5x  正常工作模式\n",STDBM);
	Uart_Printf("READ_START:%4x  读数时不进行A/D转换\n",READ_START);
	Uart_Printf("ADC_STAR:  %4x  ADC开启\n",ADC_STAR);
	Uart_Printf("变阻器控制的模拟信号经过A/D转换器转换为\n");	  
    Uart_Printf("数字信号用串口显示,并且滑动变阻器同时控\n");
    Uart_Printf("制指示灯led的闪烁速度(时长)\n");	
    Uart_Printf("#########################################\n\n");		
}

//初始化AD控制寄存器
void ADC_INIT(){
   rADCCON&=(~((0x1<<14)|(0xff<<6)|(0x7<<3)|(0x1<<2)|(0x1<<1)|(0x1<<0)));
   rADCCON=(PRSC_EN<<14)|(PRSCVL<<6)|(SEL_MUX<<3)|(STDBM<<2)|(READ_START<<1);
   rADCCON|=(ADC_STAR<<0);          //开启ADC（开启ADC后，此位会自动清零）

}

//初始化LED,四个LED输出
void LED_INIT(){                    
    rGPBCON=LED1_out|LED2_out|LED3_out|LED4_out;
}

//LED函数
void W1_LED(int get_data0){         
    rGPBDAT= LED4_ON;     //点亮四个LED
    Delay(get_data0);     //ADC后十位时延函数
    rGPBDAT= LED_OFF;     //熄灭LED
}

//串口打印变阻器模拟信号数值
void W1_ADC(int *read_xpdata){
	ADC_INIT(); //初始化AD控制寄存器
    /*****串口打印ADC数据函数****/
    while(rADCCON & 0x1);                //因为成功启动A/D转换后，该位会
                                          //自动清零，因此在这里检查ADC是否真正启动
    while(!(rADCCON & (1<<15)));            //使用查询方式等待ADC转换结束
    *read_xpdata=(int)(rADCDAT0&0x3ff);   //读取ADCDAT0后十位
    Uart_Printf("\n HELLO ADC# W1值为： %04d ",*read_xpdata);//串口输出ADCDAT0后十位值
    Delay(500);
	}

//子main函数
void ADC_read_W1(){                     
    static U32 save_A, save_B, save_BD;
    int read_xpdata0;
     
//**********************保护现场************************** 
    save_A   = rADCCON;  
    save_B   = rGPBCON;
    save_BD  = rGPBDAT;
//********************************************************
    
    ADC_display();  
    LED_INIT();  //初始化LED
   
    //while(1)
    Delay(100);Uart_Printf("\n\nq:退出\n");
    while(Uart_GetKey() != 'q')
    {
     /*串口打印变阻器模拟信号数值*/
     W1_ADC( &read_xpdata0 );//传递read_xpdata0地址，改变read_xpdata0的值来给W1_LED函数使用
    
     /*****LED点亮函数*****/
     W1_LED(read_xpdata0);                 //LED函数
     
    }
    
//**********************还原现场**************************         
    rADCCON = save_A;
    rGPBCON = save_B;
    rGPBDAT = save_BD;
//********************************************************    

}