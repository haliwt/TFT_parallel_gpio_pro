#ifndef __BSP_ST7789V_H
#define __BSP_ST7789V_H
#include "main.h"
#include "gpio.h"

#define SDA_SetHigh()     (HAL_GPIO_WritePin(SDA_GPIO_Port,SDA_Pin,GPIO_PIN_SET))  
#define SDA_SetLow()      (HAL_GPIO_WritePin(SDA_GPIO_Port,SDA_Pin,GPIO_PIN_RESET))
 

#define SCL_SetHigh()     (HAL_GPIO_WritePin(SCL_GPIO_Port,SCL_Pin,GPIO_PIN_SET))  
#define SCL_SetLow()      (HAL_GPIO_WritePin(SCL_GPIO_Port,SCL_Pin,GPIO_PIN_RESET))


void St7789v_Driver(void);

#endif 

