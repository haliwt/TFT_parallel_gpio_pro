#ifndef __BSP_FONT_H
#define __BSP_FONT_H
#include "main.h"


#define  ST7789V_DispWindow_X_Star    0 

#define  ST7789V_DispWindow_X_Star		    0     //起始点的X坐标
#define  ST7789V_DispWindow_Y_Star		    0     //起始点的Y坐标

#define  ST7789V_LESS_PIXEL	  							240			//液晶屏较短方向的像素宽度
#define  ST7789V_MORE_PIXEL	 


void GUI_DrawFont16(uint16_t x, uint16_t y, uint16_t fontcolor, uint16_t backgroundcolor, uint8_t *s,uint8_t mode);



void TFT_display_char16_16_noBackColor(const uint8_t *address ,uint16_t startX,uint16_t startY,uint16_t color);


void TFT_display_char16_16(const uint8_t *address ,uint16_t startX,uint16_t startY,
							uint16_t textColor, uint16_t backgroundColor);

void TFT_display_char16_16(const uint8_t *address ,uint16_t startX,uint16_t startY,
							uint16_t textColor, uint16_t backgroundColor);
void TFT_display_char16_16_Tow_noBackColor(const uint8_t *address ,uint16_t startX,uint16_t startY,uint16_t color);

void TFT_display_char16_16_Three_noBackColor(const uint8_t *address ,uint16_t startX,uint16_t startY,uint16_t color);

void TFT_display_char16_16_Four_noBackColor(const uint8_t *address ,uint16_t startX,uint16_t startY,uint16_t color);


void TFT_display_char16_16_English_noBackColor(const uint8_t *address ,uint16_t startX,uint16_t startY,uint16_t color);

void TFT_display_char32_32_OneEnglish_noBackColor(const uint8_t *address ,uint16_t startX,uint16_t startY,uint16_t color);



void TFT_display_char32_32_English_noBackColor(const uint8_t *address ,uint16_t startX,uint16_t startY,uint16_t color);









#endif 

