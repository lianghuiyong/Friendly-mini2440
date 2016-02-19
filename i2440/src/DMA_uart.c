//====================================================================
//  实现功能：
//           DMA直接存取  实现Uart（串口）通信,
//       将字符串数据通过DMA0通道传递给UTXH0，然后在终端
//       显示。数据传输完后，DMA0产生中断，beep声, LED亮。
//                                                   by：梁惠涌
//====================================================================

#include "2440addr.h"
#include "2440lib.h"  //beep函数

char  *SendBuffer = "Hello mini2440!" ;    //source data

/**************************************************************
                          DMA初始化
**************************************************************/
void Dma_init()
{
    rDISRC0=(U32)SendBuffer;                    //数据地址	
    rDISRCC0 |=((0<<1)|(0<<0));                 //[1]系统总线,[0]地址将根据单次和突发模式中每次传输后其数据大小而增加
    rDIDST0=(U32)UTXH0;                         //传输目标地址UTXH0 {2440addr.h文件中定义 #define UTXH0       (0x50000020) }
    rDIDSTC0 |=((0<<2)|(1<<1)|(1<<0));          //[2]在TC到达0时发生中断, [1] APB(外设总线上)  , [0]传输后地址总线不变
    //对上述寄存器说明，rDISRC0、rDIDST0分别配置为数据地址(DMA内部)、传输目标地址(uart是属于外设设备)，再相应的配置rDISRCC0、rDIDSTC0
  
    rDCON0 |=((U32)1<<31)|(0<<30)|(1<<29)|(0<<28)|(0<<27)|(1<<24)|(1<<23)|(1<<22)|(0<<20)|(15);   
   //[31] 握手模式; [30]同步PCLK=50M(APB时钟< 外设 >); [29]当所有传输完成产生中断请求（即CURR_TC变为0）;[28]执行一个单元传输;
   //[27]选择每次原子传输（单次或突发4）后DMA停止和等待其它DMA请求的单服务模式; [24]UART0; [23]选择DMA源触发DMA操作;
   //[22]当传输计数的当前值变为0时DMA通道（DMA  REQ）关闭; [20]字节(每次一个字节传输);[19:0]初始传输计数15，15=SendBuffer字符数组长度;
   
    rDMASKTRIG0=(0<<2)|(1<<1)|(0<<0);  //[1]打开DMA通道并且处理此通道DMA请求
}

/**************************************************************
                          DMA中断
**************************************************************/
void __irq Dma0_isr(){
    rSRCPND|=0x1<<17;   //清除中断挂起状态
    rINTPND|=0x1<<17;
    Uart_Printf("\n       ***DMA TO Uart finished***\n");
    Beep(2000,100);
    rGPBCON=0x015400;
    rGPBDAT=0x6<<5;
}

/**************************************************************
|                          DMA子函数
|  需设置2440lib.c里   rUCON0  |=((1<<0) | (1<<3) | (2<<10) ); 
**************************************************************/
void DMA_UART(){
    static U32 save_DI, save_DIS, save_DID, save_DTC,save_DC, save_B, save_BD, save_INT, save_DMA;
     
//**********************保护现场************************** 
    save_DI   = rDISRC0;
    save_DIS  = rDISRCC0;
    save_DID  = rDIDST0;  
    save_DTC  = rDIDSTC0;  
    save_DC   = rDCON0;
    save_DMA  = rDMASKTRIG0;
    save_B    = rGPBCON;
    save_BD   = rGPBDAT;
    save_INT  = rINTMSK;
//********************************************************
    
    Uart_Printf("\n       ***HELLO DMA TO Uart ***\n");
    Delay(1000);
    
    Dma_init();//DMA初始化   	
    rINTMSK &=~(1<<17);  //开 dma0 中断
    pISR_DMA0=(U32)Dma0_isr;
    
   
    //while(1)
    Delay(100);Uart_Printf("\n\nq:退出\n");
    while(Uart_GetKey() != 'q'){
    }  
    
//*********************还原现场************************** 
    rDISRC0   = save_DI;
   rDISRCC0   = save_DIS;
   rDIDST0    = save_DID;  
   rDIDSTC0   = save_DTC;  
   rDCON0     = save_DC;
   rDMASKTRIG0   = save_DMA;
   rGPBCON    = save_B;
   rGPBDAT    = save_BD;
   rINTMSK    = save_INT;
//******************************************************** 
}