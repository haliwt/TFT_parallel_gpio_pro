#ifndef __BSP_LCD_H
#define __BSP_LCD_H
#include "main.h"


//LCD屏幕分辨率定义
//横屏显示
#define LCD_Width   320//240
#define LCD_Height  240 //320
//颜色定义
#define WHITE   0xFFFF  //白色
#define YELLOW  0xFFE0  //黄色
#define BRRED   0XFC07  //棕红色
#define PINK    0XF81F  //粉色
#define RED     0xF800  //红色
#define BROWN   0XBC40  //棕色
#define GRAY    0X8430  //灰色
#define GBLUE   0X07FF  //兰色
#define GREEN   0x07E0  //绿色
#define BLUE    0x001F  //蓝色
#define BLACK   0x0000  //黑色

extern uint8_t spi_tx_buffer[1];

#define TFT_BACKLIGHT_ON()     HAL_GPIO_WritePin(TFT_BACKLIGHT_GPIO_Port,TFT_BACKLIGHT_Pin,GPIO_PIN_RESET) 
#define TFT_BACKLIGHT_OFF()     HAL_GPIO_WritePin(TFT_BACKLIGHT_GPIO_Port,TFT_BACKLIGHT_Pin,GPIO_PIN_SET)

#define TFT_DCX_CMD()     HAL_GPIO_WritePin(TFT_DCX_GPIO_Port,TFT_DCX_Pin,GPIO_PIN_RESET)  
#define TFT_DCX_DATA()     HAL_GPIO_WritePin(TFT_DCX_GPIO_Port,TFT_DCX_Pin,GPIO_PIN_SET)


#define LCD_RST_SetHigh()  HAL_GPIO_WritePin(TFT_RESET_GPIO_Port,TFT_RESET_Pin,GPIO_PIN_SET)

#define LCD_RST_SetLow()   HAL_GPIO_WritePin(TFT_RESET_GPIO_Port,TFT_RESET_Pin,GPIO_PIN_RESET)

#define LCD_CS_SetHigh()   HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin,GPIO_PIN_SET)

#define LCD_CS_SetLow()    HAL_GPIO_WritePin(TFT_CS_GPIO_Port,TFT_CS_Pin,GPIO_PIN_RESET)


#define LCD_WR_SetHigh()    HAL_GPIO_WritePin(TFT_WR_GPIO_Port,TFT_WR_Pin,GPIO_PIN_SET)
#define LCD_WR_SetLow()     HAL_GPIO_WritePin(TFT_WR_GPIO_Port,TFT_WR_Pin,GPIO_PIN_RESET)

#define LCD_RD_SetHigh()    HAL_GPIO_WritePin(TFT_RD_GPIO_Port,TFT_RD_Pin,GPIO_PIN_SET)
#define LCD_RD_SetLow()     HAL_GPIO_WritePin(TFT_RD_GPIO_Port,TFT_RD_Pin,GPIO_PIN_RESET)





//parallel 8080 tft data IF
#define LCD_DB7_SetHigh() 		(HAL_GPIO_WritePin(TFT_DB7_GPIO_Port,TFT_DB7_Pin,GPIO_PIN_SET))
#define LCD_DB7_SetLow()  		(HAL_GPIO_WritePin(TFT_DB7_GPIO_Port,TFT_DB7_Pin,GPIO_PIN_SET))

#define LCD_DB6_SetHigh()  		(HAL_GPIO_WritePin(TFT_DB6_GPIO_Port,TFT_DB6_Pin,GPIO_PIN_SET))
#define LCD_DB6_SetLow()  		(HAL_GPIO_WritePin(TFT_DB6_GPIO_Port,TFT_DB6_Pin,GPIO_PIN_SET))

#define LCD_DB5_SetHigh()  		(HAL_GPIO_WritePin(TFT_DB5_GPIO_Port,TFT_DB5_Pin,GPIO_PIN_SET))
#define LCD_DB5_SetLow()  		(HAL_GPIO_WritePin(TFT_DB5_GPIO_Port,TFT_DB5_Pin,GPIO_PIN_SET))

#define LCD_DB4_SetHigh()  		(HAL_GPIO_WritePin(TFT_DB4_GPIO_Port,TFT_DB4_Pin,GPIO_PIN_SET))
#define LCD_DB4_SetLow()  		(HAL_GPIO_WritePin(TFT_DB4_GPIO_Port,TFT_DB4_Pin,GPIO_PIN_SET))

#define LCD_DB3_SetHigh()  		(HAL_GPIO_WritePin(TFT_DB3_GPIO_Port,TFT_DB3_Pin,GPIO_PIN_SET))
#define LCD_DB3_SetLow()  		(HAL_GPIO_WritePin(TFT_DB3_GPIO_Port,TFT_DB3_Pin,GPIO_PIN_SET))

#define LCD_DB2_SetHigh()  		(HAL_GPIO_WritePin(TFT_DB2_GPIO_Port,TFT_DB2_Pin,GPIO_PIN_SET))
#define LCD_DB2_SetLow()  		(HAL_GPIO_WritePin(TFT_DB2_GPIO_Port,TFT_DB2_Pin,GPIO_PIN_SET))

#define LCD_DB1_SetHigh()  		(HAL_GPIO_WritePin(TFT_DB1_GPIO_Port,TFT_DB1_Pin,GPIO_PIN_SET))
#define LCD_DB1_SetLow()  		(HAL_GPIO_WritePin(TFT_DB1_GPIO_Port,TFT_DB1_Pin,GPIO_PIN_SET))

#define LCD_DB0_SetHigh()  		(HAL_GPIO_WritePin(TFT_DB0_GPIO_Port,TFT_DB0_Pin,GPIO_PIN_SET))
#define LCD_DB0_SetLow()  		(HAL_GPIO_WritePin(TFT_DB0_GPIO_Port,TFT_DB0_Pin,GPIO_PIN_SET))










void TFT_LCD_Init(void);


void LCD_GPIO_Reset(void);
void LCD_Clear(uint16_t color);


void LCD_Display_BacklightOn(void);

void LCD_Display_BacklightOff(void);

void LCD_Write_Cmd(uint8_t cmd);

void LCD_Write_Data(uint8_t data);

void LCD_Write_16bit_Data(uint16_t data);

void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);


void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2);


void Frame(void);

void DISP_WINDOWS(void);



#endif 

