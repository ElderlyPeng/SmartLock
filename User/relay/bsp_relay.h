#ifndef __RELAY_H
#define	__RELAY_H


#include "stm32f10x.h"

// �ܵ�Դ����
#define PowerAll_GPIO_PORT    	GPIOB		             	  /* GPIO�˿� */
#define PowerAll_GPIO_CLK 	    RCC_APB2Periph_GPIOB		  /* GPIO�˿�ʱ�� */
#define PowerAll_GPIO_PIN		GPIO_Pin_11			          /* ���ӵ�SCLʱ���ߵ�GPIO */

// ģ���Դ����
#define PowerDevice_GPIO_PORT    	GPIOB		         	   /* GPIO�˿� */
#define PowerDevice_GPIO_CLK 	    RCC_APB2Periph_GPIOB	   /* GPIO�˿�ʱ�� */
#define PowerDevice_GPIO_PIN		GPIO_Pin_10	   			   /* ���ӵ�SCLʱ���ߵ�GPIO */


/** the macro definition to trigger the led on or off
  * 1 - off
  * 0 - on
  */
#define ON  1
#define OFF 0

/* ʹ�ñ�׼�Ĺ̼������IO*/
#define PowerAll(a)	if (a)	\
					GPIO_SetBits(PowerAll_GPIO_PORT,PowerAll_GPIO_PIN);\
					else		\
					GPIO_ResetBits(PowerAll_GPIO_PORT,PowerAll_GPIO_PIN)

#define PowerDevice(a)	if (a)	\
						GPIO_SetBits(PowerDevice_GPIO_PORT,PowerDevice_GPIO_PIN);\
						else		\
						GPIO_ResetBits(PowerDevice_GPIO_PORT,PowerDevice_GPIO_PIN)

/* �������IO�ĺ� */
#define digitalToggle(p,i) {p->ODR ^=i;}
						
#define PowerAll_TOGGLE		 digitalToggle(PowerAll_GPIO_PORT, PowerAll_GPIO_PIN)
#define PowerAll_OFF		 PowerAll(OFF)
#define PowerAll_ON			 PowerAll(ON)

#define PowerDevice_TOGGLE	 digitalToggle(PowerDevice_GPIO_PORT, PowerDevice_GPIO_PIN)
#define PowerDevice_OFF		 PowerDevice(OFF)
#define PowerDevice_ON		 PowerDevice(ON)

void Relay_GPIO_Config(void);

#endif /* __RELAY_H */
