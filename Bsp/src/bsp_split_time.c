#include "bsp.h"


static uint8_t key_works_timer_timing_fun(void);

static uint8_t disp_works_timer_timing_fun(void);


void TimerTiming_Init(void)
{

    Key_Works_Timer_Timing_Handler(key_works_timer_timing_fun);

   Dis_pWorks_Timer_Timing_Handler(disp_works_timer_timing_fun);
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





