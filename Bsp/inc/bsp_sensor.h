#ifndef  __BSP_SENSOR_H_
#define __BSP_SENSOR_H_
#include "main.h"


#define GX_ADDR_W     0xE0
#define GX_ADDR_R     0xE1

//#define GX_WK_UP    0x3517


//extern uint8_t i2c_read_data[6];



//uint8_t  aht30_read_sensor_humidity_value(void);

//uint8_t  aht30_read_sensor_temperature_value(void);


void Update_DHT11_Value(void);

void Update_Dht11_Totencent_Value(void);


#endif 

















