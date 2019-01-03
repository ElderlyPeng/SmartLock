#include "stm32f10x.h"
#include "..\usart\bsp_usart.h"
#include "..\moto\bsp_moto.h"
#include "..\relay\bsp_relay.h"
#include "..\sim800\bsp_sim800.h"
#include "..\systick\bsp_SysTick.h"
#include <stm32f10x.h>


/**
  * @brief  主函数
  * @param  无
  * @retval 无
  */
int main(void)
{
	int i = 0;
	
	// 
    Relay_GPIO_Config();
    SysTick_Delay_Ms(200);
	for (i=0; i<2; i++)
	{ 
		PowerAll_ON;
		SysTick_Delay_Ms(800);
		PowerAll_OFF;
		SysTick_Delay_Ms(800);
			
	} 
	PowerAll_ON;	
    SysTick_Delay_Ms(200);
    PowerDevice_ON;
    SysTick_Delay_Ms(200);

    // 初始化
    USART_SIM800_Config();
    USART_DEBUG_Config();
    SysTick_Init();
    MOTO_TIM_Init();

    // sim800与服务器通讯,有一步失败则退出
    do
    {
        if (SIM800_Init() == 0) break;
        if (SIM800_Call() == 0) break;
        if (SIM800_Connect() == 0) break;
        if (SIM800_WaitCMD() == 0) break;

        DirectionControl(1);
        SysTick_Delay_Ms(5000);
        DirectionControl(2);
        SysTick_Delay_Ms(1000);
        DirectionControl(0);
    } while(0);
	SIM800_Break();

    PowerDevice_OFF;
    PowerAll_OFF;

    while(1);
}
