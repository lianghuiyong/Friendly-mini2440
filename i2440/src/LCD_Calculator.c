/***********************************
 实现功能      LCD计算器   
***********************************/

#include "2440lib.h"
#include "2440slib.h" 
#include "LCD_init.h"
#include "2440addr.h" 
#include "mmu.h" 

extern void show_picturu(int num);
void numTOpic(int m ,int n);

extern unsigned char my1_240x320[];
extern unsigned char Calculator[];
extern unsigned char clean[];
extern unsigned char error[];
extern unsigned char numerror[];
extern unsigned char num_[]; //负号
extern unsigned char num0[];
extern unsigned char num1[];
extern unsigned char num2[];
extern unsigned char num3[];
extern unsigned char num4[];
extern unsigned char num5[];
extern unsigned char num6[];
extern unsigned char num7[];
extern unsigned char num8[];
extern unsigned char num9[];

volatile static int xdata, ydata;    //volatile的作用： 作为指令关键字，确保本条指令不会因编译器的优化而省略，且要求每次直接读值.
volatile static int flag1=0 , flag2=0 ;
volatile static long long int sum1=0 , sum2=0;
volatile static int  botten1; 

/**************************************************************
 TFT LCD *触摸屏中断函数*
**************************************************************/

  
void __irq Adc_Tc_Handler(void)  
{  
  
    rADCTSC|=(1<<3)|(1<<2); //XP上拉电阻无效, 自动连续测量X坐标和Y坐标.  
    rADCCON|=(1<<0);//ADC转换开始  
  
    while(rADCCON&(1<<0));//检测ADC转换是否开始且ADCCON[0]自动清0  
    while(!(rADCCON&(0x1<<15))); //检测ADCCON[15]是否为1,ADC转换是否结束,(必须)  
    while(!(rINTPND&((U32)0x1<<31)));//检测ADC中断是否已请求  
      
    xdata=rADCDAT0&0x3ff;//读x坐标 >>xdata并不是像素点，而是模拟信号 0-1000 
    ydata=rADCDAT1&0x3ff;//读y坐标  
      
    Uart_Printf("\n       Xdata=%04d, Ydata=%04d\n", xdata, ydata);  
  
    rSUBSRCPND|=(0x1<<9);   //清除中断
    rSRCPND|=((U32)0x1<<31);  
    rINTPND|=((U32)0x1<<31);  
      
    rADCTSC =0xd3;     //ADC等待中断模式    
    rADCTSC|=(0x1<<8);  //ADCTSC[8]=1,设置抬起中断信号  
      
    while(!(rSUBSRCPND&(0x1<<9)));  //检测触屏抬起中断是否已请求   
  
    rADCTSC &=~(0x1<<8);//ADCTSC[8]=0光标按下中断信号  
    
    //  由于下面这段代码和上面这段代码是看到大神们都会加上去的，调试得到
    //  结论：上面的清除中断实现触屏中断，而下面这段则是加快中断后的响应
    //  (我的实验结果是：没下面的时候，触屏后的数值显示时间变长，蜂鸣器的
    //  响声也变长，有种慢一拍的感觉)
   
    rSUBSRCPND|=(0x1<<9);     
    rSRCPND|=((U32)0x1<<31);  
    rINTPND|=((U32)0x1<<31);    
}   
  
void Touch_Init(void)  
{  
    rADCCON=((1<<14)|(9<<6));    //A/D分频时钟有效，其值为9  
    rADCTSC=0xd3;  //光标按下中断信号,YM有效，YP无效，XM有效，XP无效，XP上拉电阻，普通ADC转换，等待中断模式  
    rADCDLY=50000; //正常转换模式转换延时大约为(1/3.6864M)*50000=13.56ms  
      
    rINTSUBMSK &=~(0x1<<9);//TC中断使能  
    rINTMSK &=~((U32)0x1<<31);//ADC总中断使能  
      
    pISR_ADC=(U32)Adc_Tc_Handler;//指向中断向量表  
      
}  

/**************************************************************
             ------------获取键值--------------
**************************************************************/ 
void get_key(void){
    //数字块
    if( xdata >= 70 && xdata <= 285 && ydata >= 100 && ydata < 262 )       //0
	{   Beep(2000, 100);botten1 = 0;flag1=1;xdata = ydata = 0;  }
	else if( xdata >= 70 && xdata <= 285 && ydata >= 263 && ydata < 425 )    //1 
	{   Beep(2000, 100);botten1 = 1;flag1=1;xdata = ydata = 0;  }
	else if( xdata >= 285 && xdata <= 500 && ydata >= 263 && ydata < 425 )    //2 
	{   Beep(2000, 100);botten1 = 2;flag1=1;xdata = ydata = 0;  }
	else if( xdata >= 500 && xdata <= 715 && ydata >= 263 && ydata < 425 )     //3
	{   Beep(2000, 100);botten1 = 3;flag1=1;xdata = ydata = 0;  }
	else if( xdata >= 70 && xdata <= 285 && ydata >= 425 && ydata < 587 )     //4  
	{   Beep(2000, 100);botten1 = 4;flag1=1;xdata = ydata = 0;  }
	else if( xdata >= 285 && xdata <= 500 && ydata >= 425 && ydata < 587 )     //5
	{   Beep(2000, 100);botten1 = 5;flag1=1;xdata = ydata = 0;  }
	else if( xdata >= 500 && xdata <= 715 && ydata >= 425 && ydata < 587 )     //6
	{   Beep(2000, 100);botten1 = 6;flag1=1;xdata = ydata = 0;  }
	else if( xdata >= 70 && xdata <= 285 && ydata >= 587 && ydata < 750 )     //7
	{   Beep(2000, 100);botten1 = 7;flag1=1;xdata = ydata = 0;  }
	else if( xdata >= 285 && xdata <= 500 && ydata >= 587 && ydata < 750 )     //8
	{   Beep(2000, 100);botten1 = 8;flag1=1;xdata = ydata = 0;  }
	else if( xdata >= 500 && xdata <= 715 && ydata >= 587 && ydata < 750 )     //9
	{   Beep(2000, 100);botten1 = 9;flag1=1;xdata = ydata = 0;  }
	else
	//功能运算块
	if( xdata >= 715 && xdata <= 930 && ydata >= 100 && ydata < 262 )          //除
	{   Beep(2000, 100);botten1 = '/';flag1=1;flag2=1;xdata = ydata = 0;  }
	else if( xdata >= 715 && xdata <= 930 && ydata >= 263 && ydata < 425 )     //乘以
	{   Beep(2000, 100);botten1 = '*';flag1=1;flag2=2;xdata = ydata = 0;  }
	else if( xdata >= 715 && xdata <= 930 && ydata >= 425 && ydata < 587 )     //减法
	{   Beep(2000, 100);botten1 = '-';flag1=1;flag2=3;xdata = ydata = 0;  }
	else if( xdata >= 715 && xdata <= 930 && ydata >= 587 && ydata < 750 )     //加法
	{   Beep(2000, 100);botten1 = '+';flag1=1;flag2=4;xdata = ydata = 0;  }
	else
	if( xdata >= 500 && xdata <= 715 && ydata >= 100 && ydata < 262 )          //等于
	{   Beep(2000, 100);botten1 = '=';flag1=1;xdata = ydata = 0;  }
	else
	if( xdata >= 285 && xdata <= 500 && ydata >= 100 && ydata < 262 )         // 清空
	{   Beep(2000, 100);botten1 = 'c';flag1=1;xdata = ydata = 0;  }	

}

/**************************************************************
             ------------处理键值--------------
**************************************************************/
void resout(){
	
	if(flag2==4)               {sum1=sum1+sum2; show_picturu(sum1);  sum1=0;sum2=0;flag2=0;  }   //根据标志flag2 进行相应的  sum1=sum1 ？ sum2  的运算,运算完sum1 sum2置零
		 else if(flag2==3)     {sum1=sum1-sum2; show_picturu(sum1);  sum1=0;sum2=0;flag2=0;  } 
		      else if(flag2==2){sum1=sum1*sum2; show_picturu(sum1);  sum1=0;sum2=0;flag2=0;  } 
		           else if(flag2==1){
		                    if(sum2==0) {sum1=0;sum2=0;flag2=0;  Pait_Bmp(1, 33, 236, 30, error);Delay(500);show_picturu(0);}  //对除数为0处理。
		                    else {sum1=sum1/sum2;show_picturu(sum1);  sum1=0;sum2=0;flag2=0; }}       //注意:除法运算结果会舍去小数部分
		

}
void cleaning(){
  sum1=0,sum2=0;flag2=0;show_picturu(0);
}
void show_num(void){ 
    while(flag1){  
	flag1=0;   //标志置零
	     switch(botten1){
		 case 0:
		 case 1:
		 case 2:
		 case 3:
		 case 4:
		 case 5:
		 case 6:
		 case 7:
		 case 8:
		 case 9:{sum2=sum2*10+botten1;
		            if(sum2>999999999){
				        sum2=0;Pait_Bmp(2, 33, 236, 30, numerror);
					    Delay(500);show_picturu(sum2);
				    } 
				    else show_picturu(sum2);
				} break;       //按键值存于 sum2
		 
		 case '+':
		 case '-':
		 case '*':
		 case '/': sum1=sum2,sum2=0; break;                                //sum2值转存于sum1，sum2置零
		 
		 case '=': resout(); break;
		 
		 //之前把下面的这段放在这里，观察变量发现sum1和和sum2到这里就置零了(按下=后永远等于0)，无奈之下试试把判断放进一个函数 resout(); 里看看。结果成功了！！！
		/* if(flag2==4)          {show_picturu(sum1+sum2);  sum2=0;flag2=0;}   //根据标志flag2 进行相应的  sum1=sum1 ？ sum2  的运算
		 else if(flag2==3)     {show_picturu(sum1-sum2);  sum2=0;flag2=0;  } 
		      else if(flag2==2){show_picturu(sum1*sum2);  sum2=0;flag2=0;  } 
		           else if(flag2==1){
		                    if(sum2==0) {sum1=0;sum2=0;flag2=0;  Pait_Bmp(1, 33, 236, 30, error);Delay(500);show_picturu(0);}  //对除数为0处理。
		                    else {show_picturu(sum1/sum2);  sum2=0;flag2=0; }}       //注意:除法运算结果会舍去小数部分
		*/
		
		case 'c': cleaning(); break;
		 }
    
    }
}
/**************************************************************
             ------------调用图片函数--------------
**************************************************************/
void show_picturu(int sum){
 int k=0,j=0;
    Pait_Bmp(1, 33, 238, 30, clean);
	if(sum>0){                          //数值大于0时
	    for(;sum>0;){
		    k=sum%10;                   //k用来取余数
            sum=sum/10;                 //num舍去最后一位
			++j;                        //j用来标志k的余数在原数值中的实际位数
            numTOpic(k,j);			
		    }
		}
	else if(sum==0){                    //数值等于0时
	         numTOpic(0,1);
	    }                     
	else if(sum<0){                    //数值小于0时
	         sum=-sum;
	         for(;sum>0;){
		         k=sum%10;                   //k用来取余数
                 sum=sum/10;                 //num舍去最后一位
			     ++j;                        //j用来标志k的余数在原数值中的实际位数
                 numTOpic(k,j);			
		        }
			Pait_Bmp(238-13*(j+1), 33, 13, 30, num_);
	    }

}
void numTOpic(int m ,int n){
switch(m){
    case 0:Pait_Bmp(238-13*n, 33, 13, 30, num0);break;
	case 1:Pait_Bmp(238-13*n, 33, 13, 30, num1);break;
	case 2:Pait_Bmp(238-13*n, 33, 13, 30, num2);break;
	case 3:Pait_Bmp(238-13*n, 33, 13, 30, num3);break;
	case 4:Pait_Bmp(238-13*n, 33, 13, 30, num4);break;
	case 5:Pait_Bmp(238-13*n, 33, 13, 30, num5);break;
	case 6:Pait_Bmp(238-13*n, 33, 13, 30, num6);break;
	case 7:Pait_Bmp(238-13*n, 33, 13, 30, num7);break;
	case 8:Pait_Bmp(238-13*n, 33, 13, 30, num8);break;
	case 9:Pait_Bmp(238-13*n, 33, 13, 30, num9);break;
}
}
/**************************************************************
 TFT LCD     *子main函数*
**************************************************************/
 
void LCD_Calculator(void)
{	
    static U32 save_A, save_C, save_CP, save_D, save_DP,save_G, save_GP,save_INTS,save_INT,
    save_LCD1, save_LCD2, save_LCD3, save_LCD4, save_LCD5 ,save_LCDINT;
     
//**********************保护现场************************** 
    save_A   = rADCCON;
    save_G   = rGPGCON;
    save_GP  = rGPGUP;  
    save_C   = rGPCCON;  
    save_CP  = rGPCUP;
    save_D   = rGPDCON;
    save_DP  = rGPDUP;
    save_LCD1 = rLCDCON1;
    save_LCD2 = rLCDCON2;
    save_LCD3 = rLCDCON3;
    save_LCD4 = rLCDCON4;
    save_LCD5 = rLCDCON5;
    save_LCDINT = rLCDINTMSK;
    save_INTS   = rINTSUBMSK;
    save_INT    = rINTMSK;
//********************************************************

    Lcd_Port_Init(); //端口初始化
    LCD_Init();  //TFT LCD功能模块初始化
	Lcd_PowerEnable(0, 1);  // TFT LCD 电源控制引脚使能
    Lcd_EnvidOnOff(1);		//LCD视频和控制信号输出或者停止，1开启视频输出
   
    
    
    /*红(255:0:0);绿(0:255:0);蓝(0:0:255);黑(0:0:0);白(255,255,255)*/
	
    /*在屏幕上显示三基色*/
		
    Lcd_ClearScr((0x1f<<11) | (0x00<<5) | (0x00));			//red
    Delay(500);
		
    Lcd_ClearScr((0x00<<11) | (0x3f<<5) | (0x00));			//green
    Delay(500);
    
    Lcd_ClearScr((0x00<<11) | (0x00<<5) | (0x1f));			//blue
    Delay(500);

    Lcd_ClearScr( (0x1f<<11) | (0x3f<<5) | (0x1f)  )  ;		//clear screen white
    Delay(500);
	
    /*显示一副图片在屏幕上*/
    Pait_Bmp(0, 0, 240, 320, my1_240x320);
    Delay(1000);
    Pait_Bmp(0, 0, 240, 320, Calculator);
    Delay(1000);
    Pait_Bmp(225, 33, 13, 30, num0);
    
    //MMU_EnableICache();这一点至关重要， 不要关闭mmu，否则中断不能正常使用
	
    Touch_Init();           //开触摸屏中断
    
    //while(1)
    Delay(100);Uart_Printf("\n\nq:退出\n");
    while(Uart_GetKey() != 'q'){
    get_key();      //获取键值
    show_num();     //处理键值
    }
    
    Lcd_ClearScr( (0x1f<<11) | (0x3f<<5) | (0x1f)  );		//白色
//**********************还原现场************************** 
    rADCCON  = save_A;
    rGPGCON  = save_G;
    rGPGUP   = save_GP ;  
    rGPCCON  = save_C ;  
    rGPCUP   = save_CP;
    rGPDCON  = save_D;
    rGPDUP   = save_DP;
    rLCDCON1 = save_LCD1;
    rLCDCON2 = save_LCD2;
    rLCDCON3 = save_LCD3;
    rLCDCON4 = save_LCD4;
    rLCDCON5 = save_LCD5;
    rLCDINTMSK = save_LCDINT;
    rINTSUBMSK = save_INTS;
    rINTMSK    = save_INT;
//********************************************************
}
