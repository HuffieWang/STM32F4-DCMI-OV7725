#include "stm32_sys.h"
#include "hnit_dcmi.h" 
#include "hnit_led.h" 
#include "hnit_lcd.h"
u8 ov_frame=0;  						//֡��
static uint16_t line_num =0;


//DCMI�жϷ�����
void DCMI_IRQHandler(void)
{
	if(DCMI_GetITStatus(DCMI_IT_FRAME)==SET)//����һ֡ͼ��
	{
        LCD_SetCursor(0, 0);  // ���ù��λ��                   
        LCD_WriteRAM_Prepare();   // ��ʼд��GRAM     
		DCMI_ClearITPendingBit(DCMI_IT_FRAME);//���֡�ж�
		ov_frame++; 
      //  dcmi_stop();
	}
} 


////ʹ��֡�ж�����line_num,�ɷ�ֹ��ʱ�����ݵ�ʱ��DMA������������ƫ��
//void DCMI_IRQHandler(void)
//{

//	if(  DCMI_GetITStatus (DCMI_IT_FRAME) == SET )    
//	{
//		/*������һ֡*/
//		line_num=0;
//		fps++; //֡�ʼ���
//		
//		DCMI_ClearITPendingBit(DCMI_IT_FRAME); 
//	}
//}

void DMA2_Stream1_IRQHandler(void)
{
    if(DMA_GetITStatus(DMA2_Stream1,DMA_IT_TCIF1) == SET)    
    {
        LED1 = 0;
//		line_num++;

//    if(line_num==out_height)
//		{
//			/*������һ֡*/
//			line_num=0;
//		}
//		
//		/*DMA һ��һ�д���*/
//    OV2640_DMA_Config(FSMC_LCD_ADDRESS+(out_width*2*(out_height-line_num-1)),out_width/2);
        DMA_ClearITPendingBit(DMA2_Stream1,DMA_IT_TCIF1);
	}
}


//DCMI DMA����
//DMA_Memory0BaseAddr:�洢����ַ    ��Ҫ�洢����ͷ���ݵ��ڴ��ַ(Ҳ�����������ַ)
//DMA_BufferSize:�洢������    0~65535
//DMA_MemoryDataSize:�洢��λ��    @DMA_MemoryDataSize_Byte/_HalfWord/_Word
//DMA_MemoryInc:�洢��������ʽ  @DMA_MemoryInc_Enable/_Disable
void dcmi_dma_init(u32 DMA_Memory0BaseAddr,u16 DMA_BufferSize,u32 DMA_MemoryDataSize,u32 DMA_MemoryInc)
{ 
    DMA_InitTypeDef  DMA_InitStructure;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ�� 
    DMA_DeInit(DMA2_Stream1);
    while (DMA_GetCmdStatus(DMA2_Stream1) != DISABLE){}//�ȴ�DMA2_Stream1������ 
	
    /* ���� DMA Stream */
    DMA_InitStructure.DMA_Channel = DMA_Channel_1; 
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&DCMI->DR;    //�����ַ
    DMA_InitStructure.DMA_Memory0BaseAddr = DMA_Memory0BaseAddr;  //�ڴ��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;       //���赽�ڴ�
    DMA_InitStructure.DMA_BufferSize = DMA_BufferSize; //���ݴ����� 
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//����no inc
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc;//�ڴ� inc
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;//�������ݳ���:32λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize;//�洢�����ݳ��� 
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// ʹ��ѭ��ģʽ 
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//�����ȼ�
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable; //FIFOģʽ        
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;//ʹ��ȫFIFO 
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//����ͻ�����δ���
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//�洢��ͻ�����δ���
    DMA_Init(DMA2_Stream1, &DMA_InitStructure);//��ʼ��DMA Stream	
    //DMA_Cmd(DMA2_Stream1,ENABLE);
   // while(DMA_GetCmdStatus(DMA2_Stream1) != ENABLE){}
        
}

//DCMI��ʼ��
void dcmi_config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    DCMI_InitTypeDef DCMI_InitStructure;
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | 
                           RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI,ENABLE);
    //PA4/6��ʼ������
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_6;//PA4/6   ���ù������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF; //���ù������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
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

    DCMI_DeInit();//���ԭ�������� 
 
    DCMI_InitStructure.DCMI_CaptureMode=DCMI_CaptureMode_Continuous;//����ģʽ
    DCMI_InitStructure.DCMI_CaptureRate=DCMI_CaptureRate_All_Frame;//ȫ֡����
    DCMI_InitStructure.DCMI_ExtendedDataMode= DCMI_ExtendedDataMode_8b;//8λ���ݸ�ʽ  
    DCMI_InitStructure.DCMI_HSPolarity= DCMI_HSPolarity_Low;//HSYNC �͵�ƽ��Ч
    DCMI_InitStructure.DCMI_PCKPolarity= DCMI_PCKPolarity_Rising;//PCLK ��������Ч
    DCMI_InitStructure.DCMI_SynchroMode= DCMI_SynchroMode_Hardware;//Ӳ��ͬ��HSYNC,VSYNC
    DCMI_InitStructure.DCMI_VSPolarity=DCMI_VSPolarity_High;//VSYNC �͵�ƽ��Ч
    DCMI_Init(&DCMI_InitStructure);
	
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    DMA_ITConfig(DMA2_Stream1,DMA_IT_TC,ENABLE); 

    NVIC_InitStructure.NVIC_IRQChannel = DCMI_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
    NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    DCMI_ITConfig(DCMI_IT_FRAME,ENABLE);//����֡�ж� 
} 

//DCMI,��������
void dcmi_start(void)
{  
    DCMI_Cmd(ENABLE);	//DCMIʹ��
    DMA_Cmd(DMA2_Stream1, ENABLE);//����DMA2,Stream1 
    DCMI_CaptureCmd(ENABLE);//DCMI����ʹ��  
}

//DCMI,�رմ���
void dcmi_stop(void)
{ 
    DCMI_CaptureCmd(DISABLE);//DCMI����ʹ�ر�
    while(DCMI->CR&0X01);		//�ȴ��������  	
    DMA_Cmd(DMA2_Stream1,DISABLE);//�ر�DMA2,Stream1
} 






