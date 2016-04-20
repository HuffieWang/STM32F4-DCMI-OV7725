/*********************** HNIT 3103 Application Team **************************
 * �� �� �� ��hnit_sccb.c
 * ��    �� ��SCCBЭ��ʵ��       
 * ʵ��ƽ̨ ��STM32F407������
 * �� �� �� ��ST1.4.0
 * ʱ    �� ��2016.04.08
 * ��    �� ��3103�����Ŷ� ������
 * �޸ļ�¼ ����
******************************************************************************/
#include "hnit_sccb.h"

#define DEV_ADR  ADDR_OV7725 			 /*�豸��ַ����*/

 /**
  * @brief  SCCB�ܽ�����
  * @param  ��
  * @retval ��
  */
void sccb_gpio_config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 
    
    /* SCL(PD6)��SDA(PD7)�ܽ����� */     
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  //ʹ��GPIOCʱ��
    
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //��ͨ���ģʽ
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;        //��©���
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          //����
    
    GPIO_Init(GPIOD, &GPIO_InitStructure);                //��ʼ��
}

/**
  * @brief  SCCBЭ��ר����ʱ
  * @param  ��
  * @retval ��
  */
static void sccb_delay(void)
{	
    uint16_t i = 800; 
    while(i) 
    { 
        i--; 
    } 
}

/**
  * @brief  SCCB��ʼ�ź�
  * @param  ��
  * @retval ��
  */
static int sccb_start(void)
{
    SDA_H;
    SCL_H;
    sccb_delay();
    if(!SDA_read)
        return DISABLE;	/* SDA��Ϊ�͵�ƽ������æ,�˳� */
    SDA_L;
    sccb_delay();
    if(SDA_read) 
        return DISABLE;	/* SDA��Ϊ�ߵ�ƽ�����߳���,�˳� */
    SDA_L;
    sccb_delay();
    
    return ENABLE;
}

/**
  * @brief  SCCBֹͣ�ź�
  * @param  ��
  * @retval ��
  */
static void sccb_stop(void)
{
    SCL_L;
    sccb_delay();
    SDA_L;
    sccb_delay();
    SCL_H;
    sccb_delay();
    SDA_H;
    sccb_delay();
}

/**
  * @brief  SCCBӦ��
  * @param  ��
  * @retval ��
  */
static void sccb_ack(void)
{	
    SCL_L;
    sccb_delay();
    SDA_L;
    sccb_delay();
    SCL_H;
    sccb_delay();
    SCL_L;
    sccb_delay();
}

/**
  * @brief  SCCB ��Ӧ��
  * @param  ��
  * @retval ��
  */
static void sccb_no_ack(void)
{	
    SCL_L;
    sccb_delay();
    SDA_H;
    sccb_delay();
    SCL_H;
    sccb_delay();
    SCL_L;
    sccb_delay();
}

/**
  * @brief  SCCB �ȴ�Ӧ��
  * @param  ��
  * @retval 1����ACK 
  *         0����ACK
  */
static int sccb_wait_ack(void) 	
{
    SCL_L;
    sccb_delay();
    SDA_H;			
    sccb_delay();
    SCL_H;
    sccb_delay();
    if(SDA_read)
    {
        SCL_L;
        return DISABLE;
    }
    SCL_L;
    return ENABLE;
}

/**
  * @brief  ���ݴӸ�λ����λ
  * @param  SendByte ���͵�����
  * @retval ��
  */
static void sccb_send_byte(uint8_t SendByte) 
{
    uint8_t i=8;
    while(i--)
    {
        SCL_L;
        sccb_delay();
      if(SendByte&0x80)
        SDA_H;  
      else 
        SDA_L;   
        SendByte<<=1;
        sccb_delay();
		SCL_H;
        sccb_delay();
    }
    SCL_L;
}

/**
  * @brief  ���ݴӸ�λ����λ
  * @param  ��
  * @retval SCCB���߷��ص�����
  */
static int sccb_receive_byte(void)  
{ 
    uint8_t i=8;
    uint8_t ReceiveByte=0;

    SDA_H;				
    while(i--)
    {
      ReceiveByte<<=1;      
      SCL_L;
      sccb_delay();
	  SCL_H;
      sccb_delay();	
      if(SDA_read)
      {
        ReceiveByte|=0x01;
      }
    }
    SCL_L;
    return ReceiveByte;
}

/**
  * @brief  дһ�ֽ�����
  * @param  WriteAddress ��д���ַ
  * @param  SendByte     ��д������
  * @retval ��
  */
int sccb_write_byte( uint16_t WriteAddress , uint8_t SendByte )
{  
    if(!sccb_start())
    {
        return DISABLE;
    }
    sccb_send_byte( DEV_ADR );                    /* ������ַ */            
    if( !sccb_wait_ack() )
    {          
        sccb_stop(); 
        return DISABLE;
    }
    
    sccb_send_byte((uint8_t)(WriteAddress & 0x00FF));   /* ���õ���ʼ��ַ */      
    sccb_wait_ack();	
    sccb_send_byte(SendByte);
    sccb_wait_ack();   
    sccb_stop(); 
    return ENABLE;
}

/**
  * @brief  ��ȡһ������
  * @param  pBuffer     ��Ŷ�������
  * @param  ReadAddress ��������ַ
  * @retval 1���ɹ�����
  *         2��ʧ��
  */
int sccb_read_byte(uint8_t* pBuffer, uint16_t length, uint8_t ReadAddress)
{	
    if(!sccb_start())
    {
        return DISABLE;
    }
    sccb_send_byte( DEV_ADR );         /* ������ַ */
    if( !sccb_wait_ack() )
    {
        sccb_stop(); 
        return DISABLE;
    }
    sccb_send_byte( ReadAddress );     /* ���õ���ʼ��ַ */      
    sccb_wait_ack();	
    sccb_stop(); 
	
    if(!sccb_start())
    {
        return DISABLE;
    }
    sccb_send_byte( DEV_ADR + 1 );     /* ������ַ */ 
    if(!sccb_wait_ack())
    {
        sccb_stop(); 
        return DISABLE;
    }
    while(length)
    {
        *pBuffer = sccb_receive_byte();
        if(length == 1)
        {
            sccb_no_ack();
        }
        else
        {
            sccb_ack(); 
        }
        pBuffer++;
        length--;
    }
    sccb_stop();
    return ENABLE;
}
/*********************************************END OF FILE**********************/
