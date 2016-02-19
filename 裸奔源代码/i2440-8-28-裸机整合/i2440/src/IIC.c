#include "def.h"  
#include "2440addr.h"  
#include "2440lib.h" 


static unsigned int i,j;  
static U8 data[256];  
static volatile int flag;   //用于标识是否收到应答信号，改标识在终端处理程序中被清0 

/**************************************************************
IIC 初始化函数
**************************************************************/
void iic_init(void){
    rGPECON |= 0xa0000000;                 //GPE15:IICSDA , GPE14:IICSCL (rGPEUP[14:15]没有上拉选项)
   
    //[7]=1允许应答发生  [6]=0 IICCLK = fPCLK /16  PCLK 50MHz, IICCLK = 3.17MHz, Tx Clock = 0.198MHz
    // 允许IIC总线Tx/Rx中断使能   IICCON[3:0]=16       
    rIICCON = (1<<7) | (0<<6) | (1<<5) | (0xf); 
    
    rIICADD  = 0x10;     //从地址    表示2440作为从设备的时候的地址，在这里2440是作为一个主设备存在的，所以没有作用。   
    rIICSTAT = 0x10;         //IIC总线数据输出使能读写  
    rIICLC = (1<<2)|(1);     //使能滤波器 延时五个时钟  

}

/**************************************************************
 IIC中断函数
**************************************************************/
void __irq Iic_isr(void)  
{  
    flag = 0; 
    rSRCPND = 0x1<<27;          //清除中断  
    rINTPND = 0x1<<27;    
}

void iic_isr(void){
    rINTMSK &= ~(0x1<<27);
    pISR_IIC = (unsigned)Iic_isr;//中断入口  
}

/**************************************************************
    写EEPROM中的数据
**************************************************************/  
void Wr24C080(U32 slvAddr, U32 addr, U8 data)  
{  
    flag=1;            //以下都是应答标志 标志位置1 
    rIICDS = slvAddr;  //**设备地址（EEPROM的地址为1010） 
    rIICSTAT = 0xf0;   // 主设备发送模式，写）产生起始信号  
    rIICCON &= ~0x10;  //以下都是清中断标志  
    while(flag == 1)   //当发送从地址完成之后会收到ACK信号，在中断处理函数中将该标志置为0  
    Delay(1);  
       
      
    flag =1 ;          
    rIICDS = addr;     //偏移地址
    rIICCON &= ~0x10;             
    while(flag == 1)   //当发送从地址完成之后会收到ACK信号，在中断处理函数中将该标志置为0  
    Delay(1);  
       
    flag =1 ;          
    rIICDS = data;     //要写入的具体数据
    rIICCON &= ~0x10;           
    while(flag == 1)   //当发送从地址完成之后会收到ACK信号，在中断处理函数中将该标志置为0  
    Delay(1);  
       
    rIICSTAT = 0xd0;                    //Stop MasTx condition   
    rIICCON  = 0xaf;                    //Resumes IIC operation.   
    Delay(1);  
      
} 
 
/**************************************************************
    读EEPROM中的数据
**************************************************************/  
void Rd24C080(U32 slvAddr, U32 addr, U8 *data)  
{  
    unsigned char temp;  
    flag=1;                            
    rIICDS = slvAddr;   //设备地址（EEPROM的地址为1010）
    rIICSTAT = 0xf0;    // 主设备发送模式用来发送slvAddr和addr,，启动  
    rIICCON &= ~0x10;               
    while(flag == 1)    // 当发送从地址完成之后会收到ACK信号，在中断处理函数中将该标志置为0  
    Delay(1);  
       
    flag =1 ;                           
    rIICDS = addr;       //偏移地址
    rIICCON &= ~0x10;                
    while(flag == 1)    // 当发送从地址完成之后会收到ACK信号，在中断处理函数中将该标志置为0  
    Delay(1);  
          
    flag=1;                         
    rIICDS = slvAddr;   //从设备地址。这是特定要求这样发送的。。（在这里主设备会被配置为接收状态），
                        //这此发送设备地址是用来同时调整主设备状态的。
    rIICSTAT = 0xb0;    // 主设备接收模式用来接收数据，启动  
    rIICCON &= ~0x10;             
    while(flag == 1)    // 当发送从地址完成之后会收到ACK信号，在中断处理函数中将该标志置为0  
     Delay(1);  
      
    // 注意：读取下面这个字节必须进行，因为在发送带有读命令的从设备地址后，  
    // AT24C02A会再返回一个从设备地址信息或从设备内存地址信息作为应答，所以一定要把该字节读取后抛弃，因为它不是我们所要读取的信息；  
    flag =1 ;                          // 标志位置1  
    temp = rIICDS;      // 抛弃第一自己  
    rIICCON &= ~0x10;   // 清中断标志  
    while(flag)  
     Delay(1);  
       
    if(i==256-1)  rIICCON &= ~0x80;//如果是最后一个数据 不再响应
    else          rIICCON = 0x2f;     //Resumes IIC operation with NOACK.   
    *data = rIICDS;   
    Delay(1);  
    rIICSTAT = 0x90;   //Stop MasTx condition   
    rIICCON  = 0xaf;   //Resumes IIC operation.  
          
    Delay(1);  
      
}  
  
/**************************************************************
 IIC 子main函数
**************************************************************/
void IIC(void)  
{  
    static U32 save_E, save_PE, save_C, save_CA, save_CS, save_CLC, save_INT, save_CDS, save_CTA; 
//**********************保护现场************************** 
    save_E   = rGPECON;  
    save_PE  = rGPEUP;
    save_C   = rIICCON;
    save_CA  = rIICADD;
    save_CS  = rIICSTAT;
    save_CLC = rIICLC;
    save_INT = rINTMSK;
    save_CDS = rIICDS;
    save_CTA = rIICSTAT;   
//******************************************************** 
          
    Uart_Printf("\n  ***  IIC TEST  ***  \n"); // Delay(1000); //1
      
    iic_init(); //IIC初始化
    iic_isr();  //IIC中断
   
    Uart_Printf("写入数据到EEPROM\n"); //向EEPROM写数据i  
    for(i=0;i<256;i++){ Wr24C080(0xa0,(U8)i,i); Delay(1);}   //注意这个延时不能少，否则出现有些数据无法写入的问题           
    
    for(i=0;i<256;i++) data[0] = 0; //初始化data[i]  
   
    Uart_Printf("读EEPROM中的数据\n"); //EEPROM读取数据到data[i]    
    for(i=0;i<256;i++){ Rd24C080(0xa0,(U8)i,&(data[i])); Delay(1);}
    
    for(i=0;i<16;i++)  //打印data[i]数组
    {    Uart_Printf("\n");
         for(j=0;j<16;j++)   Uart_Printf(" %2x",data[i*16+j]);  } 
      
    
    Delay(100);Uart_Printf("\n\nq:退出\n");
    while(Uart_GetKey() != 'q'){
    } 

//**********************保护现场************************** 
    rGPECON  = save_E;  
    rGPEUP   = save_PE;
    rIICCON  = save_C;
    rIICADD  = save_CA;
    rIICSTAT = save_CS;
    rIICLC   = save_CLC;
    rINTMSK  = save_INT;
    rIICDS   = save_CDS;
    rIICSTAT = save_CTA;    
//********************************************************
}  


