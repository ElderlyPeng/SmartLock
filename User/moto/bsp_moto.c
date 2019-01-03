#include "..\moto\bsp_moto.h"

static void MOTO_TIM_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // 输出比较通道 GPIO 初始化
    RCC_APB2PeriphClockCmd(MOTO_TIM_CH1_GPIO_CLK, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin =  MOTO_TIM_CH1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTO_TIM_CH1_PORT, &GPIO_InitStructure);
	
	// 普通 GPIO 初始化
	RCC_APB2PeriphClockCmd(MOTO_AI0_GPIO_CLK|MOTO_AI1_GPIO_CLK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  MOTO_AI0_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(MOTO_AI0_PORT, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  MOTO_AI1_PIN;
	GPIO_Init(MOTO_AI1_PORT, &GPIO_InitStructure);
	
	DirectionControl(0);
}

static void MOTO_TIM_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
	TIM_BDTRInitTypeDef TIM_BDTRInitStructure;

    // 开启定时器时钟,即内部时钟CK_INT=72M
    MOTO_TIM_APBxClock_FUN(MOTO_TIM_CLK,ENABLE);

    /*--------------------时基结构体初始化-------------------------*/
    // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period=MOTO_TIM_PERIOD;
    // 驱动CNT计数器的时钟 = Fck_int/(psc+1)
    TIM_TimeBaseStructure.TIM_Prescaler= MOTO_TIM_PSC;
    // 时钟分频因子 ，配置死区时间时需要用到
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    // 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    // 重复计数器的值，没用到不用管
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
    // 初始化定时器
    TIM_TimeBaseInit(MOTO_TIM, &TIM_TimeBaseStructure);
    /*--------------------输出比较结构体初始化-------------------*/
    // 配置为PWM模式1
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    // 输出使能
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    // 互补输出不使能
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    // 设置占空比大小
    TIM_OCInitStructure.TIM_Pulse = MOTO_TIM_PULSE;
    // 输出通道电平极性配置
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    // 输出通道空闲电平极性配置
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCNIdleState_Reset;
    TIM_OC1Init(MOTO_TIM, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(MOTO_TIM, TIM_OCPreload_Enable);	
	/*-------------------刹车和死区结构体初始化-------------------*/
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;
	TIM_BDTRConfig(MOTO_TIM, &TIM_BDTRInitStructure);
	
    // 使能计数器
    TIM_Cmd(MOTO_TIM, ENABLE);
    // 主输出使能，当使用的是通用定时器时，这句不需要
    TIM_CtrlPWMOutputs(MOTO_TIM, ENABLE);
}

// 电机初始化
void MOTO_TIM_Init(void)
{
    MOTO_TIM_GPIO_Config();
    MOTO_TIM_Mode_Config();
}

// 电机方向控制 0-停止 1-正转 2-反转0
void DirectionControl(uint8_t direction)
{
	switch (direction)
	{
		case 0:
			AI0(OFF);
			AI1(OFF);
			break;
		case 1:
			AI0(ON);
			AI1(OFF);
			break;
		case 2:
			AI0(OFF);
			AI1(ON);
			break;
	}
}


