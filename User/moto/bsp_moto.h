#ifndef __MOTO_H
#define	__MOTO_H


#include "stm32f10x.h"

#define            MOTO_TIM                   TIM1
#define            MOTO_TIM_APBxClock_FUN     RCC_APB2PeriphClockCmd
#define            MOTO_TIM_CLK               RCC_APB2Periph_TIM1
// PWM 信号的频率 F = TIM_CLK/{(ARR+1)*(PSC+1)}
#define            MOTO_TIM_PERIOD            (100-1)
#define            MOTO_TIM_PSC               (72-1)
#define            MOTO_TIM_PULSE             40

// TIM1 输出比较通道
#define            MOTO_TIM_CH1_GPIO_CLK      RCC_APB2Periph_GPIOA
#define            MOTO_TIM_CH1_PORT          GPIOA
#define            MOTO_TIM_CH1_PIN           GPIO_Pin_8

// 普通GPIO口
#define            MOTO_AI0_GPIO_CLK      	  RCC_APB2Periph_GPIOA
#define            MOTO_AI0_PORT         	  GPIOA
#define            MOTO_AI0_PIN         	  GPIO_Pin_11

#define            MOTO_AI1_GPIO_CLK      	  RCC_APB2Periph_GPIOA
#define            MOTO_AI1_PORT         	  GPIOA
#define            MOTO_AI1_PIN        	      GPIO_Pin_12

/** the macro definition to trigger the led on or off
  * 1 - off
  * 0 - on
  */
#define ON  1
#define OFF 0

/* 使用标准的固件库控制IO*/
#define AI0(a)	if (a)	\
					GPIO_SetBits(MOTO_AI0_PORT,MOTO_AI0_PIN);\
					else		\
					GPIO_ResetBits(MOTO_AI0_PORT,MOTO_AI0_PIN)

#define AI1(a)	if (a)	\
					GPIO_SetBits(MOTO_AI1_PORT,MOTO_AI1_PIN);\
					else		\
					GPIO_ResetBits(MOTO_AI1_PORT,MOTO_AI1_PIN)

void MOTO_TIM_Init(void);
void DirectionControl(uint8_t direction);

#endif /* __MOTO_H */
