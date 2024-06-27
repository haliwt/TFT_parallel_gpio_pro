
#include "bsp.h"

#define HORIZON   1


#if 1
#define SET_D0_H()   do{TFT_DB0_GPIO_Port->BSRR |= TFT_DB0_Pin;}while(0)//HAL_GPIO_WritePin(TFT_DB0_GPIO_Port,TFT_DB0_Pin,GPIO_PIN_SET)
#define SET_D0_L()   do{TFT_DB0_GPIO_Port->BSRR |= (uint32_t)TFT_DB0_Pin << 16;}while(0)//HAL_GPIO_WritePin(TFT_DB0_GPIO_Port,TFT_DB0_Pin,GPIO_PIN_RESET)

#define SET_D1_H()   do{TFT_DB1_GPIO_Port->BSRR |= TFT_DB1_Pin;}while(0)//HAL_GPIO_WritePin(TFT_DB1_GPIO_Port,TFT_DB1_Pin,GPIO_PIN_SET)
#define SET_D1_L()   do{TFT_DB1_GPIO_Port->BSRR |= (uint32_t)TFT_DB1_Pin << 16;}while(0)//HAL_GPIO_WritePin(TFT_DB1_GPIO_Port,TFT_DB1_Pin,GPIO_PIN_RESET)

#define SET_D2_H()   do{TFT_DB2_GPIO_Port->BSRR |= TFT_DB2_Pin;}while(0)//HAL_GPIO_WritePin(TFT_DB2_GPIO_Port,TFT_DB2_Pin,GPIO_PIN_SET)
#define SET_D2_L()   do{TFT_DB2_GPIO_Port->BSRR |= (uint32_t)TFT_DB2_Pin << 16;}while(0)//HAL_GPIO_WritePin(TFT_DB2_GPIO_Port,TFT_DB2_Pin,GPIO_PIN_RESET)

#define SET_D3_H()   do{TFT_DB3_GPIO_Port->BSRR |= TFT_DB3_Pin;}while(0)//HAL_GPIO_WritePin(TFT_DB3_GPIO_Port,TFT_DB3_Pin,GPIO_PIN_SET)
#define SET_D3_L()   do{TFT_DB3_GPIO_Port->BSRR |= (uint32_t)TFT_DB3_Pin << 16;}while(0)// HAL_GPIO_WritePin(TFT_DB3_GPIO_Port,TFT_DB3_Pin,GPIO_PIN_RESET)

#define SET_D4_H()   do{TFT_DB4_GPIO_Port->BSRR |= TFT_DB4_Pin;}while(0)//HAL_GPIO_WritePin(TFT_DB4_GPIO_Port,TFT_DB4_Pin,GPIO_PIN_SET)
#define SET_D4_L()   do{TFT_DB4_GPIO_Port->BSRR |= (uint32_t)TFT_DB4_Pin << 16;}while(0)///HAL_GPIO_WritePin(TFT_DB4_GPIO_Port,TFT_DB4_Pin,GPIO_PIN_RESET)

#define SET_D5_H()   do{TFT_DB5_GPIO_Port->BSRR |= TFT_DB5_Pin;}while(0)//HAL_GPIO_WritePin(TFT_DB5_GPIO_Port,TFT_DB5_Pin,GPIO_PIN_SET)
#define SET_D5_L()   do{TFT_DB5_GPIO_Port->BSRR |= (uint32_t)TFT_DB5_Pin << 16;}while(0)//HAL_GPIO_WritePin(TFT_DB5_GPIO_Port,TFT_DB5_Pin,GPIO_PIN_RESET)

#define SET_D6_H()   do{TFT_DB6_GPIO_Port->BSRR |= TFT_DB6_Pin;}while(0)//HAL_GPIO_WritePin(TFT_DB6_GPIO_Port,TFT_DB6_Pin,GPIO_PIN_SET)
#define SET_D6_L()   do{TFT_DB6_GPIO_Port->BSRR |= (uint32_t)TFT_DB6_Pin << 16;}while(0)///HAL_GPIO_WritePin(TFT_DB6_GPIO_Port,TFT_DB6_Pin,GPIO_PIN_RESET)

#define SET_D7_H()   do{TFT_DB7_GPIO_Port->BSRR |= TFT_DB7_Pin;}while(0)///HAL_GPIO_WritePin(TFT_DB7_GPIO_Port,TFT_DB7_Pin,GPIO_PIN_SET)
#define SET_D7_L()   do{TFT_DB7_GPIO_Port->BSRR |= (uint32_t)TFT_DB7_Pin << 16;}while(0)//HAL_GPIO_WritePin(TFT_DB7_GPIO_Port,TFT_DB7_Pin,GPIO_PIN_RESET)
/************************************************************************/
#endif 


//#define LCD_TOTAL_BUF_SIZE (240*320*2)
//#define LCD_Buf_Size 1152

uint8_t spi_tx_buffer[1];


static void TFT_Parallel_WriteByte(uint8_t txdata);

static void St7789v_Send_Byte(uint8_t chData);








/*******************************************************************************
 * 
 * Function Name: uint8_t TFT_Parallel_WriteByte(uint8_t *txdata,uint16_t size)
 * Function : to write data to TFT register
 * Input Ref: need write data, data of length
 * Return Ref: 0--success 1 - fail
 * 
***********************************************************************************/
static void TFT_Parallel_WriteByte(uint8_t txdata)
{
 
   
   St7789v_Send_Byte(txdata);

    
}
void LCD_GPIO_Reset(void)
{

    
	
	LCD_RST_SetLow();
    HAL_Delay(100);
    LCD_RST_SetHigh();
	HAL_Delay(100);

}
/*******************************************************************************
 * 
 * Function Name: static void LCD_Write_Cmd(uint8_t cmd)
 * Function : to write data to TFT register
 * Input Ref: write data 
 * Return Ref: NO
 * 
***********************************************************************************/
void LCD_Write_Cmd(uint8_t cmd)
{

	TFT_CS_GPIO_Port->BSRR |= TFT_CS_Pin ;//LCD_CS_SetHigh();
	TFT_CS_GPIO_Port ->BSRR |=(uint32_t)TFT_CS_Pin << 16 ;//LCD_CS_SetLow(); 
  
	TFT_RD_GPIO_Port->BSRR |= TFT_RD_Pin ;//LCD_RD_SetHigh();
	
	TFT_DCX_GPIO_Port->BSRR |= TFT_DCX_Pin ;//TFT_DCX_DATA();
	
    TFT_DCX_GPIO_Port ->BSRR|=(uint32_t)TFT_DCX_Pin << 16;   //TFT_DCX_CMD();//D/CX =0
 

    TFT_Parallel_WriteByte(cmd);


    TFT_WR_GPIO_Port ->BSRR |=(uint32_t)TFT_WR_Pin<<16;//LCD_WR_SetLow() ;

	TFT_WR_GPIO_Port ->BSRR |= TFT_WR_Pin ;//LCD_WR_SetHigh() ;

    TFT_CS_GPIO_Port ->BSRR |= TFT_CS_Pin ;//LCD_CS_SetHigh();



}

void LCD_Write_Data(uint8_t data)
{
	
	TFT_CS_GPIO_Port->BSRR |= TFT_CS_Pin ;//LCD_CS_SetHigh(); //To write data to TFT is high level
	TFT_CS_GPIO_Port ->BSRR |=(uint32_t)TFT_CS_Pin << 16 ;//LCD_CS_SetLow();

	TFT_RD_GPIO_Port->BSRR |= TFT_RD_Pin ;//LCD_RD_SetHigh();
	TFT_DCX_GPIO_Port ->BSRR|=(uint32_t)TFT_DCX_Pin << 16;//TFT_DCX_CMD();//D/CX =0

	TFT_DCX_GPIO_Port->BSRR |= TFT_DCX_Pin ;//TFT_DCX_DATA();


	TFT_Parallel_WriteByte(data);

    TFT_WR_GPIO_Port ->BSRR |=(uint32_t)TFT_WR_Pin<<16;//LCD_WR_SetLow() ;

	TFT_WR_GPIO_Port ->BSRR |= TFT_WR_Pin ;//LCD_WR_SetHigh() ;

	TFT_CS_GPIO_Port ->BSRR |= TFT_CS_Pin ;//LCD_CS_SetHigh();
		 
}

void LCD_Write_16bit_Data(uint16_t data)
{

    LCD_Write_Data(data >> 8);
    LCD_Write_Data(data);
    

}

/*******************************************************************************
 * 
 * Function Name: void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
 * Function : turn on TFT LCD back light
 * Input Ref: from x to y of point (x1,y1) to (x2,y2)
 * Return Ref: NO
 * 
***********************************************************************************/
void LCD_Address_Set(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2)
{

   if(HORIZON ==1){
     /* 指定X方向操作区域 */
	   LCD_Write_Cmd(0x2a); //display column
	   LCD_Write_Data(x1 >> 8);
	   LCD_Write_Data(x1);
	   LCD_Write_Data(x2 >> 8);
	   LCD_Write_Data(x2);
   
	   /* 指定Y方向操作区域 */
	   LCD_Write_Cmd(0x2b); //display row 
	   LCD_Write_Data(y1 >> 8);
	   LCD_Write_Data(y1);
	   LCD_Write_Data(y2 >> 8);
	   LCD_Write_Data(y2);
   
	   /* 发送该命令，LCD开始等待接收显存数据 */
	   LCD_Write_Cmd(0x2C);



   }
   else{
	/* 指定X方向操作区域 */
    LCD_Write_Cmd(0x2a); //display column
    LCD_Write_Data(x1 >> 8);
    LCD_Write_Data(x1);
    LCD_Write_Data(x2 >> 8);
    LCD_Write_Data(x2);

    /* 指定Y方向操作区域 */
    LCD_Write_Cmd(0x2b); //display row 
    LCD_Write_Data(y1 >> 8);
    LCD_Write_Data(y1);
    LCD_Write_Data(y2 >> 8);
    LCD_Write_Data(y2);

    /* 发送该命令，LCD开始等待接收显存数据 */
    LCD_Write_Cmd(0x2C);
   }

}

/*******************************************************************************
 * 
 * Function Name: static void LCD_Clear(uint16_t color)
 * Function : display TFT color
 * Input Ref: NO
 * Return Ref: NO
 * 
********************************************************************************/
void LCD_Clear(uint16_t color)
{

	uint16_t i, j;
    if(HORIZON == 0){
		LCD_Write_Cmd(0x2A);
		LCD_Write_Data(0);
		LCD_Write_Data(0);
		LCD_Write_Data(0);
		LCD_Write_Data(240);
		
		LCD_Write_Cmd(0X2B);
		LCD_Write_Data(0);
		LCD_Write_Data(0);
		LCD_Write_Data(0X01);
		LCD_Write_Data(0X40);
	
		LCD_Write_Cmd(0X2C);
	
		for (i = 0; i < 240; i++)
		{
			for (j = 0; j < 320; j++)
			{
				LCD_Write_Data(color >> 8);
				LCD_Write_Data(color);
			}
		}
		//lcd_display_on(); /* 开LCD显示 */
	   }
	   else{
           LCD_Write_Cmd(0x2A);
		   LCD_Write_Data(0);
		   LCD_Write_Data(0);
		   LCD_Write_Data(0x01);
		   LCD_Write_Data(0x3f); //320
		   
		   LCD_Write_Cmd(0X2B);
		   LCD_Write_Data(0);
		   LCD_Write_Data(0);
		   LCD_Write_Data(0);
		   LCD_Write_Data(0Xef); //240
	   
		   LCD_Write_Cmd(0X2C);
	   
		   for (i = 0; i < 320; i++)
		   {
			   for (j = 0; j < 240; j++)
			   {
				   LCD_Write_Data(color >> 8);
				   LCD_Write_Data(color);
			   }
		   }

		}

}



/*******************************************************************************
 * 
 * Function Name: void DISP_WINDOWS(void)
 * Function : display TFT color
 * Input Ref: NO
 * Return Ref: NO
 * 
********************************************************************************/
void DISP_WINDOWS(void)
{

         if(HORIZON ==1){
		   LCD_Write_Cmd(0x2A);
		   LCD_Write_Data(0);
		   LCD_Write_Data(0);
		   LCD_Write_Data(0x01);
		   LCD_Write_Data(0x3f); //320
		   
		   LCD_Write_Cmd(0X2B);
		   LCD_Write_Data(0);
		   LCD_Write_Data(0);
		   LCD_Write_Data(0);
		   LCD_Write_Data(0Xef); //240
	   
		   LCD_Write_Cmd(0X2C);
         }
		 else{
          
		 LCD_Write_Cmd(0x2A);
         LCD_Write_Data(0x00);
         LCD_Write_Data(0x00);
         LCD_Write_Data(0x00);
         LCD_Write_Data(0xEF);

         LCD_Write_Cmd(0x2B);
         LCD_Write_Data(0x00);
         LCD_Write_Data(0x00);
         LCD_Write_Data(0x01);
         LCD_Write_Data(0x3f);
         LCD_Write_Cmd(0x2C);
			 
		
        }
}


/*******************************************************************************
 * 
 * Function Name: static void LCD_Clear(uint16_t color)
 * Function : display TFT color
 * Input Ref: NO
 * Return Ref: NO
 * 
***********************************************************************************/
#if 1
void TFT_LCD_Init(void)
{
  /* 关闭睡眠模式 */
	LCD_GPIO_Reset();
	HAL_Delay(120);

	//TFT_BACKLIGHT_ON();
    LCD_Write_Cmd(0x11);
	HAL_Delay(120);              
 
    
    	
    LCD_Write_Cmd(0x36);//修改此处，可以改变屏幕的显示方向，横屏，竖屏等
    //LCD_Write_Data(0x00);  //竖屏
  //  LCD_Write_Data(0x60); //横屏
    LCD_Write_Data(0xA0); //横屏

 
   // LCD_Write_Cmd(0xa0);
    /* RGB 5-6-5-bit格式  */
    LCD_Write_Cmd(0x3A);
    LCD_Write_Data(0x55);//0x063
    // LCD_Clear(BLACK);
    /* porch 设置 */
    LCD_Write_Cmd(0xB2);
    LCD_Write_Data(0x0C);//< Back porch in normal mode
    LCD_Write_Data(0x0C);//< Front porch in normal mode
    LCD_Write_Data(0x00);//< Disable separate porch control
    LCD_Write_Data(0x33);//< Back and front porch in idle mode
    LCD_Write_Data(0x33);//< Back and front porch in partial mode

 LCD_Write_Cmd(0xB7);     
 LCD_Write_Data(0x56);   

 LCD_Write_Cmd(0xBB);     
 LCD_Write_Data(0x20);   

 LCD_Write_Cmd(0xC0);     
 LCD_Write_Data(0x2C);   

 LCD_Write_Cmd(0xC2);     
 LCD_Write_Data(0x01);   

 LCD_Write_Cmd(0xC3);     
 LCD_Write_Data(0x0F);   

 LCD_Write_Cmd(0xC4);     
 LCD_Write_Data(0x20); 

 LCD_Write_Cmd(0xC6);     
 LCD_Write_Data(0x0F);   
 
 LCD_Write_Cmd(0xD0);     
 LCD_Write_Data(0xA4);   
 LCD_Write_Data(0xA1);   

 LCD_Write_Cmd(0xD6);     
LCD_Write_Data(0xA1);   

 LCD_Write_Cmd(0xE0);
 LCD_Write_Data(0xF0);
 LCD_Write_Data(0x00);
 LCD_Write_Data(0x06);
 LCD_Write_Data(0x06);
LCD_Write_Data(0x05);
 LCD_Write_Data(0x30);
 LCD_Write_Data(0x44);
 LCD_Write_Data(0x48);
 LCD_Write_Data(0x38);
 LCD_Write_Data(0x11);
 LCD_Write_Data(0x10);
 LCD_Write_Data(0x34);

 LCD_Write_Cmd(0xE1);
 LCD_Write_Data(0xF0);
 LCD_Write_Data(0x0A);
 LCD_Write_Data(0x0E);
 LCD_Write_Data(0x0D);
 LCD_Write_Data(0x0B);
 LCD_Write_Data(0x27);
 LCD_Write_Data(0x2F);
 LCD_Write_Data(0x44);
 LCD_Write_Data(0x47);
 LCD_Write_Data(0x35);
 LCD_Write_Data(0x12);
 LCD_Write_Data(0x12);
 LCD_Write_Data(0x2C);
 LCD_Write_Data(0x32);

  LCD_Write_Cmd(0x35);     
  LCD_Write_Data(0x00);

 LCD_Write_Cmd(0x21);     

 LCD_Write_Cmd(0x29);    

 LCD_Write_Cmd(0x2C);  




}
#else
void TFT_LCD_Init(void)
{
    /* 关闭睡眠模式 */
	LCD_GPIO_Reset();
	HAL_Delay(120);

	//TFT_BACKLIGHT_ON();
    LCD_Write_Cmd(0x11);
	HAL_Delay(120);
  
    /* 开始设置显存扫描模式，数据格式等 */
	
    LCD_Write_Cmd(0x36);//修改此处，可以改变屏幕的显示方向，横屏，竖屏等
    //LCD_Write_Data(0x00);  //竖屏
  //  LCD_Write_Data(0x60); //横屏
    LCD_Write_Data(0xA0); //横屏

 
    LCD_Write_Cmd(0xa0);
    /* RGB 5-6-5-bit格式  */
    LCD_Write_Cmd(0x3A);
    LCD_Write_Data(0x65);
    /* porch 设置 */
    LCD_Write_Cmd(0xB2);
    LCD_Write_Data(0x0C);//< Back porch in normal mode
    LCD_Write_Data(0x0C);//< Front porch in normal mode
    LCD_Write_Data(0x00);//< Disable separate porch control
    LCD_Write_Data(0x33);//< Back and front porch in idle mode
    LCD_Write_Data(0x33);//< Back and front porch in partial mode
    /* VGH设置 */
    LCD_Write_Cmd(0xB7);
    LCD_Write_Data(0x72);
    /* VCOM 设置 */
    LCD_Write_Cmd(0xBB);
    LCD_Write_Data(0x3D);
    /* LCM 设置 */
    LCD_Write_Cmd(0xC0);
    LCD_Write_Data(0x2C);
    /* VDV and VRH 设置 */
    LCD_Write_Cmd(0xC2);
    LCD_Write_Data(0x01);
    /* VRH 设置 */
    LCD_Write_Cmd(0xC3);
    LCD_Write_Data(0x19);
    /* VDV 设置 */
    LCD_Write_Cmd(0xC4);
    LCD_Write_Data(0x20);
    /* 普通模式下显存速率设置 60Mhz */
    LCD_Write_Cmd(0xC6);
    LCD_Write_Data(0x0F);
    /* 电源控制 */
    LCD_Write_Cmd(0xD0);
    LCD_Write_Data(0xA4);
    LCD_Write_Data(0xA1);
    /* 电压设置 */
    LCD_Write_Cmd(0xE0);
    LCD_Write_Data(0xD0);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x11);
    LCD_Write_Data(0x13);
    LCD_Write_Data(0x2B);
    LCD_Write_Data(0x3F);
    LCD_Write_Data(0x54);
    LCD_Write_Data(0x4C);
    LCD_Write_Data(0x18);
    LCD_Write_Data(0x0D);
    LCD_Write_Data(0x0B);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x23);
    /* 电压设置 */
    LCD_Write_Cmd(0xE1);
    LCD_Write_Data(0xD0);
    LCD_Write_Data(0x04);
    LCD_Write_Data(0x0C);
    LCD_Write_Data(0x11);
    LCD_Write_Data(0x13);
    LCD_Write_Data(0x2C);
    LCD_Write_Data(0x3F);
    LCD_Write_Data(0x44);
    LCD_Write_Data(0x51);
    LCD_Write_Data(0x2F);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x1F);
    LCD_Write_Data(0x20);
    LCD_Write_Data(0x23);


//	 LCD_Write_Cmd(0x2A);
//     LCD_Write_Data(0x00);
//     LCD_Write_Data(0x00);
//     LCD_Write_Data(0x01);
//     LCD_Write_Data(0x3f);
//
//     LCD_Write_Cmd(0x2B);
//     LCD_Write_Data(0x00);
//     LCD_Write_Data(0x00);
//     LCD_Write_Data(0x00);
//     LCD_Write_Data(0xef);


	
    /* 显示开 */
	LCD_Write_Cmd(0x21); // Display Inversion On
	//LCD_Write_Cmd(0x20); // Display Inversion Off
    LCD_Write_Cmd(0x29); // display on 
    //LCD_Write_Cmd(0x28);  // display off ---WT.EDIT  

     LCD_Write_Cmd(0x2C);  

    /* 设置背景色 */
    LCD_Clear(BLACK);
   // LCD_Clear(WHITE);
	

    /*打开显示*/
 
}
#endif 

static void St7789v_Send_Byte(uint8_t chData)
{
   if((chData>>0 & 0x01)){
	 	SET_D0_H();
	}
	else
	    SET_D0_L();
	
	if((chData>>1 & 0x01)){
		SET_D1_H();
	}
	else
		SET_D1_L();
	
	if(chData>>2 & 0x01){	
		SET_D2_H();
	}
	else 
	   SET_D2_L();

	if(chData>>3 & 0x01){ 	
		SET_D3_H();
	}
	else
	   SET_D3_L();

	if(chData>>4 & 0x01){
		SET_D4_H();
	}
	else
	  SET_D4_L();
	
    if(chData>>5 & 0x01){
		SET_D5_H();
    }
	else
	  SET_D5_L();

	if(chData>>6 & 0x01){
		SET_D6_H();
	}
	else
	   SET_D6_L();
	
	if(chData>>7 & 0x01){
		SET_D7_H();
	}
	else
	   SET_D7_L();
	
}


