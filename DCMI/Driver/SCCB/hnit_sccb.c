/*********************** HNIT 3103 Application Team **************************
 * 文 件 名 ：hnit_sccb.c
 * 描    述 ：SCCB协议实现       
 * 实验平台 ：STM32F407开发板
 * 库 版 本 ：ST1.4.0
 * 时    间 ：2016.04.08
 * 作    者 ：3103创新团队
 * 修改记录 ：无
******************************************************************************/

#include "hnit_sccb.h"
#include "hnit_lcd.h"

#define DEV_ADR  ADDR_OV7725 			 /*设备地址定义*/

/********************************************************************
 * 函数名：sccb_gpio_config
 * 描述  ：SCCB管脚配置
 * 输入  ：无
 * 输出  ：无
 * 注意  ：无        
 ********************************************************************/
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

/********************************************************************
 * 函数名：sccb_delay
 * 描述  ：延迟时间
 * 输入  ：无
 * 输出  ：无
 * 注意  ：内部调用        
 ********************************************************************/
static void sccb_delay(void)
{	
   uint16_t i = 800; 
   while(i) 
   { 
     i--; 
   } 
}

/********************************************************************
 * 函数名：sccb_start
 * 描述  ：SCCB起始信号
 * 输入  ：无
 * 输出  ：无
 * 注意  ：内部调用        
 ********************************************************************/
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

/********************************************************************
 * 函数名：sccb_stop
 * 描述  ：SCCB停止信号
 * 输入  ：无
 * 输出  ：无
 * 注意  ：内部调用        
 ********************************************************************/
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

/********************************************************************
 * 函数名：sccb_ack
 * 描述  ：SCCB应答方式
 * 输入  ：无
 * 输出  ：无
 * 注意  ：内部调用        
 ********************************************************************/
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

/********************************************************************
 * 函数名：sccb_no_ack
 * 描述  ：SCCB 无应答方式
 * 输入  ：无
 * 输出  ：无
 * 注意  ：内部调用        
 ********************************************************************/
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

/********************************************************************
 * 函数名：sccb_wait_ack
 * 描述  ：SCCB 等待应答
 * 输入  ：无
 * 输出  ：返回为:=1有ACK,=0无ACK
 * 注意  ：内部调用        
 ********************************************************************/
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

 /*******************************************************************
 * 函数名：sccb_send_byte
 * 描述  ：数据从高位到低位
 * 输入  ：SendByte: 发送的数据
 * 输出  ：无
 * 注意  ：内部调用        
 *********************************************************************/
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

 /******************************************************************
 * 函数名：sccb_receive_byte
 * 描述  ：数据从高位到低位
 * 输入  ：无
 * 输出  ：SCCB总线返回的数据
 * 注意  ：内部调用        
 *******************************************************************/
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

 /*****************************************************************************************
 * 函数名：sccb_write_byte
 * 描述  ：写一字节数据
 * 输入  ：- WriteAddress: 待写入地址 	- SendByte: 待写入数据	- DeviceAddress: 器件类型
 * 输出  ：返回为:=1成功写入,=0失败
 * 注意  ：无        
 *****************************************************************************************/           
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

/******************************************************************************************************************
 * 函数名：sccb_read_byte
 * 描述  ：读取一串数据
 * 输入  ：- pBuffer: 存放读出数据 	- length: 待读出长度	- ReadAddress: 待读出地址		 - DeviceAddress: 器件类型
 * 输出  ：返回为:=1成功读入,=0失败
 * 注意  ：无        
 **********************************************************************************************************************/           
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
