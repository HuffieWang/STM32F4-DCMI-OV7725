/*********************** HNIT 3103 Application Team **************************
 * �� �� �� ��hnit_lcd.h
 * ��    �� ��SRAM  IS62WV51216BLL(1Mb)��ʼ��  
 * ʵ��ƽ̨ ��STM32F407������
 * �� �� �� ��ST1.4.0
 * ʱ    �� ��2016.3.24
 * ��    �� ������ԭ�� www.opendv.com
 * �޸ļ�¼ ����
******************************************************************************/
#include "hnit_sram.h"	  
#include "stm32_delay.h"     

#define Bank1_SRAM3_ADDR    ((u32)(0x68000000))	
						  
/**
  * @brief  ��ʼ���ⲿSRAM 
  * @param  ��
  * @retval ��
  * @attention demo u32 testSram[250000] __attribute__((at(0X68000000)));
  *            ��ʼ����Ҫ��ʱһ��ʱ�䣬���ܽ��в��������ں���β����100ms��ʱ
  */
void fsmc_sram_init(void)
{	
    GPIO_InitTypeDef  GPIO_InitStructure;
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  readWriteTiming; 
	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|
                           RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);//ʹ��PD,PE,PF,PGʱ��  
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC,ENABLE);//ʹ��FSMCʱ��  
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = (3<<0)|(3<<4)|(0XFF<<8);//PD0,1,4,5,8~15 AF OUT
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��  
	
    GPIO_InitStructure.GPIO_Pin = (3<<0)|(0X1FF<<7);//PE0,1,7~15,AF OUT
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��  
	
    GPIO_InitStructure.GPIO_Pin = (0X3F<<0)|(0XF<<12); 	//PF0~5,12~15
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOF, &GPIO_InitStructure);//��ʼ��  

    GPIO_InitStructure.GPIO_Pin =(0X3F<<0)| GPIO_Pin_10;//PG0~5,10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;//�������
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//����
    GPIO_Init(GPIOG, &GPIO_InitStructure);//��ʼ�� 
 
 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);//PD0,AF12
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);//PD1,AF12
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC); 
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource11,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);//PD15,AF12
    
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource0,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource1,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);//PE7,AF12
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);//PE15,AF12
 
    GPIO_PinAFConfig(GPIOF,GPIO_PinSource0,GPIO_AF_FSMC);//PF0,AF12
    GPIO_PinAFConfig(GPIOF,GPIO_PinSource1,GPIO_AF_FSMC);//PF1,AF12
    GPIO_PinAFConfig(GPIOF,GPIO_PinSource2,GPIO_AF_FSMC);//PF2,AF12
    GPIO_PinAFConfig(GPIOF,GPIO_PinSource3,GPIO_AF_FSMC);//PF3,AF12
    GPIO_PinAFConfig(GPIOF,GPIO_PinSource4,GPIO_AF_FSMC);//PF4,AF12
    GPIO_PinAFConfig(GPIOF,GPIO_PinSource5,GPIO_AF_FSMC);//PF5,AF12
    GPIO_PinAFConfig(GPIOF,GPIO_PinSource12,GPIO_AF_FSMC);//PF12,AF12
    GPIO_PinAFConfig(GPIOF,GPIO_PinSource13,GPIO_AF_FSMC);//PF13,AF12
    GPIO_PinAFConfig(GPIOF,GPIO_PinSource14,GPIO_AF_FSMC);//PF14,AF12
    GPIO_PinAFConfig(GPIOF,GPIO_PinSource15,GPIO_AF_FSMC);//PF15,AF12
	
    GPIO_PinAFConfig(GPIOG,GPIO_PinSource0,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOG,GPIO_PinSource1,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOG,GPIO_PinSource2,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOG,GPIO_PinSource3,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOG,GPIO_PinSource4,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOG,GPIO_PinSource5,GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOG,GPIO_PinSource10,GPIO_AF_FSMC);
	
 	  
    readWriteTiming.FSMC_AddressSetupTime = 0x00;	 //��ַ����ʱ�䣨ADDSET��Ϊ1��HCLK 1/36M=27ns
    readWriteTiming.FSMC_AddressHoldTime = 0x00;	 //��ַ����ʱ�䣨ADDHLD��ģʽAδ�õ�	
    readWriteTiming.FSMC_DataSetupTime = 0x08;		 ////���ݱ���ʱ�䣨DATAST��Ϊ9��HCLK 6*9=54ns	 	 
    readWriteTiming.FSMC_BusTurnAroundDuration = 0x00;
    readWriteTiming.FSMC_CLKDivision = 0x00;
    readWriteTiming.FSMC_DataLatency = 0x00;
    readWriteTiming.FSMC_AccessMode = FSMC_AccessMode_A;	 //ģʽA 
    
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;//  ��������ʹ��NE3 ��Ҳ�Ͷ�ӦBTCR[4],[5]��
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_MemoryType =FSMC_MemoryType_SRAM;// FSMC_MemoryType_SRAM;  //SRAM   
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//�洢�����ݿ��Ϊ16bit  
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode =FSMC_BurstAccessMode_Disable;// FSMC_BurstAccessMode_Disable; 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait=FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;   
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;  
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;	//�洢��дʹ�� 
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;  
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable; // ��дʹ����ͬ��ʱ��
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;  
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &readWriteTiming;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &readWriteTiming; //��дͬ��ʱ��

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);  //��ʼ��FSMC����

    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);  // ʹ��BANK3			

    delay_ms(100);
}
	  														  
/**
  * @brief  ��ָ����ַ(WriteAddr+Bank1_SRAM3_ADDR)��ʼ,����д��n���ֽ�.
  * @param  pBuffer   �ֽ�ָ��
  * @param  WriteAddr Ҫд��ĵ�ַ
  * @param  n         Ҫд����ֽ���
  * @retval ��
  */
void sram_write_buffer(u8* pBuffer,u32 WriteAddr,u32 n)
{
	for(;n!=0;n--)  
	{										    
		*(vu8*)(Bank1_SRAM3_ADDR+WriteAddr)=*pBuffer;	  
		WriteAddr++;
		pBuffer++;
	}   
}																			    

/**
  * @brief  ��ָ����ַ((WriteAddr+Bank1_SRAM3_ADDR))��ʼ,��������n���ֽ�.
  * @param  pBuffer   �ֽ�ָ��
  * @param  WriteAddr Ҫ�����ĵ�ַ
  * @param  n         Ҫ�������ֽ���
  * @retval ��
  */
void sram_read_buffer(u8* pBuffer,u32 ReadAddr,u32 n)
{
	for(;n!=0;n--)  
	{											    
		*pBuffer++=*(vu8*)(Bank1_SRAM3_ADDR+ReadAddr);    
		ReadAddr++;
	}  
} 











