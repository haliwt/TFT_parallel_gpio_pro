
#ifndef __BSP_FAN_H_
#define __BSP_FAN_H_
#include "main.h"


#define FAN_CCW_SetHigh()            HAL_GPIO_WritePin(FAN_CTL_2_GPIO_Port,FAN_CTL_2_Pin,GPIO_PIN_SET)    // output high level
#define FAN_CCW_SetLow()             HAL_GPIO_WritePin(FAN_CTL_2_GPIO_Port,FAN_CTL_2_Pin,GPIO_PIN_RESET)    // output low level

#define FAN_COM_SetHigh()            HAL_GPIO_WritePin(FAN_CTL_1_GPIO_Port,FAN_CTL_1_Pin,GPIO_PIN_SET)    // output high level
#define FAN_COM_SetLow()             HAL_GPIO_WritePin(FAN_CTL_1_GPIO_Port,FAN_CTL_1_Pin,GPIO_PIN_RESET)    // output low level


void Fan_Run(void);

 
void Fan_Stop(void);

void RunMain_And_Interval_Handler(void);

void Display_Fan_Notworking_Words(uint8_t disp);


#endif

