#ifndef __SIM800_H
#define	__SIM800_H


#include "stm32f10x.h"

int SIM800_Init(void);
int SIM800_Call(void);
int SIM800_Connect(void);
int SIM800_Break(void);
int SIM800_WaitCMD(void);

#endif /* __SIM800_H */
