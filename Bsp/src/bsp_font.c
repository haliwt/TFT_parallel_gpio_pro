#include "bsp_font.h"
#include "bsp.h"

#define  CMD_SetPixel	0x2C	     //填充像素

/***************************** ILI934 显示区域的起始坐标和总行列数 ***************************/
#define      ILI9341_DispWindow_X_Star		    0     //起始点的X坐标
#define      ILI9341_DispWindow_Y_Star		    0     //起始点的Y坐标


uint16_t LCD_X_LENGTH = 320;
uint16_t LCD_Y_LENGTH = 240;


/****************************************************************************************
	*@brief TFT_display_char16_16
	*@details 显示16x16的汉字
	*@param[in] address:图片数据地址
	*			startX：X起始坐标
	*			startY：Y起始坐标
	*			textColor：字体显示颜色
	*			backgroundColor:背景色
	*@return void
	*@author zx
	*@date 2023-06-04
******************************************************************************************/
void TFT_display_char16_16(const uint8_t *address ,uint16_t startX,uint16_t startY,
							uint16_t textColor, uint16_t backgroundColor)
{
	unsigned int column;
	unsigned char tm=0,temp;

	//TFT_SetWindows(startX, startY, 16, 16);
    TFT_SetWindow(startX, startY, 32, 32);
	
	for(column = 0; column < 32; column++)  //column loop
	{
		temp =* address;
		for(tm = 8; tm > 0; tm--)//for(tm = 0; tm < 8; tm++)
		{
			if(temp&0x01)
			{
				LCD_Write_Data(textColor>>8);//TFT_SEND_DATA(textColor>>8);
				LCD_Write_Data(textColor);//TFT_SEND_DATA(textColor);
			}
			else 
			{
				LCD_Write_Data(backgroundColor>>8);//TFT_SEND_DATA(backgroundColor>>8);
				LCD_Write_Data(backgroundColor);//TFT_SEND_DATA(backgroundColor);
			}
			temp >>= 1;
		}
		address++;
	}
} 

/**************************************************************************************************************
*@brief TFT_display_char16_16_noBackColor
*@details 显示16x16的汉字(不带背景颜色，镂空)
*		  显示规则：一行一行显示，首先显示第一行的前八位，
*		  然后显示后八位，显示完成之后显示第二行，
*		  注意：数据取模时是低位在前高位在后（逆序），具体根
*		  据取模方向来确定
*@param[in] address:图片数据地址
*			startX：X起始坐标
*			startY：Y起始坐标
*			color：字体显示颜色
*@return void
*@author zx
*@date 2023-06-04
**************************************************************************************************************/
#if 0
void TFT_display_char16_16_English_noBackColor(const uint8_t *address ,uint16_t startX,uint16_t startY,uint16_t color)
{

	uint16_t column;
	uint8_t tm=0;
	uint8_t temp;
	uint16_t x = 0;
	uint16_t y = 0;
	for(column = 0; column < 16; column++) //2个字 1个字是16 
	{
		temp =* address;
	   for(tm = 8; tm > 0; tm--)//for(tm = 0; tm < 8; tm++)
		{			
			if(temp&0x01)
			{
				//TFT_display_point(startX+ tm, startY+ y ,color);
				 TFT_DrawPoint(startX+tm, startY+ y ,color);
			}
			
			temp >>= 1;
		}
//		if(column>0 && column%2 == 0)//如果开启字体的高读会压缩到之前的一半
		y++;
		address++;
	}	

}

#endif 


/**************************************************************************************************************
*@brief TFT_display_char48_48_noBackColor
*@details 显示16x16的汉字(不带背景颜色，镂空)
*		  显示规则：一行一行显示，首先显示第一行的前八位，
*		  然后显示后八位，显示完成之后显示第二行，
*		  注意：数据取模时是低位在前高位在后（逆序），具体根
*		  据取模方向来确定
*@param[in] address:图片数据地址
*			startX：X起始坐标
*			startY：Y起始坐标
*			color：字体显示颜色
*@return void
*@author zx
*@date 2023-06-04
**************************************************************************************************************/
#if 0
void TFT_display_char32_32_English_noBackColor(const uint8_t *address ,uint16_t startX,uint16_t startY,uint16_t color)
{

	uint16_t column;
	uint8_t tm=0;
	uint8_t temp;
	uint16_t x = 0;
	uint16_t y = 0;
	for(column = 0; column < 16; column++) //2个字 1个字是16 
	{
	   temp =* address;
	   for(tm = 8; tm > 0; tm--)//for(tm = 0; tm < 8; tm++)
		{			
			if(temp&0x01)
			{
				//TFT_display_point(startX+ tm, startY+ y ,color);
				 TFT_DrawPoint(startX+tm, startY+ y ,color);
			}
			
			temp >>= 1;
		}

	    address++;
		temp =* address;
		for(tm = 8; tm > 0; tm--) //for(tm = 0; tm < 8; tm++)
		{			
			if(temp&0x01)
			{
				//TFT_display_point(startX+ tm+8, startY+ y ,color);
				TFT_DrawPoint(startX+tm+8, startY+ y ,color);
			}
			
			temp >>= 1;
			 
		}
       //2
		address++;
		temp =*(address+32);
		for(tm = 8; tm > 0; tm--) //for(tm = 0; tm < 8; tm++)
		{			
			if(temp&0x01)
			{
				//TFT_display_point(startX+ tm+8, startY+ y ,color);
				TFT_DrawPoint(startX+tm+9, startY+ y ,color);
			}
			
			temp >>= 1;
			 
		}
		address++;
		temp =* (address+32);
		for(tm = 8; tm > 0; tm--) //for(tm = 0; tm < 8; tm++)
		{			
			if(temp&0x01)
			{
				//TFT_display_point(startX+ tm+8, startY+ y ,color);
				TFT_DrawPoint(startX+tm+17, startY+ y ,color);
			}
			
			temp >>= 1;
			 
		}
        //3
		address++;
		temp =* (address+64);
		for(tm = 8; tm > 0; tm--) //for(tm = 0; tm < 8; tm++)
		{			
			if(temp&0x01)
			{
				//TFT_display_point(startX+ tm+8, startY+ y ,color);
				TFT_DrawPoint(startX+tm+18, startY+ y ,color);
			}
			
			temp >>= 1;
			 
		}
		address++;
		temp =*(address+64);
		for(tm = 8; tm > 0; tm--) //for(tm = 0; tm < 8; tm++)
		{			
			if(temp&0x01)
			{
				//TFT_display_point(startX+ tm+8, startY+ y ,color);
				TFT_DrawPoint(startX+tm+26, startY+ y ,color);
			}
			
			temp >>= 1;
			 
		}
		//4
		address++;
		temp =* (address+96);
		for(tm = 8; tm > 0; tm--) //for(tm = 0; tm < 8; tm++)
		{			
			if(temp&0x01)
			{
				//TFT_display_point(startX+ tm+8, startY+ y ,color);
				TFT_DrawPoint(startX+tm+27, startY+ y ,color);
			}
			
			temp >>= 1;
			 
		}
		address++;
		temp =*(address+96);
		for(tm = 8; tm > 0; tm--) //for(tm = 0; tm < 8; tm++)
		{			
			if(temp&0x01)
			{
				//TFT_display_point(startX+ tm+8, startY+ y ,color);
				TFT_DrawPoint(startX+tm+35, startY+ y ,color);
			}
			
			temp >>= 1;
			 
		}
		
		
			 
		
//		if(column>0 && column%2 == 0)//如果开启字体的高读会压缩到之前的一半
		y++;
		address++;
	}	

}

void TFT_display_char32_32_OneEnglish_noBackColor(const uint8_t *address ,uint16_t startX,uint16_t startY,uint16_t color)
{

	uint16_t column;
	uint8_t tm=0;
	uint8_t temp;
	uint16_t x = 0;
	uint16_t y = 0;
	for(column = 0; column < 32; column++) //2个字 1个字是16 
	{
	  temp =* address;
	   for(tm = 8; tm > 0; tm--)//for(tm = 0; tm < 8; tm++)
		{			
			if(temp&0x01)
			{
				//TFT_display_point(startX+ tm, startY+ y ,color);
				 TFT_DrawPoint(startX+tm, startY+ y ,color);
			}
			
			temp >>= 1;
		}
		address++;
		temp =* address;
		for(tm = 8; tm > 0; tm--) //for(tm = 0; tm < 8; tm++)
		{			
			if(temp&0x01)
			{
				//TFT_display_point(startX+ tm+8, startY+ y ,color);
				TFT_DrawPoint(startX+tm+8, startY+ y ,color);
			}
			
			temp >>= 1;
			 
		}
		
//		if(column>0 && column%2 == 0)//如果开启字体的高读会压缩到之前的一半
		y++;
		address++;
	}	

}
#endif 


