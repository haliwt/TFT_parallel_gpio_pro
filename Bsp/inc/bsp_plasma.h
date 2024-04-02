#ifndef __BSP_PLASMA_H
#define __BSP_PLASMA_H
#include "main.h"

#define PLASMA_SetHigh()            HAL_GPIO_WritePin(PLASMA_CTL_GPIO_Port,PLASMA_CTL_Pin ,GPIO_PIN_SET)    // output high level
#define PLASMA_SetLow()             HAL_GPIO_WritePin(PLASMA_CTL_GPIO_Port,PLASMA_CTL_Pin ,GPIO_PIN_RESET)   

void Plasma_On(void);
void Plasma_Off(void);



#endif 
