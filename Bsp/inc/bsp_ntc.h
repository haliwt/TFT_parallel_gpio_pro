#ifndef __BSP_NTC_H_
#define __BSP_NTC_H_
#include "main.h"

#define PTC_RES_NTC_DIS     0

typedef enum{
   decimals,
   integer

}read_ntc_value;









//void Read_NTC_Temperature_Power_On(void);

void Read_NTC_Temperature_Value_Handler(void);
//void Calculate_Speicial_Temperature_Value(uint8_t temp);

void Get_Ntc_Resistance_Temperature_Voltage(uint32_t channel,uint8_t times);

uint8_t ntc_res_linear_value(uint8_t ntc_value);

void Judge_NTC_Temperature_Value(uint16_t adc_ptc);



#endif 
