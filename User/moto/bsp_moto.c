#include "..\moto\bsp_moto.h"

static void MOTO_TIM_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    // ����Ƚ�ͨ�� GPIO ��ʼ��
    RCC_APB2PeriphClockCmd(MOTO_TIM_CH1_GPIO_CLK, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin =  MOTO_TIM_CH1_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(MOTO_TIM_CH1_PORT, &GPIO_InitStructure);
	
	// ��ͨ GPIO ��ʼ��
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

    // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    MOTO_TIM_APBxClock_FUN(MOTO_TIM_CLK,ENABLE);

    /*--------------------ʱ���ṹ���ʼ��-------------------------*/
    // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period=MOTO_TIM_PERIOD;
    // ����CNT��������ʱ�� = Fck_int/(psc+1)
    TIM_TimeBaseStructure.TIM_Prescaler= MOTO_TIM_PSC;
    // ʱ�ӷ�Ƶ���� ����������ʱ��ʱ��Ҫ�õ�
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    // ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    // �ظ���������ֵ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
    // ��ʼ����ʱ��
    TIM_TimeBaseInit(MOTO_TIM, &TIM_TimeBaseStructure);
    /*--------------------����ȽϽṹ���ʼ��-------------------*/
    // ����ΪPWMģʽ1
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    // ���ʹ��
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    // ���������ʹ��
    TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
    // ����ռ�ձȴ�С
    TIM_OCInitStructure.TIM_Pulse = MOTO_TIM_PULSE;
    // ���ͨ����ƽ��������
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    // ���ͨ�����е�ƽ��������
    TIM_OCInitStructure.TIM_OCIdleState = TIM_OCNIdleState_Reset;
    TIM_OC1Init(MOTO_TIM, &TIM_OCInitStructure);
    TIM_OC1PreloadConfig(MOTO_TIM, TIM_OCPreload_Enable);	
	/*-------------------ɲ���������ṹ���ʼ��-------------------*/
	TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Disable;
	TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Disable;
	TIM_BDTRConfig(MOTO_TIM, &TIM_BDTRInitStructure);
	
    // ʹ�ܼ�����
    TIM_Cmd(MOTO_TIM, ENABLE);
    // �����ʹ�ܣ���ʹ�õ���ͨ�ö�ʱ��ʱ����䲻��Ҫ
    TIM_CtrlPWMOutputs(MOTO_TIM, ENABLE);
}

// �����ʼ��
void MOTO_TIM_Init(void)
{
    MOTO_TIM_GPIO_Config();
    MOTO_TIM_Mode_Config();
}

// ���������� 0-ֹͣ 1-��ת 2-��ת0
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


