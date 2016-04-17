/*********************** HNIT 3103 Application Team **************************
 * 文 件 名 ：hnit_key.h
 * 描    述 ：独立按键扫描        
 * 实验平台 ：STM32F407开发板
 * 库 版 本 ：ST1.4.0
 * 时    间 ：2016.3.19
 * 作    者 ：3103创新团队
 * 修改记录 ：无
******************************************************************************/

#include "hnit_key.h"

/*****************************************************************************
*	函 数 名: KEY_Init
*	功    能: 按键扫描GPIO配置
*   调用函数：GPIO类库函数
*	形    参：无
*	返 回 值: 无
******************************************************************************/	
void key_init(void)
{    	 
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);  //使能GPIOE时钟

    /* GPIOE3,E4初始化设置 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;         //普通输入模式
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;        //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;    //100MHz
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;          //上拉
    GPIO_Init(GPIOE, &GPIO_InitStructure);                //初始化	
}

/******************************* END OF FILE *********************************/
