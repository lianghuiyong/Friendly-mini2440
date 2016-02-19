//
//**********************************************************
//*****************此函数为实现*****************************
//**led1亮>>>led1灭>>>                                    **
//**>>>led1、led2亮>>>led1、led2灭>>>                     **
//**>>>led1、led2、led3亮>>>ed1、led2、led3灭             **
//**>>>led1、led2、led3、led4亮>>>Led1、led2、led3、led4灭**
//
//
//
#define rGPBCON  (*(volatile unsigned long*)0x56000010)  //rGPBCON是指向0x56000010这个地址空间的一个宏
#define rGPBDAT  (*(volatile unsigned long*)0x56000014)
#define LED_OFF  (0x0f<<5)
#define U8       unsigned char
#define U32      unsigned int

//实现时间间隔函数
void daly()
{
    U32 t;
    U32 i;
    for(t=888;t>0;t--){
    for(i=0;i<10000;i++){}
    }
}

int Main()
{
U8  i;
U8  datastar;
U32 GPBDAT;
datastar=0x0f;     //0x0f=00001111
rGPBCON=0x015400;  //GPB5 GPB6 GPB7 GPB8 初始化为输出
while(1){
    GPBDAT=(datastar<<5);
	for(i=0;i<4;i++){
	    //点亮LED
	    rGPBDAT=(GPBDAT<<1);//四个循环的左移一位实现四种点亮模式
	    GPBDAT=rGPBDAT;
	    daly();
	    //熄灭LED
	    rGPBDAT=LED_OFF;
	    daly();
	}

}

return 0;
}
