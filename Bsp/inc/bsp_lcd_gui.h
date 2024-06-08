#ifndef __BSP_LCD_GUI_H
#define __BSP_LCD_GUI_H
#include "main.h"

#define TFT_DISP_TEMP_24   0


#define TIMER_X0    100
#define TIMER_Y     150

#define TIMER_X1    125
#define TIMER_X2    150
#define TIMER_X3  	175

#define DIS_TIME_X0   80
#define DIS_TIME_Y    190

#define DIS_TIME_X1    114

#define DIS_TIME_X2    166

#define DIS_TIME_X3    200









void TFT_DrawPoint(uint16_t x,uint16_t y,uint16_t color);

void TFT_SetWindow(uint16_t xstart,uint16_t ystart,uint16_t xend,uint16_t yend);


void TFT_St7789_SetBGColor(uint32_t color);

void TFT_St7789_FillBlock(uint16_t xstart,uint16_t ystart,uint16_t block_width,uint16_t block_height,uint16_t color);


void TFT_ST7789_FillPicture(uint16_t xstart,uint16_t ystart,uint16_t block_width,uint16_t block_height,const uint8_t *black_data);


void TFT_Disp_Temp_Value(uint8_t bc,uint8_t temp_value);
void TFT_Disp_Humidity_Value(uint8_t hum_value);







void TFT_ShowString(uint16_t x,uint16_t y,char *str,uint8_t fw,uint8_t fh,uint8_t mode);

//void TFT_ShowFont(uint8_t x,uint8_t y,char *font,uint8_t fw,uint8_t fh,uint8_t mode);

void TFT_ShowText(uint16_t x,uint16_t y,char *str,uint8_t fw,uint8_t fh,uint8_t mode);

void TFT_ShowPicture(uint16_t x,uint16_t y,const uint8_t *p,uint8_t pw,uint8_t ph);

void TFT_Works_Or_Timer_times_Handler(void);

void TFT_Display_Handler(void);

void TFT_Works_Or_Timer_times_Handler(void);

void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

void TFT_Display_Temp_Symbol(void);

void TFT_Display_Humidity_Symbol(void);

void TFT_Display_WorksTime(void);
void TFT_Only_Disp_Timing(void);



void TFT_Disp_Set_TimerTime(uint8_t bc);
void TFT_Disp_Set_TimerTime_Init(void);




void TFT_DonnotDisp_Works_Time(void);


void TFT_Disp_Voice_Temp_Value(uint8_t bc,uint8_t temp_value);

void TFT_Disp_Voice_Set_TimerTime_Init(void);

void TFT_Disp_Onley_Set_TimerTime_Value(void);

void TFT_Only_Disp_Set_Timer_Blink(void);

void TFT_Display_WorksTime_Voice(void);

void TFT_Display_PowerOn_WorksTime_Init(void);

void TFT_Disp_Only_Humidity_Numbers(uint8_t hum_value);

void TFT_Disp_Only_Temp_Numbers(uint8_t bc,uint8_t temp_value);

void TFT_Disp_Temp_SwitchSet_Value(uint8_t bc,uint8_t temp_value);

void TFT_Display_Update_Handler(void);

void TFT_Display_Precise_WorksTime(void);

void TFT_Display_PowerOn_Init_Handler(void);

void TFT_Disp_CountDown_60s(uint8_t idata);



#endif

