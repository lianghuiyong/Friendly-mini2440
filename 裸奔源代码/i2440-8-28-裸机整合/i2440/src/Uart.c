#include "2440addr.h" 
#include "def.h"
#include "2440lib.h"
#include <stdio.h>   //文件库函数

void Uart_init(){

//以下使用uart0

    rGPHCON&=~((3<<4)|(3<<6));  //GPH2--TXD0;GPH3--RXD0
    rGPHCON|=((2<<4)|(2<<6));    //设置GPH2、GPH3为TXD0、RXD0功能
//手册中GPH2、GPH3描述如下:
//GPHCON    	Bit        	Description
//GPH3  	   [7:6]    	00 = Input   	01 = Output   
//	                 	    10 = RXD0   	11 = reserved
//GPH2         [5:4] 	    00 = Input    	01 = Output
//                   	    10 = TXD0  	    11 = Reserved 
    
    rGPHUP=0x00;                //上拉电阻使能
    rUFCON0 = 0x0;              //禁止FIFO
    rUMCON0 = 0x0;              //撤销nRTS
    rULCON0|=0x03;              //设置UART0数据发送8个数据位
    rUCON0  = 0x5;;           //发送模式和接收模式都使用查询模式
  
    
    //设置波特率，其中波特率作为一个参数传递到该初始化函数
    rUBRDIV0=(int)((50000000/(115200*16))-1);  
    Delay( 10 ) ;
       
}


    

void Uart(){
    int i=0;
    char ch;
    FILE *FP;
   
    static U32 save_H, save_HP, save_U, save_UF0, save_UM0, save_UL0;    
//**********************保护现场************************** 
    save_H   = rGPHCON;
    save_HP  = rGPHUP;
    save_U   = rUCON0;  
    save_UF0 = rUFCON0;
    save_UM0 = rUMCON0;
    save_UL0 = rULCON0;  
//********************************************************                                            //文件指针
  
    FP=fopen("uart.txt","w");  //以写方式打开uart.txt文件
    Uart_init();                                        //uart初始化函数

    //while(1)
    Delay(100);Uart_Printf("\n\nq:退出\n");
    while(Uart_GetKey() != 'q'){
        if(rUTRSTAT0&1) {            //如果接收数据缓存器接收到有效数据
    
         ch = rURXH0;            //ch存储接收字节数据
         rUTXH0=ch;              //PC机将接收的字符通过串口调试工具显示在屏幕上 
		 
		 fputc(ch,FP);                //将ch字符内容存进文件中
         i++; if(i>200) fclose(FP);   //如果有200个字节关闭文件       
         }
    }
//**********************还原现场************************** 
    rGPHCON  = save_H;
    rGPHUP   = save_HP;
    rUCON0   = save_U;  
    rUFCON0  = save_UF0;
    rUMCON0  = save_UM0;
    rULCON0  = save_UL0;  
//******************************************************** 
}