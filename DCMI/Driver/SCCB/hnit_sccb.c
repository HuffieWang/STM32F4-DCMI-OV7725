/*********************** HNIT 3103 Application Team **************************
 * �� �� �� ��hnit_sccb.c
 * ��    �� ��SCCBЭ��ʵ��       
 * ʵ��ƽ̨ ��STM32F407������
 * �� �� �� ��ST1.4.0
 * ʱ    �� ��2016.04.08
 * ��    �� ��3103�����Ŷ�
 * �޸ļ�¼ ����
******************************************************************************/

#include "hnit_sccb.h"
#include "hnit_lcd.h"

#define DEV_ADR  ADDR_OV7725 			 /*�豸��ַ����*/

/********************************************************************
 * ��������SCCB_Configuration
 * ����  ��SCCB�ܽ�����
 * ����  ����
 * ���  ����
 * ע��  ����        
 ********************************************************************/
void SCCB_GPIO_Config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 
    /* SCL(PC1)��SDA(PC2)�ܽ����� */
       
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);  //ʹ��GPIOCʱ��
    
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;        //��©���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          //����
    
    GPIO_Init(GPIOC, &GPIO_InitStructure);                //��ʼ��
}

/********************************************************************
 * ��������SCCB_delay
 * ����  ���ӳ�ʱ��
 * ����  ����
 * ���  ����
 * ע��  ���ڲ�����        
 ********************************************************************/
static void SCCB_delay(void)
{	
   uint16_t i = 800; 
   while(i) 
   { 
     i--; 
   } 
}

/********************************************************************
 * ��������SCCB_Start
 * ����  ��SCCB��ʼ�ź�
 * ����  ����
 * ���  ����
 * ע��  ���ڲ�����        
 ********************************************************************/
static int SCCB_Start(void)
{
	SDA_H;
	SCL_H;
	SCCB_delay();
	if(!SDA_read)
	return DISABLE;	/* SDA��Ϊ�͵�ƽ������æ,�˳� */
	SDA_L;
	SCCB_delay();
	if(SDA_read) 
	return DISABLE;	/* SDA��Ϊ�ߵ�ƽ�����߳���,�˳� */
	SDA_L;
	SCCB_delay();
    
	return ENABLE;
}

/********************************************************************
 * ��������SCCB_Stop
 * ����  ��SCCBֹͣ�ź�
 * ����  ����
 * ���  ����
 * ע��  ���ڲ�����        
 ********************************************************************/
static void SCCB_Stop(void)
{
	SCL_L;
	SCCB_delay();
	SDA_L;
	SCCB_delay();
	SCL_H;
	SCCB_delay();
	SDA_H;
	SCCB_delay();
}

/********************************************************************
 * ��������SCCB_Ack
 * ����  ��SCCBӦ��ʽ
 * ����  ����
 * ���  ����
 * ע��  ���ڲ�����        
 ********************************************************************/
static void SCCB_Ack(void)
{	
	SCL_L;
	SCCB_delay();
	SDA_L;
	SCCB_delay();
	SCL_H;
	SCCB_delay();
	SCL_L;
	SCCB_delay();
}

/********************************************************************
 * ��������SCCB_NoAck
 * ����  ��SCCB ��Ӧ��ʽ
 * ����  ����
 * ���  ����
 * ע��  ���ڲ�����        
 ********************************************************************/
static void SCCB_NoAck(void)
{	
	SCL_L;
	SCCB_delay();
	SDA_H;
	SCCB_delay();
	SCL_H;
	SCCB_delay();
	SCL_L;
	SCCB_delay();
}

/********************************************************************
 * ��������SCCB_WaitAck
 * ����  ��SCCB �ȴ�Ӧ��
 * ����  ����
 * ���  ������Ϊ:=1��ACK,=0��ACK
 * ע��  ���ڲ�����        
 ********************************************************************/
static int SCCB_WaitAck(void) 	
{
	SCL_L;
	SCCB_delay();
	SDA_H;			
	SCCB_delay();
	SCL_H;
	SCCB_delay();
	if(SDA_read)
	{
      SCL_L;
      return DISABLE;
	}
	SCL_L;
	return ENABLE;
}

 /*******************************************************************
 * ��������SCCB_SendByte
 * ����  �����ݴӸ�λ����λ
 * ����  ��SendByte: ���͵�����
 * ���  ����
 * ע��  ���ڲ�����        
 *********************************************************************/
static void SCCB_SendByte(uint8_t SendByte) 
{
    uint8_t i=8;
    while(i--)
    {
        SCL_L;
        SCCB_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        SCCB_delay();
		SCL_H;
        SCCB_delay();
    }
    SCL_L;
}

 /******************************************************************
 * ��������SCCB_ReceiveByte
 * ����  �����ݴӸ�λ����λ
 * ����  ����
 * ���  ��SCCB���߷��ص�����
 * ע��  ���ڲ�����        
 *******************************************************************/
static int SCCB_ReceiveByte(void)  
{ 
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      SCCB_delay();
	  SCL_H;
      SCCB_delay();	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
}

 /*****************************************************************************************
 * ��������SCCB_WriteByte
 * ����  ��дһ�ֽ�����
 * ����  ��- WriteAddress: ��д���ַ 	- SendByte: ��д������	- DeviceAddress: ��������
 * ���  ������Ϊ:=1�ɹ�д��,=0ʧ��
 * ע��  ����        
 *****************************************************************************************/           
int SCCB_WriteByte( uint16_t WriteAddress , uint8_t SendByte )
{  
    if(!SCCB_Start())
	{
	    return DISABLE;
	}
    SCCB_SendByte( DEV_ADR );                    /* ������ַ */            
    if( !SCCB_WaitAck() )
	{          
		SCCB_Stop(); 
		return DISABLE;
	}
    
    SCCB_SendByte((uint8_t)(WriteAddress & 0x00FF));   /* ���õ���ʼ��ַ */      
    SCCB_WaitAck();	
    SCCB_SendByte(SendByte);
    SCCB_WaitAck();   
    SCCB_Stop(); 
    return ENABLE;
}

/******************************************************************************************************************
 * ��������SCCB_ReadByte
 * ����  ����ȡһ������
 * ����  ��- pBuffer: ��Ŷ������� 	- length: ����������	- ReadAddress: ��������ַ		 - DeviceAddress: ��������
 * ���  ������Ϊ:=1�ɹ�����,=0ʧ��
 * ע��  ����        
 **********************************************************************************************************************/           
int SCCB_ReadByte(uint8_t* pBuffer, uint16_t length, uint8_t ReadAddress)
{	
    if(!SCCB_Start())
	{
	    return DISABLE;
	}
    SCCB_SendByte( DEV_ADR );         /* ������ַ */
    if( !SCCB_WaitAck() )
	{
		SCCB_Stop(); 
		return DISABLE;
	}
    SCCB_SendByte( ReadAddress );     /* ���õ���ʼ��ַ */      
    SCCB_WaitAck();	
    SCCB_Stop(); 
	
    if(!SCCB_Start())
	{
		return DISABLE;
	}
    SCCB_SendByte( DEV_ADR + 1 );     /* ������ַ */ 
    if(!SCCB_WaitAck())
	{
		SCCB_Stop(); 
		return DISABLE;
	}
    while(length)
    {
      *pBuffer = SCCB_ReceiveByte();
      if(length == 1)
	  {
		  SCCB_NoAck();
	  }
      else
	  {
		SCCB_Ack(); 
	  }
      pBuffer++;
      length--;
    }
    SCCB_Stop();
    return ENABLE;
}
/*********************************************END OF FILE**********************/
