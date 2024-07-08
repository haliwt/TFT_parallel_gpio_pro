#include "bsp.h"


static uint8_t key_works_timer_timing_fun(void);

static uint8_t disp_works_timer_timing_fun(void);


void TimerTiming_Init(void)
{

    Key_Works_Timer_Timing_Handler(key_works_timer_timing_fun);

   Dis_pWorks_Timer_Timing_Handler(disp_works_timer_timing_fun);
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

		if(gctl_t.gTimer_ctl_disp_works_time_second > 59){
             gctl_t.power_on_dis_work_time = 2;
			TFT_Display_WorksTime();
           
		}

		if(gctl_t.power_on_dis_work_time ==0 && gctl_t.get_beijing_time_success == 1){

            gctl_t.disp_works_hours = gctl_t.save_time_hours[0] ;
           gctl_t.disp_works_minutes  = gctl_t.save_time_hours[1]  ;
			
			TFT_Only_Disp_Timing();//
			
		}
      

	break;

	case timer_time: //02 ->timer_time 
		if(gctl_t.gTimer_ctl_set_timer_time_senconds >59  && gpro_t.set_timer_timing_success == 1){

		
			gctl_t.gTimer_ctl_set_timer_time_senconds =0;

			gctl_t.gSet_timer_minutes --;

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
            HAL_Delay(200);
            TFT_Disp_Onley_Set_TimerTime_Value();

           }
		   else{
               
			   if(wifi_link_net_state()==1 && gctl_t.get_beijing_time_success==1){
					
					gpro_t.disp_works_timer_timing_mode_item= works_time;//gpro_t.disp_works_timer_timing_mode_item
					TFT_Disp_Chinese_BeiJing_Time_23_23(TIMER_X0,TIMER_Y,0);
					TFT_Disp_Chinese_BeiJing_Time_23_23(TIMER_X1,TIMER_Y,1);
					TFT_Disp_Chinese_BeiJing_Time_23_23(TIMER_X2,TIMER_Y,2);
					TFT_Disp_Chinese_BeiJing_Time_23_23(TIMER_X3,TIMER_Y,3);
					TFT_Display_WorksTime();

				}
				else{

					
					gpro_t.disp_works_timer_timing_mode_item= works_time;//gpro_t.disp_works_timer_timing_mode_item
					//TFT_Disp_Chinese_WorksTime_96_24(100,150);
					TFT_Disp_Chinese_WorkTime_23_23(TIMER_X0,TIMER_Y,0);
					TFT_Disp_Chinese_WorkTime_23_23(TIMER_X1,TIMER_Y,1);
					TFT_Disp_Chinese_WorkTime_23_23(TIMER_X2,TIMER_Y,2);
					TFT_Disp_Chinese_WorkTime_23_23(TIMER_X3,TIMER_Y,3);
					TFT_Display_WorksTime();
			   }
               gpro_t.set_timer_timing_success = 0;
                
			}

        
			gpro_t.mode_key_run_item_step = mode_key_set_temp; //key input item by changed.
			
		}
	   break;


   
	}

}

/*******************************************************************************************************
    **
    *Function Name:void Display_Precise_Works_Time(void)
    *Function :display works or beijing timing value 
    *Input Ref:NO
    *Return Ref:NO
    *
*********************************************************************************************************/
void Display_Precise_Works_Time(void)
{
	  if(gpro_t.disp_works_timer_timing_mode_item==timer_time){
	  	    TFT_DonnotDisp_Works_Time();
	  }
	  else{
	    if(gctl_t.gTimer_ctl_disp_works_time_second > 59){
				  TFT_Display_Precise_WorksTime();
			}


	  }


}
/********************************************************************************
 * 
 * Function Name: void Power_Off_Retain_Beijing_Time(void)
 * Function:  get beijing of timing 
 * Input Ref: 
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
					   if(gctl_t.disp_works_hours > 23){
						   gctl_t.disp_works_hours=0;
					   }
			     }
			
   }
 }
}

	
/*******************************************************************************************************
    **
    *Function Name:void Power_Key_Detected(void)
    *Function : 
    *Input Ref:lightNum--LED ,filterNum -filter number, unionNum - smart menu number
    *Return Ref:NO
    *
*********************************************************************************************************/
void Key_Works_Timer_Timing_Handler(uint8_t(*worktiming_handler)(void))
{
     gpro_t.key_works_timer_timing_state = worktiming_handler;

}


static uint8_t key_works_timer_timing_fun(void)
{

     if(gpro_t.mode_key_run_item_step==mode_key_set_temp)return mode_key_set_temp;
	 else if(gpro_t.mode_key_run_item_step==mode_key_select)return mode_key_select;
	 else if(gpro_t.mode_key_run_item_step==mode_key_confirm)return mode_key_confirm;
     else if(gpro_t.mode_key_run_item_step==mode_key_set_timer_value)return mode_key_set_timer_value;
	 else 
	 	return  mode_key_set_temp;

	 

}

void Dis_pWorks_Timer_Timing_Handler(uint8_t(*disp_worktimer_handler)(void))
{

    gpro_t.disp_works_timer_timing_state = disp_worktimer_handler;

}




static uint8_t disp_works_timer_timing_fun(void)
{

  if(gpro_t.disp_works_timer_timing_mode_item==works_time)return works_time ;
  else if(gpro_t.disp_works_timer_timing_mode_item==timer_time)return timer_time ;
  else if(gpro_t.disp_works_timer_timing_mode_item==timer_set_time)return timer_set_time ;
   
   return 0;

}





