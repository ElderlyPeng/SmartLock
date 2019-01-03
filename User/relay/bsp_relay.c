#include "..\relay\bsp_relay.h"

/**
  * @brief  初始化控制继电器的IO
  * @param  无
  * @retval 无
  */
void Relay_GPIO_Config(void)
{
    /*定义一个GPIO_InitTypeDef类型的结构体*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*开启LED相关的GPIO外设时钟*/
    RCC_APB2PeriphClockCmd( PowerAll_GPIO_CLK | PowerDevice_GPIO_CLK, ENABLE);

    /*配置要控制的GPIO引脚*/
    GPIO_InitStructure.GPIO_Pin = PowerAll_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PowerAll_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = PowerDevice_GPIO_PIN;
    GPIO_Init(PowerDevice_GPIO_PORT, &GPIO_InitStructure);

    /* 初始化后立即关闭所有继电器*/
    PowerAll_OFF;
    PowerDevice_OFF;
}
