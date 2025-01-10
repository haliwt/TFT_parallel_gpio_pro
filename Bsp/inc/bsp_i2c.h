#ifndef __BSP_I2C_H
#define __BSP_I2C_H
#include "main.h"


void EE_IIC_Init(void);


/* Private function prototypes -----------------------------------------------*/
//void I2C_Configuration(void);
//void EE_IIC_Init(void);

//uint8_t I2C_Read(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t addr,uint8_t *buf,uint16_t num);
//uint8_t I2C_Write(I2C_TypeDef *I2Cx,uint8_t I2C_Addr,uint8_t addr,uint8_t *buf,uint16_t num);

void aht30_read_ee_i2c_data(uint8_t *buf);







#endif 

