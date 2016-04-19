/*********************** HNIT 3103 Application Team **************************
 * 文 件 名 ：hnit_dcmi.c
 * 描    述 ：DCMI配置    
 * 实验平台 ：STM32F407开发板
 * 库 版 本 ：ST1.4.0
 * 时    间 ：2016.04.08
 * 作    者 ：3103创新团队
 * 修改记录 ：无
******************************************************************************/
#include "hnit_dcmi.h" 
#include "stm32_sys.h"
#include "hnit_led.h" 
#include "hnit_lcd.h"

/**
  * @brief  DCMI初始化
  * @param  无
  * @retval 无
  */
void dcmi_config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    DCMI_InitTypeDef DCMI_InitStructure;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | 
                           RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI,ENABLE);
    //PA4/6初始化设置
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6;//PA4/6   复用功能输出
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //复用功能输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//上拉
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_6;
    GPIO_Init(GPIOB, &GPIO_InitStructure);	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_11;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOA,GPIO_PinSource4,GPIO_AF_DCMI); //PA4,AF13  DCMI_HSYNC
    GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_DCMI); //PA6,AF13  DCMI_PCLK  
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource7,GPIO_AF_DCMI); //PB7,AF13  DCMI_VSYNC 
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_DCMI); //PC6,AF13  DCMI_D0  
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_DCMI); //PC7,AF13  DCMI_D1 
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_DCMI); //PC8,AF13  DCMI_D2
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_DCMI); //PC9,AF13  DCMI_D3
    GPIO_PinAFConfig(GPIOC,GPIO_PinSource11,GPIO_AF_DCMI);//PC11,AF13 DCMI_D4 
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource6,GPIO_AF_DCMI); //PB6,AF13  DCMI_D5 
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_DCMI); //PE5,AF13  DCMI_D6
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource6,GPIO_AF_DCMI); //PE6,AF13  DCMI_D7

    DCMI_DeInit();//清除原来的设置 
 
    DCMI_InitStructure.DCMI_CaptureMode=DCMI_CaptureMode_Continuous;//连续模式
    DCMI_InitStructure.DCMI_CaptureRate=DCMI_CaptureRate_All_Frame;//全帧捕获
    DCMI_InitStructure.DCMI_ExtendedDataMode= DCMI_ExtendedDataMode_8b;//8位数据格式  
    DCMI_InitStructure.DCMI_HSPolarity= DCMI_HSPolarity_Low;//HSYNC 低电平有效
    DCMI_InitStructure.DCMI_PCKPolarity= DCMI_PCKPolarity_Rising;//PCLK 上升沿有效
    DCMI_InitStructure.DCMI_SynchroMode= DCMI_SynchroMode_Hardware;//硬件同步HSYNC,VSYNC
    DCMI_InitStructure.DCMI_VSPolarity=DCMI_VSPolarity_High;//VSYNC 低电平有效
    DCMI_Init(&DCMI_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化NVIC寄存器
    DCMI_ITConfig(DCMI_IT_FRAME,ENABLE);//开启帧中断     
} 

/**
  * @brief  DCMI的DMA初始化
  * @param  DMA_Memory0BaseAddr 将要存储摄像头数据的内存地址(或外设地址)
  * @param  DMA_BufferSize 存储器长度(0~65535)
  * @param  DMA_Memory0Inc 存储器地址是否自增
  * @retval 无
  */
void dcmi_dma_init(u32 DMA_Memory0BaseAddr, u16 DMA_BufferSize, u16 DMA_Memory0Inc)
{ 
    DMA_InitTypeDef  DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
      
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE); //DMA2时钟使能 
    DMA_DeInit(DMA2_Stream1);
    while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){} //等待DMA2_Stream1可配置 
	
    //配置 DMA Stream
    DMA_InitStructure.DMA_Channel = DMA_Channel_1; 
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DCMI->DR;    
    DMA_InitStructure.DMA_Memory0BaseAddr = DMA_Memory0BaseAddr;  //内存地址 
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;       
    DMA_InitStructure.DMA_BufferSize = DMA_BufferSize;            //数据传输量 
    DMA_InitStructure.DMA_PeripheralInc = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_Memory0Inc;      //内存自增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;            // 使用循环模式 
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable; 
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;           //储存器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;   //外设突发单次传输
    DMA_Init(DMA2_Stream1, &DMA_InitStructure);
                    
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    DMA_ITConfig(DMA2_Stream1,DMA_IT_TC,ENABLE); 
    DCMI_Cmd(ENABLE);	             //DCMI使能       
}

/**
  * @brief  DCMI启动传输
  * @param  无
  * @retval 无
  */
void dcmi_start(void)
{  
    DMA_Cmd(DMA2_Stream1, ENABLE);   //开启DMA2,Stream1 
    DCMI_CaptureCmd(ENABLE);         //DCMI捕获使能  
}

/**
  * @brief  DCMI停止传输
  * @param  无
  * @retval 无
  */
void dcmi_stop(void)
{ 
    DCMI_CaptureCmd(DISABLE);       //DCMI捕获使关闭
    while(DCMI->CR&0X01);		    //等待传输结束  	
    DMA_Cmd(DMA2_Stream1,DISABLE);  //关闭DMA2,Stream1
} 






