#define GPBCON    (*(volatile unsigned long*)0x56000010)  //rGPBCON是指向0x56000010这个地址空间的一个宏
#define GPBDAT    (*(volatile unsigned long*)0x56000014)
#define GPGCON    (*(volatile unsigned long*)0x56000060)
#define GPGDAT    (*(volatile unsigned long*)0x56000064)
#define U8        unsigned char
#define U32       unsigned int
#define PWM_out   (1)
#define LED1_out  (1<<(5*2))
#define LED2_out  (1<<(6*2))
#define LED3_out  (1<<(7*2))
#define LED4_out  (1<<(8*2))

#define LED_OFF   (0x0F<<5)

#define BEEP_OFF  (0)        //蜂鸣器来说1代表发出声音，而0则代表不出声音
#define BEEP_ON   (1)

#define LED1_ON   (0x0E<<5)  //LED1                ON
#define LED2_ON   (0x0E<<6)  //LED1+LED2           ON
#define LED3_ON   (0x0E<<7)  //LED1+LED2+LED3      ON
#define LED4_ON   (0x0E<<8)  //LED1+LED2+LED3+LED4 ON

void daly()
{
    U32 t, i;
    for(t=8888;t>0;t--){
    for(i=0;i<3000;i++){} }
}

int Main()
{
    U8 j;
    GPBCON=LED1_out|LED2_out|LED3_out|LED4_out|PWM_out;                                  //led PWM 设置为输出
    GPGCON= ~( (3<<(2*0))|(3<<(2*3))|(3<<(2*5))|(3<<(2*6))|(3<<(2*7))|(3<<(2*11)) );     //GPG0 3 5 6 7 11 输入
    GPBDAT=LED_OFF;

    while(1)
    {
        GPBDAT=LED_OFF;
        if( !(GPGDAT &( 1<<0 )) ){      // K1>>LED1 ON
            GPBDAT=LED1_ON;daly();
            GPBDAT=LED_OFF;         
            } else
         

        if(!(GPGDAT &( 1<<3 )) ){      // K2>>LED1+LED2 ON
            GPBDAT=LED2_ON; daly();
            GPBDAT=LED_OFF;
            } else

        if(!(GPGDAT &( 1<<5 )) ){      // K3>>LED1+LED2+LED3 ON
            GPBDAT=LED3_ON;daly();
            GPBDAT=LED_OFF;
            } else

        if(!(GPGDAT &( 1<<6 )) ){      // K4>>LED1+LED2+LED3+LED4 ON
            GPBDAT=LED4_ON; daly();
            GPBDAT=LED_OFF;
            } else

        if(!(GPGDAT &( 1<<7 )) ){      // K5>>LED1+LED2+LED3+LED4+BEEP ON
            GPBDAT=LED4_ON|BEEP_ON;daly();
            GPBDAT=LED_OFF;
            } else

        if(!(GPGDAT &( 1<<11 )) ){      // K6>>(LED1+LED2+LED3+LED4)*3+BEEP ON
            for(j=0; j<2; j++){
                GPBDAT=LED4_ON;daly();
                GPBDAT=LED_OFF;daly();
                }
            GPBDAT=LED4_ON|BEEP_ON; daly();
            GPBDAT=LED_OFF;
            }

    }
    return 0;
}
