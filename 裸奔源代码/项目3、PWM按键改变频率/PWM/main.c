//按按键k1蜂鸣器频率降低,
//按按键k2蜂鸣器频率升高,
//按按键k3退出蜂鸣器测试
//按按键k4开启蜂鸣器测试

#define GPBCON    (*(volatile unsigned long*)0x56000010)  //rGPBCON是指向0x56000010这个地址空间的一个宏
#define GPBDAT    (*(volatile unsigned long*)0x56000014)  //led GPIO

#define GPGCON    (*(volatile unsigned long*)0x56000060)  //KEY GPIO
#define GPGDAT    (*(volatile unsigned long*)0x56000064)

#define TCFG0     (*(volatile unsigned long*)0x51000000)  //PWM定时器
#define TCFG1     (*(volatile unsigned long*)0x51000004)
#define TCON      (*(volatile unsigned long*)0x51000008)
#define TCNTB0    (*(volatile unsigned long*)0x5100000C)
#define TCMPB0    (*(volatile unsigned long*)0x51000010)

#define PWM_out   (0x01)                      //开启PWM为输出
#define PWM_TOUT0 (10)                        //开启PWM为TOUT0功能，LED输出功能
#define LED1_out  (1<<(5*2))
#define LED2_out  (1<<(6*2))
#define LED3_out  (1<<(7*2))
#define LED4_out  (1<<(8*2))

#define BEEP_OFF  (0)        //蜂鸣器来说1代表发出声音，而0则代表不出声音
#define BEEP_ON   (1)

#define LED_OFF   (0x0F<<5)  //LED                 OFF
#define LED1_ON   (0x0E<<5)  //LED1                ON
#define LED2_ON   (0x0E<<6)  //LED1+LED2           ON
#define LED3_ON   (0x0E<<7)  //LED1+LED2+LED3      ON
#define LED4_ON   (0x0E<<8)  //LED1+LED2+LED3+LED4 ON

#define U8        unsigned char
#define U32       unsigned int


void daly()               //时间延迟函数
{
    U32 t;
    U32 i;
    for(t=888;t>0;t--)
    for(i=0;i<10000;i++);

}

void led_key_init()            //初始化LED和KEY
{
    GPBCON=LED1_out|LED2_out|LED3_out|LED4_out|PWM_TOUT0;                                  //led PWM 设置为输出
    GPGCON= ~( (3<<(2*0))|(3<<(2*3))|(3<<(2*5))|(3<<(2*6)) );                              //GPG0 3 5  6输入  开启按键1 2 3 4功能
    GPBDAT=LED_OFF|BEEP_ON;
}

void PWM_init()                //初始化PWM定时器函数
{
    TCFG0 &= 0xFFFF00;         //初始化预分频值
    TCFG0 |= 0x31;             //prescal=49
    TCFG1 &= ~0xF;             //mux=1/2    ，PCLK为50MHz,定时器输入时钟频率 = PCLK / {预分频值+1} / {分频值}；所以50MHz/(49+1)/2=500kHz
    TCNTB0 = 5000;             //初始化双缓冲器TCNTB0和TCMPB0
    TCMPB0 = 2500;
    TCON &= ~0x1F;             //TCON置零
    TCON |= 0xf;               //死区无效，自动装载，电平反转，手动更新，定时器开启
    TCON &= ~0x2 ;             //手动更新位清零，PWM开始工作
}

void ESC_PWM()
{
    GPBCON = ~0x3;             //关闭GPB0的TOUT0功能，
    GPBCON = PWM_out;          //设置GPB0为01输出功能
    GPBDAT = BEEP_OFF;         //设置GPB0数据寄存器为0，对蜂鸣器来说1代表发出声音，而0则代表不出声音
}

void OPEN_PWM()
{
    GPBCON=LED1_out|LED2_out|LED3_out|LED4_out|PWM_TOUT0;  //重置GPB控制器和数据寄存器
    GPBDAT=LED_OFF|BEEP_ON;
}


void led2_1_on()                              //LED1 2；1点亮
{
    GPBDAT=LED2_ON; daly();
    GPBDAT=LED_OFF; daly();
    GPBDAT=LED1_ON; daly();
    GPBDAT=LED_OFF;
}

void led3_4_on()                              //LED3 ; 3 4点亮
{
    GPBDAT=LED3_ON; daly();
    GPBDAT=LED_OFF; daly();
    GPBDAT=LED4_ON; daly();
    GPBDAT=LED_OFF;
}

int Main()
{
    U32  freq=2500;
    led_key_init();         //初始化LED和KEY
    PWM_init();             //初始化PWM定时器

    while(1)
    {
        if( !(GPGDAT &( 1<<0 )) ){      // K1  PWM值降低
            freq =freq+300;
            if(freq>5000) freq=2500;

            TCMPB0 = freq;             //数据手册里说：PWM功能可以通过使用TCMPBn实现。PWM频率由TCNTBn决定。
                                       //这里通过改变TCMPB0来改变PWM的值。
                                       //减小TCMPBn可以提高PWM值。增大TCMPBn可以降低PWM值。
            led2_1_on();
            } else

        if(!(GPGDAT &( 1<<3 )) ){      // K2   PWM值增加
            freq =freq-300;
            if(freq<1000) freq=2500;
            TCMPB0 = freq;
            led3_4_on();
            } else

        if(!(GPGDAT &( 1<<5 )) ){      // K3   关闭蜂鸣器输出
            ESC_PWM();
            } else

        if(!(GPGDAT &( 1<<6 )) ){      // K4   打开蜂鸣器输出
            OPEN_PWM();
            }
    }
    return 0;
}
