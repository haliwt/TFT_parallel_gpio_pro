#ifndef __BSP_TIME_H
#define __BSP_TIME_H
#include "main.h"


void TFT_Display_WorksTime(void);



void TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(uint16_t x,uint16_t y,uint8_t sel ,uint8_t num);




void tft_at_once_disp_time(void);


void TimeTimer_Pro_Handler(void);






#endif 

