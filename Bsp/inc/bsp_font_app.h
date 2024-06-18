#ifndef __BSP_FONT_APP_H
#define __BSP_FONT_APP_H
#include "main.h"

#define NORMAL_DISPLAY    0





void TFT_Disp_Temp_24_24_onBlack(uint16_t x,uint16_t y,uint8_t num);

void TFT_Disp_Temp_Symbol_24_24(uint16_t x,uint16_t y);


void TFT_Disp_Humidity_Symbol_24_24(uint16_t x,uint16_t y);

void TFT_Disp_Time_Split_Symbol(uint16_t x,uint16_t y,uint8_t dat);

void TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(uint16_t x,uint16_t y,uint8_t sel ,uint8_t num);



void TFT_Disp_Chinese_Humidity_39_18(uint16_t x,uint16_t y,uint8_t num);



void TFT_Disp_Temp_Symbol_16_15(uint16_t x,uint16_t y);


void TFT_Disp_Numbers_Pic_413(uint16_t x,uint16_t y,uint8_t bc,uint8_t num);

void TFT_Disp_Timer_Split_Symbol(void);

void TFT_Disp_Pic_Warnign_Words(uint16_t x,uint16_t y,uint8_t sel,uint8_t num);



void TFT_MainDisp_Numbers_Pic_354(uint16_t x,uint16_t y,uint8_t bc,uint8_t num);

void TFT_Disp_Humidity_Symbol_16_16(uint16_t x,uint16_t y);


void TFT_Disp_Chinese_Temp_36_18(uint16_t x,uint16_t y);




void TFT_Disp_Fan_RunIcon(uint16_t x,uint16_t y,uint8_t num,uint8_t bc);



void TFT_Disp_Chinese_Timer_23_23(uint16_t x,uint16_t y,uint8_t num);
void TFT_Disp_Chinese_WorkTime_23_23(uint16_t x,uint16_t y,uint8_t num);
void TFT_Disp_Chinese_BeiJing_Time_23_23(uint16_t x,uint16_t y,uint8_t num);



void TFT_Disp_Fan_Leasefiness_RunIcon(uint16_t x,uint16_t y,uint8_t num);

void TFT_Disp_Fan_Leasefiness2_RunIcon(uint16_t x,uint16_t y,uint8_t num);

void TFT_donotDisp_Chinese_WorkTime_23_23(uint16_t x,uint16_t y,uint8_t num);


void TFT_Disp_CountDown_Time_48_48_onBlack(uint16_t x,uint16_t y,uint8_t sel,uint8_t num);

#endif 

