/*********************** HNIT 3103 Application Team **************************
 * 文 件 名 ：hnit_sccb.c
 * 描    述 ：SCCB协议实现       
 * 实验平台 ：STM32F407开发板
 * 库 版 本 ：ST1.4.0
 * 时    间 ：2016.04.08
 * 作    者 ：3103创新团队 王昱霏
 * 修改记录 ：无
******************************************************************************/
#include "hnit_sccb.h"

#define DEV_ADR  ADDR_OV7725 			 /*设备地址定义*/

 /**
  * @brief  SCCB管脚配置
  * @param  无
  * @retval 无
  */
void sccb_gpio_config(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure; 
    
    /* SCL(PD6)、SDA(PD7)管脚配置 */     
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);  //使能GPIOC时钟
    
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;         //普通输出模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;        //开漏输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;     //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          //上拉
    
    GPIO_Init(GPIOD, &GPIO_InitStructure);                //初始化
}

/**
  * @brief  SCCB协议专用延时
  * @param  无
  * @retval 无
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
  * @brief  SCCB起始信号
  * @param  无
  * @retval 无
  */
static int sccb_start(void)
{
    SDA_H;
    SCL_H;
    sccb_delay();
    if(!SDA_read)
        return DISABLE;	/* SDA线为低电平则总线忙,退出 */
    SDA_L;
    sccb_delay();
    if(SDA_read) 
        return DISABLE;	/* SDA线为高电平则总线出错,退出 */
    SDA_L;
    sccb_delay();
    
    return ENABLE;
}

/**
  * @brief  SCCB停止信号
  * @param  无
  * @retval 无
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
  * @brief  SCCB应答
  * @param  无
  * @retval 无
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
  * @brief  SCCB 无应答
  * @param  无
  * @retval 无
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
  * @brief  SCCB 等待应答
  * @param  无
  * @retval 1：有ACK 
  *         0：无ACK
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
  * @brief  数据从高位到低位
  * @param  SendByte 发送的数据
  * @retval 无
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
  * @brief  数据从高位到低位
  * @param  无
  * @retval SCCB总线返回的数据
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
  * @brief  写一字节数据
  * @param  WriteAddress 待写入地址
  * @param  SendByte     待写入数据
  * @retval 无
  */
int sccb_write_byte( uint16_t WriteAddress , uint8_t SendByte )
{  
    if(!sccb_start())
    {
        return DISABLE;
    }
    sccb_send_byte( DEV_ADR );                    /* 器件地址 */            
    if( !sccb_wait_ack() )
    {          
        sccb_stop(); 
        return DISABLE;
    }
    
    sccb_send_byte((uint8_t)(WriteAddress & 0x00FF));   /* 设置低起始地址 */      
    sccb_wait_ack();	
    sccb_send_byte(SendByte);
    sccb_wait_ack();   
    sccb_stop(); 
    return ENABLE;
}

/**
  * @brief  读取一串数据
  * @param  pBuffer     存放读出数据
  * @param  ReadAddress 待读出地址
  * @retval 1：成功读入
  *         2：失败
  */
int sccb_read_byte(uint8_t* pBuffer, uint16_t length, uint8_t ReadAddress)
{	
    if(!sccb_start())
    {
        return DISABLE;
    }
    sccb_send_byte( DEV_ADR );         /* 器件地址 */
    if( !sccb_wait_ack() )
    {
        sccb_stop(); 
        return DISABLE;
    }
    sccb_send_byte( ReadAddress );     /* 设置低起始地址 */      
    sccb_wait_ack();	
    sccb_stop(); 
	
    if(!sccb_start())
    {
        return DISABLE;
    }
    sccb_send_byte( DEV_ADR + 1 );     /* 器件地址 */ 
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
