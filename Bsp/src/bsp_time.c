#include "bsp.h"


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
	

    if(gctl_t.gTimer_ctl_disp_works_time_second > 59 ){
		   gctl_t.gTimer_ctl_disp_works_time_second =0;
            gctl_t.power_on_dis_work_time = 2;
		   gctl_t.disp_works_minutes++;
		   if(gctl_t.disp_works_minutes>59){
			   gctl_t.disp_works_minutes=0;
			   gctl_t.disp_works_hours++;


               if(wifi_link_net_state()==0){
    			   if(gctl_t.disp_works_hours > 99){
    				   gctl_t.disp_works_hours=0;
    			   }
               }
               else{

               if(gctl_t.disp_works_hours > 23){
                         gctl_t.disp_works_hours=0;
                      }


               }
		   }
	
	


	temp_decade_hours = gctl_t.disp_works_hours /10;
	temp_unit_hours = gctl_t.disp_works_hours % 10;

	temp_decade_minutes = gctl_t.disp_works_minutes/10;
	temp_unit_minutes = gctl_t.disp_works_minutes%10;

	

//   if(disp_works !=gpro_t.works_or_timer_disp_timing_flag){
//
//        disp_works =gpro_t.works_or_timer_disp_timing_flag;
   	
//	    TFT_Disp_WorksTime_24_24_onBlack(112,150,0,0);//works one "工"
//		TFT_Disp_WorksTime_24_24_onBlack(136,150,0,1);//works tow "作"
//		TFT_Disp_WorksTime_24_24_onBlack(160,150,0,2);//"时"
//		TFT_Disp_WorksTime_24_24_onBlack(184,150,0,3);//“间”
       // TFT_Disp_Chinese_WorksTime_96_24(100,150);
		if(wifi_link_net_state()==1 && gctl_t.get_beijing_time_success==1){

			TFT_Disp_Chinese_BeiJing_Time_23_23(TIMER_X0,TIMER_Y,0); //"北"
			TFT_Disp_Chinese_BeiJing_Time_23_23(TIMER_X1,TIMER_Y,1); // "京"
			TFT_Disp_Chinese_BeiJing_Time_23_23(TIMER_X2,TIMER_Y,2); // "时"
			TFT_Disp_Chinese_BeiJing_Time_23_23(TIMER_X3,TIMER_Y,3); // “间”

		}
		else{
			TFT_Disp_Chinese_WorkTime_23_23(TIMER_X0,TIMER_Y,0);
			TFT_Disp_Chinese_WorkTime_23_23(TIMER_X1,TIMER_Y,1);
			TFT_Disp_Chinese_WorkTime_23_23(TIMER_X2,TIMER_Y,2);
			TFT_Disp_Chinese_WorkTime_23_23(TIMER_X3,TIMER_Y,3);
			if(wifi_link_net_state()==0)gctl_t.get_beijing_time_success=0;

		}
	 

  //  }
	
	//works time value
	
//   	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(100,188,0,temp_decade_hours);
//	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(134,188,0,temp_unit_hours);

		
   	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(80,190,0,temp_decade_hours);
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(114,190,0,temp_unit_hours);

	

    //symbol colon
	//TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(186,188,0,temp_decade_minutes);
	//TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(220,188,0,temp_unit_minutes);

	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(166,190,0,temp_decade_minutes);
	TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(200,190,0,temp_unit_minutes);

    }

}


/****************************************************************************************
	*
	*Function  Name :void TFT_Disp_Temp_24_24_onBlack(uint16_t x,uint16_t y,uint8_t num)
	*Function:
	*Input Ref:NO
	*Return Ref:NO
	*
****************************************************************************************/
void TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(uint16_t x,uint16_t y,uint8_t sel,uint8_t num)
{
    
	  
		uint8_t temp, t,tbit;
		uint8_t x0=x;
	   
		static uint16_t color;
		
	
		//for(t = 0; t < 144; t++)	/*遍历打印所有像素点到LCD */
		for(t = 0; t < 116; t++)
		{	
		
			//temp = font4848_works_time_value[num][t]; 
			temp = font_pic_time_value[num][t];
			
			
			for(tbit = 0; tbit < 8; tbit++) /* 打印一个像素点到液晶 */
			{	
				
				if(temp & 0x80){ //display of backgroud color is "black" picture of words is "black"->0x00, background is "white" ->0xff
					
					color =BLACK; // WHITE;
	
				}
				else if(sel==0){ //display of words of color is "white"
					
					  color = WHITE;//BLACK;
					
               }
			   else color = BLACK; //don't display words ,all color is backgroud black.
				
				TFT_DrawPoint(x, y,color);
				
				temp <<= 1; 		
				//y++; // 垂直扫描
				x++;//水平扫描
	
				if(x >= LCD_Width){
						gpro_t.lcd_over_width_flag =1;
						return; /* 超区域了 */
	
				}
				
				if((x - x0) == 26){//
					x = x0;
					y++;
					
					if(y >= LCD_Height){
					gpro_t.lcd_over_height_flag=1;
					return; 	/* 超区域了 */
	
					 }
	 
					break;
				}
			}	 
		}  

}


void tft_at_once_disp_time_value(void)
{
    static uint8_t temp_decade_hours,temp_unit_hours,temp_decade_minutes,temp_unit_minutes;

       temp_decade_hours = gctl_t.disp_works_hours /10;
        temp_unit_hours = gctl_t.disp_works_hours % 10;
    
        temp_decade_minutes = gctl_t.disp_works_minutes/10;
        temp_unit_minutes = gctl_t.disp_works_minutes%10;
    
        //display hours value  
    
        TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(80,190,0,temp_decade_hours);
        TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(114,190,0,temp_unit_hours);
    
        //display minutes value  
        TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(166,190,0,temp_decade_minutes);
        TFT_Disp_Pic_WorkTime_Value_48_48_onBlack(200,190,0,temp_unit_minutes);

}


/*******************************************************************************************************
    **
    *Function Name:void TimeTimer_Pro_Handler(void)
    *Function :display works or timer timing 
    *Input Ref:NO
    *Return Ref:NO
    *
*********************************************************************************************************/
void TimeTimer_Pro_Handler(void)
{
 
  
  switch(gpro_t.disp_works_timer_timing_mode_item){


   case works_time://1

		
            
		if(gctl_t.power_on_dis_work_time ==0 && gctl_t.get_beijing_time_success == 1){

			
			TFT_Only_Disp_Timing();//
			
		}
       

         TFT_Display_WorksTime();

    break;

	case timer_time: //02 ->timer_time 
		if(gctl_t.gTimer_ctl_set_timer_time_senconds >59  && gpro_t.set_timer_timing_success == 1){

		
			gctl_t.gTimer_ctl_set_timer_time_senconds =0;

            #if UNIT_TESTING

                gctl_t.gSet_timer_minutes = gctl_t.gSet_timer_minutes - 20;


            #else

			    gctl_t.gSet_timer_minutes --;

            #endif 

           // gctl_t.gSet_timer_minutes = gctl_t.gSet_timer_minutes - 20;

			if(gctl_t.gSet_timer_minutes <0){
                
			    gctl_t.gSet_timer_minutes =59;
			    gctl_t.gSet_timer_hours --;
			
			}

			
			if(gctl_t.gSet_timer_hours < 0){
			 gpro_t.run_process_step=0xf0;
             App_PowerOff_Handler();

            }
            else{
			TFT_Disp_Set_TimerTime(0);

            }
		}
       
        if(gpro_t.set_timer_timing_success == 0){

             TFT_Disp_Set_TimerTime(0);
             gpro_t.disp_works_timer_timing_mode_item  =  works_time;

             TFT_Display_WorksTime();
        }

		
	break;

	

	 case timer_set_time://03

		if(gpro_t.gTimer_pro_set_long_key_tims > 2){
            gpro_t.gTimer_pro_set_long_key_tims =0;  
            
         
           if(gctl_t.gSet_timer_hours >0 ){

       
           
            gpro_t.disp_works_timer_timing_mode_item= timer_time;
            gpro_t.set_timer_timing_success = 1;

            gctl_t.gTimer_ctl_set_timer_time_senconds =0;

            gctl_t.gSet_timer_minutes =0;
            //TFT_Disp_Chinese_Timer_96_24(100,150);
            TFT_Disp_Chinese_Timer_23_23(TIMER_X0,TIMER_Y,0);//"定"
            TFT_Disp_Chinese_Timer_23_23(TIMER_X1,TIMER_Y,1);//"时"
            TFT_Disp_Chinese_Timer_23_23(TIMER_X2,TIMER_Y,2);//“时”
            TFT_Disp_Chinese_Timer_23_23(TIMER_X3,TIMER_Y,3);//“间”
            TFT_Only_Disp_Set_Timer_Blink();
            osDelay(200);//HAL_Delay(200);
            TFT_Disp_Onley_Set_TimerTime_Value();

           }
		   else{
               
			   if(wifi_link_net_state()==1 && gctl_t.get_beijing_time_success==1){
					
					gpro_t.disp_works_timer_timing_mode_item= works_time;//gpro_t.disp_works_timer_timing_mode_item
					TFT_Disp_Chinese_BeiJing_Time_23_23(TIMER_X0,TIMER_Y,0);
					TFT_Disp_Chinese_BeiJing_Time_23_23(TIMER_X1,TIMER_Y,1);
					TFT_Disp_Chinese_BeiJing_Time_23_23(TIMER_X2,TIMER_Y,2);
					TFT_Disp_Chinese_BeiJing_Time_23_23(TIMER_X3,TIMER_Y,3);
                    tft_at_once_disp_time_value();
					//TFT_Display_WorksTime();

				}
				else{

					
					gpro_t.disp_works_timer_timing_mode_item= works_time;//gpro_t.disp_works_timer_timing_mode_item
					//TFT_Disp_Chinese_WorksTime_96_24(100,150);
					TFT_Disp_Chinese_WorkTime_23_23(TIMER_X0,TIMER_Y,0);
					TFT_Disp_Chinese_WorkTime_23_23(TIMER_X1,TIMER_Y,1);
					TFT_Disp_Chinese_WorkTime_23_23(TIMER_X2,TIMER_Y,2);
					TFT_Disp_Chinese_WorkTime_23_23(TIMER_X3,TIMER_Y,3);
                    tft_at_once_disp_time_value();
					//TFT_Display_WorksTime();
			   }
               gpro_t.set_timer_timing_success = 0;
                
			}

        
			gpro_t.mode_key_run_item_step = mode_key_set_temp; //key input item by changed.
			
		}
	   break;


   
	}

}


