#ifndef __BSP_FONT_APP_H
#define __BSP_FONT_APP_H
#include "main.h"


void TFT_Disp_WorksTime_24_24_onBlack(uint16_t x,uint16_t y,uint8_t sel,uint8_t num);

void TFT_Disp_Temp_24_24_onBlack(uint16_t x,uint16_t y,uint8_t num);

void TFT_Disp_Temp_Symbol_24_24(uint16_t x,uint16_t y);


void TFT_Disp_Humidity_Symbol_24_24(uint16_t x,uint16_t y);

void TFT_Disp_Time_Split_Symbol(uint16_t x,uint16_t y,uint8_t dat);

void TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(uint16_t x,uint16_t y,uint8_t sel ,uint8_t num);

void TFT_Disp_Temp_20_20(uint16_t x,uint16_t y,uint8_t num);

void TFT_Disp_Humidity_20_20(uint16_t x,uint16_t y,uint8_t num);


void TFT_Disp_Numbers_Pic_413(uint16_t x,uint16_t y,uint8_t bc,uint8_t num);

void TFT_Disp_Timer_Split_Symbol(void);

void TFT_Disp_Pic_Warnign_Words(uint16_t x,uint16_t y,uint8_t sel,uint8_t num);





#endif 


