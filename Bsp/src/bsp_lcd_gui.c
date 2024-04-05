#include "bsp_lcd_gui.h"
#include "bsp.h"
#include "bsp_tft_font.h"

uint16_t BACK_COLOR=BLACK;
uint16_t  POINT_COLOR=WHITE;


static void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);




/***********************************************************************
 *
 * Function Name: void TFT_Display_Handler(void)
 * Function: TFT LCD 显示全部的内容
 * Inpurt Ref: NO           
 * Return Ref: NO 
 *
************************************************************************/
void TFT_Display_Handler(void)
{

    
    TFT_Display_Temp_Symbol();

	TFT_Display_Humidity_Symbol();
	
	//lcd_draw_rectangle(157,35,163,110,WHITE);  //display veritcal "I"
	//TFT_St7789_FillBlock(157,35,6,75,WHITE);

   //temperature value 
   TFT_Disp_Temp_Value(0,gctl_t.dht11_temp_value);
   //huimidity value

   TFT_Disp_Humidity_Value(gctl_t.dht11_hum_value);

   TFT_Display_WorksTime();
 


}

/***********************************************************************************************
	*
	*Function Name:void TFT_Display_Temp_Symbol(void)
	*Function : special TFT of itme
	*Input: NO
	*Return: NO 
	*
***********************************************************************************************/
void TFT_Display_Temp_Symbol(void)
{
   //TFT_Disp_Temp_Symbol_24_24(130,40); //temp symbol 
   TFT_Disp_Temp_Symbol_24_24(123,40); //temp symbol “°C”
#if TFT_DISP_TEMP_24
	TFT_Disp_Temp_24_24_onBlack(122,10,2); //temp symbol 

	TFT_Disp_Temp_24_24_onBlack(102,100,0);
	TFT_Disp_Temp_24_24_onBlack(126,100,1);
#else
	 TFT_Disp_Temp_20_20(115,90,0); //(111,90,0);
	 TFT_Disp_Temp_20_20(135,90,1); //(131,90,1)
#endif 
}

/***********************************************************************************************
	*
	*Function Name:void TFT_Display_Humidity_Symbol(void)
	*Function : menu of humidity chines words
	*Input: NO
	*Return: NO 
	*
***********************************************************************************************/
void TFT_Display_Humidity_Symbol(void)
{

    //TFT_Disp_Humidity_Symbol_24_24(292,40);//(286,40);
    TFT_Disp_Humidity_Symbol_24_24(285,40);//(286,40);//humidity symbol %
    
#if TFT_DISP_TEMP_24 
	TFT_Disp_Humidity_24_24_onBlack(286,10,2); //humidity symbol %

    TFT_Disp_Humidity_24_24_onBlack(268,100,0);
    TFT_Disp_Humidity_24_24_onBlack(292,100,1);
#else
    TFT_Disp_Humidity_20_20(278,90,0);//(275,90,0)
	TFT_Disp_Humidity_20_20(298,90,1);//(295,90,1)

#endif 


}
/***********************************************************************************************
	*
	*Function Name:void TFT_Display_WorksTime(void)
	*Function : display of works time value 
	*Input: NO
	*Return: NO 
	*
***********************************************************************************************/
void TFT_Display_WorksTime(void)
{

    static uint8_t temp_decade_hours,temp_unit_hours,temp_decade_minutes,temp_unit_minutes;
	static uint8_t disp_work_time=0xff;

    if(gctl_t.gTimer_ctl_disp_works_time_second > 59){
        gctl_t.gTimer_ctl_disp_works_time_second =0;
        gctl_t.disp_works_minutes++;
        if(gctl_t.disp_works_minutes>59){
            gctl_t.disp_works_minutes=0;
            gctl_t.disp_works_hours++;
            if(gctl_t.disp_works_hours > 99){
                gctl_t.disp_works_hours=0;
            }
        }
    }
	temp_decade_hours = gctl_t.disp_works_hours /10;
	temp_unit_hours = gctl_t.disp_works_hours % 10;

	temp_decade_minutes = gctl_t.disp_works_minutes/10;
	temp_unit_minutes = gctl_t.disp_works_minutes%10;

	//gctl_t.timer_timing_words_changed_flag++ ; //表示要进入切换状态

    //display works of words of chines
    if(pro_t.timer_mode_flag ==timer_time || pro_t.timer_mode_flag==timer_set_time ){
                //do something 
    }
    else if(disp_work_time != gctl_t.timing_words_changed_flag){
		disp_work_time = gctl_t.timing_words_changed_flag;
	    TFT_Disp_WorksTime_24_24_onBlack(112,150,0,0);//works one "工"
		TFT_Disp_WorksTime_24_24_onBlack(136,150,0,1);//works tow "作"
		TFT_Disp_WorksTime_24_24_onBlack(160,150,0,2);//"时"
		TFT_Disp_WorksTime_24_24_onBlack(184,150,0,3);//“间”

    }
	
	//works time value
	
   	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(100,188,0,temp_decade_hours);
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(134,188,0,temp_unit_hours);
    //symbol colon
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(186,188,0,temp_decade_minutes);
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(220,188,0,temp_unit_minutes);

	if(gctl_t.gTimer_ctl_total_continue_time > 119 && gctl_t.gTimer_ctl_total_continue_time < 131){

	      gctl_t.time_out_flag =1;

	}
	else{

		gctl_t.time_out_flag =0;

	}

}
/***********************************************************************************************
	*
	*Function Name:void TFT_Only_Disp_Timing(void)
	*Function : display of works time value 
	*Input: NO
	*Return: NO 
	*
***********************************************************************************************/
void TFT_Only_Disp_Timing(void)
{

static uint8_t temp_decade_hours,temp_unit_hours,temp_decade_minutes,temp_unit_minutes;
	
	    temp_decade_hours = gctl_t.disp_works_hours /10;
		temp_unit_hours = gctl_t.disp_works_hours % 10;

		temp_decade_minutes = gctl_t.disp_works_minutes/10;
		temp_unit_minutes = gctl_t.disp_works_minutes%10;


	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(100,188,0,temp_decade_hours);
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(134,188,0,temp_unit_hours);
    //symbol colon
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(186,188,0,temp_decade_minutes);
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(220,188,0,temp_unit_minutes);




}

void TFT_Only_Disp_Set_Timer_Blink(void)
{

static uint8_t temp_decade_hours,temp_unit_hours,temp_decade_minutes,temp_unit_minutes;
	
	    temp_decade_hours = gctl_t.disp_works_hours /10;
		temp_unit_hours = gctl_t.disp_works_hours % 10;

		temp_decade_minutes = gctl_t.disp_works_minutes/10;
		temp_unit_minutes = gctl_t.disp_works_minutes%10;


	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(100,188,1,temp_decade_hours);
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(134,188,1,temp_unit_hours);
    //symbol colon
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(186,188,1,temp_decade_minutes);
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(220,188,1,temp_unit_minutes);




}

void TFT_Disp_Onley_Set_TimerTime_Value(void)
{

   static uint8_t timer_decade_hours,timer_unit_hours,timer_decade_minutes,timer_unit_minutes;
   static uint8_t set_timer_hours=0xff,disp_timer_words=0xff;
  

  

    timer_decade_hours = gctl_t.gSet_timer_hours /10;
	timer_unit_hours = gctl_t.gSet_timer_hours % 10;


   
	    
		TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(100,188,0,timer_decade_hours);
		TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(134,188,0,timer_unit_hours);

    

	
        timer_decade_minutes= gctl_t.gSet_timer_minutes / 10;
	    timer_unit_minutes = gctl_t.gSet_timer_minutes % 10;
        TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(186,188,0,timer_decade_minutes);
        TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(220,188,0,timer_unit_minutes);
    
    


  }

/***********************************************************************************************
	*
	*Function Name:void TFT_Display_WorksTime(void)
	*Function : display of works time value 
	*Input: NO
	*Return: NO 
	*
***********************************************************************************************/
void TFT_DonnotDisp_Works_Time(void)
{

   
	if(gctl_t.gTimer_ctl_disp_works_time_second > 59){
		   gctl_t.gTimer_ctl_disp_works_time_second =0;
		   gctl_t.disp_works_minutes++;
		   if(gctl_t.disp_works_minutes>59){
			   gctl_t.disp_works_minutes=0;
			   gctl_t.disp_works_hours++;
			   if(gctl_t.disp_works_hours > 99){
				   gctl_t.disp_works_hours=0;
			   }
		   }
	 }
   
	if(gctl_t.gTimer_ctl_total_continue_time > 119 && gctl_t.gTimer_ctl_total_continue_time < 131){

	      gctl_t.time_out_flag =1;

	}
	else{

		gctl_t.time_out_flag =0;

	}

}

/********************************************************************************
 * 
 * Function Name: static void TFT_Disp_Set_TimerTime(void)
 * Function: set timer time of TFT of numbers blink.
 * Input Ref: bc : 1-don't display numbers, 0-display numbers
 * Return Ref:
 * 
*********************************************************************************/
void Power_Off_Retain_Beijing_Time(void)
{

   if(wifi_link_net_state()==1){
	if(gctl_t.gTimer_ctl_disp_works_time_second > 59){
		   gctl_t.gTimer_ctl_disp_works_time_second =0;
		   gctl_t.disp_works_minutes++;
		   if(gctl_t.disp_works_minutes>59){
			   gctl_t.disp_works_minutes=0;
			   gctl_t.disp_works_hours++;
			   if(gctl_t.disp_works_hours > 99){
				   gctl_t.disp_works_hours=0;
			   }
		   }
	 }
   }
   

}

/********************************************************************************
 * 
 * Function Name: static void TFT_Disp_Set_TimerTime(void)
 * Function: set timer time of TFT of numbers blink.
 * Input Ref: bc : 1-don't display numbers, 0-display numbers
 * Return Ref:
 * 
*********************************************************************************/
void TFT_Disp_Set_TimerTime(uint8_t bc)
{

   static uint8_t timer_decade_hours,timer_unit_hours,timer_decade_minutes,timer_unit_minutes;
   static uint8_t set_timer_hours=0xff,set_timer_minutes = 0xff,disp_timer_words=0xff,dis_hours=0xff,dis_minutes=0xff;

    timer_decade_hours = gctl_t.gSet_timer_hours /10;
	timer_unit_hours = gctl_t.gSet_timer_hours % 10;

	timer_decade_minutes= gctl_t.gSet_timer_minutes / 10;
	timer_unit_minutes = gctl_t.gSet_timer_minutes % 10;

    //display works of words of chinese 
    if(disp_timer_words != gctl_t.timer_timing_words_changed_flag){
		disp_timer_words = gctl_t.timer_timing_words_changed_flag;
    TFT_Disp_WorksTime_24_24_onBlack(112,150,1,0);//works one "定"
	TFT_Disp_WorksTime_24_24_onBlack(136,150,1,1);//"时"
	TFT_Disp_WorksTime_24_24_onBlack(160,150,1,2);//“时”
	TFT_Disp_WorksTime_24_24_onBlack(184,150,1,3);//“间”

    }
	
	//works time value
	if(set_timer_hours != gctl_t.gSet_timer_hours ||(dis_hours != gctl_t.timer_timing_words_changed_flag) ){
		set_timer_hours = gctl_t.gSet_timer_hours;
		dis_hours = gctl_t.timer_timing_words_changed_flag;
	    
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(100,188,bc,timer_decade_hours);
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(134,188,bc,timer_unit_hours);
//	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(160,180,10); //时间分割符号

	
	}

	if((set_timer_minutes != gctl_t.gSet_timer_minutes)|| (dis_minutes != gctl_t.timer_timing_words_changed_flag)){
		set_timer_minutes = gctl_t.gSet_timer_minutes;
		dis_minutes = gctl_t.timer_timing_words_changed_flag;

	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(186,188,bc,timer_decade_minutes);
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(220,188,bc,timer_unit_minutes);

	}
	
  }
/****************************************************************************************************
	*
	*Function Name:void TFT_Disp_Set_TimerTime_Init(void)
	*Function: display set up timer timing value how meny value 
	*Input Ref:NO
	*Return Ref:NO
	*
****************************************************************************************************/
void TFT_Disp_Set_TimerTime_Init(void)
{

   static uint8_t timer_decade_hours,timer_unit_hours,timer_decade_minutes,timer_unit_minutes;
   static uint8_t set_timer_hours=0xff,disp_timer_words=0xff;
   static uint8_t bc;

   bc = 0;

    timer_decade_hours = gctl_t.gSet_timer_hours /10;
	timer_unit_hours = gctl_t.gSet_timer_hours % 10;


    if(disp_timer_words != gctl_t.timer_timing_words_changed_flag){
		disp_timer_words = gctl_t.timer_timing_words_changed_flag;
    //display works of words of chinese 
    TFT_Disp_WorksTime_24_24_onBlack(112,150,1,0);//works one "定"
	TFT_Disp_WorksTime_24_24_onBlack(136,150,1,1);//"时"
	TFT_Disp_WorksTime_24_24_onBlack(160,150,1,2);//“时”
	TFT_Disp_WorksTime_24_24_onBlack(184,150,1,3);//“间”

    }
	
	//works time value
	if(set_timer_hours != gctl_t.gSet_timer_hours){
		set_timer_hours = gctl_t.gSet_timer_hours;
	    
		TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(100,188,bc,timer_decade_hours);
		TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(134,188,bc,timer_unit_hours);

     }

	if(gctl_t.timer_time_define_flag == 1){
        timer_decade_minutes= gctl_t.gSet_timer_minutes / 10;
	    timer_unit_minutes = gctl_t.gSet_timer_minutes % 10;
        TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(186,188,bc,timer_decade_minutes);
        TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(220,188,bc,timer_unit_minutes);
    }
    else{
	    timer_decade_minutes=0;
	    timer_unit_minutes =0;
        TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(186,188,bc,timer_decade_minutes);
        TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(220,188,bc,timer_unit_minutes);
    }


  }
void TFT_Disp_Voice_Set_TimerTime_Init(void)
{

   static uint8_t timer_decade_hours,timer_unit_hours,timer_decade_minutes,timer_unit_minutes;
   static uint8_t set_timer_hours=0xff,disp_timer_words=0xff;
   static uint8_t bc;

   bc = 0;

    timer_decade_hours = gctl_t.gSet_timer_hours /10;
	timer_unit_hours = gctl_t.gSet_timer_hours % 10;

	
	    timer_decade_minutes=0;
	    timer_unit_minutes =0;


    if(disp_timer_words != gctl_t.timer_timing_words_changed_flag){
		disp_timer_words = gctl_t.timer_timing_words_changed_flag;
    //display works of words of chinese 
    TFT_Disp_WorksTime_24_24_onBlack(112,150,1,0);//works one "定"
	TFT_Disp_WorksTime_24_24_onBlack(136,150,1,1);//"时"
	TFT_Disp_WorksTime_24_24_onBlack(160,150,1,2);//“时”
	TFT_Disp_WorksTime_24_24_onBlack(184,150,1,3);//“间”

    }
	

	    
		TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(100,188,bc,timer_decade_hours);
		TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(134,188,bc,timer_unit_hours);


    
        TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(186,188,bc,timer_decade_minutes);
        TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(220,188,bc,timer_unit_minutes);
    


  }


/********************************************************************************
 * 
 * Function Name: void TFT_Disp_Timer_Split_Symbol(void)
 * Function: display timer timing of split symbol
 * Input Ref: display timer ":" symbol 
 * Return Ref:
 * 
*********************************************************************************/
void TFT_Disp_Timer_Split_Symbol(void)
{
    if(pro_t.gPower_On==power_on){  

	     #if 0
          if(  pro_t.gTimer_pro_time_split_symbol > 1 && pro_t.gTimer_pro_time_split_symbol< 3){
             
              TFT_Disp_Time_Split_Symbol(160,173,0); //时间分割符号,turn on
		  }
		  else if(pro_t.gTimer_pro_time_split_symbol >3 ||  pro_t.gTimer_pro_time_split_symbol ==3){
                pro_t.gTimer_pro_time_split_symbol=0;
		        TFT_Disp_Time_Split_Symbol(160,173,1); //时间分割符号 turn off

		  }
		 #endif 

		 if(  pro_t.gTimer_pro_time_split_symbol > 1 && pro_t.gTimer_pro_time_split_symbol< 3){
             
              TFT_Disp_Time_Split_Symbol(160,173,0); //时间分割符号,turn on
              TFT_Display_WorksTime();
		 }
		 else if(pro_t.gTimer_pro_time_split_symbol >3 ){
                pro_t.gTimer_pro_time_split_symbol=0;
		        TFT_Disp_Time_Split_Symbol(160,173,1); //时间分割符号 turn off
		        TFT_Disp_Temp_Value(0,gctl_t.dht11_temp_value);
                TFT_Disp_Humidity_Value(gctl_t.dht11_hum_value);
		        

		 }
		 

     }
}

/***********************************************************************************************
	*
	*Function Name:void TFT_Disp_Temp_Value(uint8_t temp_value)
	*Function : display dht11 of sensor temperature value 
	*Input: bc= backgroud color,bc=0 ->display numbers ,bc= 1 ->don't display numbers 
	*Return: NO 
	*
***********************************************************************************************/
void TFT_Disp_Temp_Value(uint8_t bc,uint8_t temp_value)
{

  static uint8_t temp_unit,temp_decade,refresh_one=0xff,refresh_two=0xff;

   temp_decade = temp_value /10;

   temp_unit= temp_value%10; 
  
   if(refresh_one != temp_decade || v_t.voice_set_temperature_value_flag==2){
   	refresh_one = temp_decade;
	//__disable_irq();
   	TFT_Disp_Numbers_Pic_413(5,40,bc,temp_decade); //间隔58
   //	__enable_irq();

   }

   if(refresh_two != temp_unit || v_t.voice_set_temperature_value_flag==2){
   	  refresh_two = temp_unit;
	   v_t.voice_set_temperature_value_flag++;
   //__disable_irq();
   TFT_Disp_Numbers_Pic_413(63,40,bc,temp_unit);

   //	__enable_irq();
   }

}

void TFT_Disp_Voice_Temp_Value(uint8_t bc,uint8_t temp_value)
{

   static uint8_t temp_unit,temp_decade,refresh_one=0xff,refresh_two=0xff;

   temp_decade = temp_value /10;

   temp_unit= temp_value%10; 
  
   
   	TFT_Disp_Numbers_Pic_413(5,40,bc,temp_decade); //间隔58

 


   TFT_Disp_Numbers_Pic_413(63,40,bc,temp_unit);
   


}

/***********************************************************************************************
	*
	*Function Name:void TFT_Disp_Humidity_Value(uint8_t hum_value)
	*Function : display dht11 of sensor humidity value 
	*Input: NO
	*Return: NO 
	*
***********************************************************************************************/
void TFT_Disp_Humidity_Value(uint8_t hum_value)
{

   static uint8_t hum_unit,hum_decade;
   static uint8_t hum_default_one = 0xff,hum_default_two= 0xff;

   hum_decade = hum_value /10;

   hum_unit = hum_value%10;

   if(hum_default_one != hum_decade){

   hum_default_one = hum_decade;

   TFT_Disp_Numbers_Pic_413(168,40,0,hum_decade); //间隔58
  

   }

   if(hum_default_two != hum_unit){

   hum_default_two = hum_unit;
   TFT_Disp_Numbers_Pic_413(226,40,0, hum_unit);

   }

}
/**************************************************************************
*
* Function Name:void TFT_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
* Function : 画点
* Input Ref:x，y:起始坐标  color:颜色
* Return Ref: NO
*
***********************************************************************/
void TFT_DrawPoint(uint16_t x,uint16_t y,uint16_t color)
{
   LCD_Write_Cmd(0x2a);
   LCD_Write_Data(x >>8);
   LCD_Write_Data(x);

   LCD_Write_Cmd(0x2b);
   LCD_Write_Data(y >>8);
   LCD_Write_Data(y);

   LCD_Write_Cmd(0x2c);
   LCD_Write_Data(color >>8);
   LCD_Write_Data(color);
}
/**************************************************************************
*
* Function Name:void TFT_SetWindow(uint32_t xstart,uint32_t ystart,uint32_t xend,uint32_t yend)
* Function : windows set 
* Input Ref:xtart，ystart:起始坐标  终点坐标：xend,yend
* Return Ref: NO
*
***********************************************************************/
void TFT_SetWindow(uint16_t xstart,uint16_t ystart,uint16_t xend,uint16_t yend)
{
   LCD_Write_Cmd(0x2a);
    LCD_Write_Data(xstart >>8);
   LCD_Write_Data(xstart);                      //Column start

    LCD_Write_Data(xend >>8);
   LCD_Write_Data(xend);

   LCD_Write_Cmd(0x2b);
    LCD_Write_Data(ystart >>8);
   LCD_Write_Data(ystart);
   
     LCD_Write_Data(yend >>8);
   LCD_Write_Data(yend);                     //Row start

   LCD_Write_Cmd(0x2c);


}
/**************************************************************************
*
* Function Name:void TFT_St7789_SetBGColor(uint32_t color)
* Function : set background color
* Input Ref: color
* Return Ref: NO
*
***********************************************************************/
void TFT_St7789_SetBGColor(uint32_t color)
{
   uint32_t i,j;
   TFT_SetWindow(0,0,(LCD_Width-1),(LCD_Height -1));
   for(i=0;i<LCD_Height;i++){
       for(j=0;j<LCD_Width;j++){
           LCD_Write_16bit_Data(color);
       }
   }


}
/**************************************************************************
*
* Function Name:void TFT_St7789_FillBlock(uint32_t xstart,uint32_t ystart,uint32_t block_width,uint32_t block_height,uint32_t colo)
* Function : set background color
* Input Ref: color
* Return Ref: NO
*
***********************************************************************/
void TFT_St7789_FillBlock(uint16_t xstart,uint16_t ystart,uint16_t block_width,uint16_t block_height,uint16_t color)
{
    uint32_t i,j;
    TFT_SetWindow(xstart,ystart,(xstart+block_width-1),(ystart+block_height-1));
    for(i=0;i<block_width;i++){
        for(j=0;j<block_height;j++){
            LCD_Write_Data(color >>8);
			LCD_Write_Data(color );
        }
    }
}

/*********************************************************************
 * 
 * Function Name:void TFT_ShowText(uitn16_t x,uint16_t y,char *str,uint8_t fw,uint8_t fh,uint8_t mode)
 * Function : TFT display chinese of word 
 * Input Ref: x,y start position, *str of input chinese word,fw ,fh -> tft of sieze
 *            mode: 0 normal . 1->叠加
 * 
*********************************************************************/
void TFT_ShowText(uint16_t x,uint16_t y,char *str,uint8_t fw,uint8_t fh,uint8_t mode)
{
    while(*str!=0)
    {
       // TFT_ShowFont(x,y,str,fw,fh,mode); //显示这个汉字,空心显示
        str+=2;
        x+=fh;//下一个汉字偏移
    }
}
/*******************************************************************************************
	 *
	 * @brief       画线
	 * @param       x1,y1: 起始坐标
	 * @param       x2,y2: 终点坐标
	 * @param       color: 线的颜色
	 * @retval      无
	 *
 *******************************************************************************************/
static void lcd_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint16_t t;
    int xerr = 0, yerr = 0, delta_x, delta_y, distance;
    int incx, incy, row, col;
    delta_x = x2 - x1;          /* 计算坐标增量 */
    delta_y = y2 - y1;
    row = x1;
    col = y1;

    if (delta_x > 0)incx = 1;   /* 设置单布方向 */
    else if (delta_x == 0)incx = 0; /* 垂直线 */
    else
    {
        incx = -1;
        delta_x = -delta_x;
    }

    if (delta_y > 0)incy = 1;
    else if (delta_y == 0)incy = 0; /* 水平线*/
    else
    {
        incy = -1;
        delta_y = -delta_y;
    }

    if ( delta_x > delta_y)distance = delta_x;  /* 选取基本增量坐标轴 */
    else distance = delta_y;

    for (t = 0; t <= distance + 1; t++ )   /* 画线输出 */
    {
        //lcd_draw_point(row, col, color); /* 画点 */
        TFT_DrawPoint(row,col,color);
        xerr += delta_x ;
        yerr += delta_y ;

        if (xerr > distance)
        {
            xerr -= distance;
            row += incx;
        }

        if (yerr > distance)
        {
            yerr -= distance;
            col += incy;
        }
    }
}

/*************************************************************************************************
 * @brief       画矩形
 * @param       x1,y1: start
 * @param       x2,y2: end 
 * @param       color: rectangle is color
 * @retval      ÎÞ
 *************************************************************************************************/
void lcd_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    lcd_draw_line(x1, y1, x2, y1, color);
    lcd_draw_line(x1, y1, x1, y2, color);
    lcd_draw_line(x1, y2, x2, y2, color);
    lcd_draw_line(x2, y1, x2, y2, color);
}




