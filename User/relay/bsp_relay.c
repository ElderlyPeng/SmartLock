#include "..\relay\bsp_relay.h"

/**
  * @brief  ��ʼ�����Ƽ̵�����IO
  * @param  ��
  * @retval ��
  */
void Relay_GPIO_Config(void)
{
    /*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
    GPIO_InitTypeDef GPIO_InitStructure;

    /*����LED��ص�GPIO����ʱ��*/
    RCC_APB2PeriphClockCmd( PowerAll_GPIO_CLK | PowerDevice_GPIO_CLK, ENABLE);

    /*����Ҫ���Ƶ�GPIO����*/
    GPIO_InitStructure.GPIO_Pin = PowerAll_GPIO_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(PowerAll_GPIO_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = PowerDevice_GPIO_PIN;
    GPIO_Init(PowerDevice_GPIO_PORT, &GPIO_InitStructure);

    /* ��ʼ���������ر����м̵���*/
    PowerAll_OFF;
    PowerDevice_OFF;
}
