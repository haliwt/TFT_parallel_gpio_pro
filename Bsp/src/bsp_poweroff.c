#include "bsp.h"




/*****************************************************************
 * 
 * Function Name : void Power_Off_Handler(void)
 * Function : handle power off function 
 * Input Ref:No
 * Return Ref:NO
 * 
******************************************************************/
 void Power_Off_Process_Handler(void)
 {   
 
    if(gpro_t.power_off_flag == 1){


        DISABLE_INT();
        LCD_Clear(BLACK);
        //TFT_Disp_Fan_Leasefiness_RunIcon(100,30,0);

        ENABLE_INT();
        gpro_t.gTimer_countdown_one_minute =90;
        TFT_Disp_CountDown_60s(gpro_t.gTimer_countdown_one_minute);
        Device_NoAction_Power_Off();

        gctl_t.ptc_warning=0;
        gctl_t.fan_warning =0;
        wifi_t.repeat_login_tencent_cloud_init_ref=0;
        wifi_t.smartphone_app_power_on_flag=0; //手机定时关机和开机，设置参数的标志位
        gctl_t.disp_ntc_res_switch_normal_ptc_counter=0;

        gctl_t.fan_continuce_flag =1;

        gpro_t.power_off_flag ++;


    }

    if(gctl_t.fan_continuce_flag ==1){ //power off fan run need run one minute

	    if(gpro_t.gTimer_countdown_one_minute   > 0 || gpro_t.gTimer_countdown_one_minute==0){
            
            Fan_Run();  //this is fan need run 
            gpro_t.gTimer_get_data_from_tencent_data=0;
           
		}
		else{
			//gctl_t.fan_continuce_flag++;
			gctl_t.fan_continuce_flag=2; //WT.EIDT 2024.12.24
            TFT_BACKLIGHT_OFF();
            Fan_Stop();
          
           
		}
    }
    
    if(gpro_t.power_off_flag ==2){
        gpro_t.power_off_flag ++;
        Power_Off_Fun();

        TFT_Disp_Fan_Leasefiness_RunIcon(100,30,0);

        if(wifi_link_net_state() ==1){

            wifi_t.link_net_tencent_data_flag=1;
            MqttData_Publish_PowerOff_Ref();
            osDelay(50);//HAL_Delay(100);
            wifi_t.link_net_step= wifi_publish_update_tencent_cloud_data;

        }
    }

    if(gctl_t.fan_continuce_flag==1){



       TFT_Disp_CountDown_60s(gpro_t.gTimer_countdown_one_minute);

    }
   
    Breath_Led();

 

}
/*****************************************************************
 * 
 * Function Name : void PowerOff_Ref_Fun(void)
 * Function : handle power off function 
 * Input Ref:No
 * Return Ref:NO
 * 
******************************************************************/
void PowerOff_Ref_Fun(void)
{
 
	   
       //  LCD_Clear(BLACK);
		 //clear control falg
        gctl_t.fan_continuce_flag =1;
        gpro_t.set_timer_timing_success = 0;
        
       
		//wifi ref 
        wifi_t.link_net_tencent_data_flag=1;
        wifi_t.smartphone_app_power_on_flag=0;
      
        

        //display ptc temperature value 
        gctl_t.gSet_temperature_value_item = disp_ptc_temp_value_item;
        gctl_t.ptc_flag = 0;
        gctl_t.plasma_flag = 0;
	    gctl_t.ultrasonic_flag =0;
        gctl_t.set_ptc_temp_value_success =0;
        gctl_t.manual_operation_ptc_flag = ptc_manual_on;
        
        //main process ref
  
		gpro_t.gTimer_pro_wifi_fast_led=0;
	    gpro_t.disp_works_timer_timing_mode_item = works_time;
        /*********************************************/

       // Power_Off_Fun();
       
            LED_Mode_Key_Off();
            LED_Power_Key_Off();
            LED_WIFI_ICON_OFF();
        
            //Power_Off_Led();
           gctl_t.mode_flag = 0;
           gpro_t.gPower_On = power_off;
        
           gctl_t.plasma_flag = 0;
           gctl_t.ultrasonic_flag =0;
           gctl_t.ptc_flag = 0;
           //timer timing flag
         
        
            gpro_t.wifi_led_fast_blink_flag=0;
            gctl_t.gSet_timer_hours =0;
        
            gctl_t.ptc_warning = 0;
            gctl_t.fan_warning=0;
        
            Ultrasonic_Pwm_Stop();
            Plasma_Off();
            Ptc_Off();
        /****************************************/
		LED_Mode_Key_Off();
}
/******************************************************************************
	*
	*Function Name:void Power_Off_Special_Fun(void)
	*Funcion: speical of power key function
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
void Power_Off_Special_Fun(void)
{

  
    buzzer_sound();
    DISABLE_INT();
     LCD_Clear(BLACK);
     ENABLE_INT();
    PowerOff_Ref_Fun();
    gpro_t.power_off_flag=1;
           
     gpro_t.gPower_On = power_off;  
     gpro_t.run_process_step=0xff;
 }
/******************************************************************************
	*
	*Function Name:void Power_Off_Special_Fun(void)
	*Funcion: speical of power key function
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
void Power_On_Special_Fun(void)
{
       buzzer_sound();	
       
       DISABLE_INT();
       LCD_Clear(BLACK);
       ENABLE_INT();
       
	 //  Donot_Disp_CountDown_60s();
	 //  HAL_Delay(50);
       power_on_init_set_ref();

     
       gpro_t.gPower_On = power_on;   
  
       gpro_t.run_process_step=0;


}
/******************************************************************************
	*
	*Function Name:void power_off_handler(void)
	*Funcion: speical of power key function
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
void power_off_handler(void)
{

    LED_Mode_Key_Off();
	LED_Power_Key_Off();
    LED_WIFI_ICON_OFF();

	//Power_Off_Led();
   gctl_t.mode_flag = 0;
   gpro_t.gPower_On = power_off;

   gctl_t.plasma_flag = 0;
   gctl_t.ultrasonic_flag =0;
   gctl_t.ptc_flag = 0;
   //timer timing flag
 

	gpro_t.wifi_led_fast_blink_flag=0;
	gctl_t.gSet_timer_hours =0;

	gctl_t.ptc_warning = 0;
	gctl_t.fan_warning=0;

	Ultrasonic_Pwm_Stop();
	Plasma_Off();
    Ptc_Off();

    /*****************************/
    gpro_t.power_off_flag=1;
    gpro_t.gPower_On = power_off; 
    PowerOff_Ref_Fun();
    //gpro_t.run_process_step=0xff;
    gpro_t.run_process_step=0;

}

/******************************************************************************
	*
	*Function Name:void power_on_init_set_ref(void)
	*Funcion: display of TFT lcd 
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
void power_off_init_set_ref(void)
{
  
  
      Power_Off_Fun();
   
       
		 
 }
 /************************************************************************
	*
	*Function Name: void Power_Off_Fun(void)
	*Function : power off
	*Input Ref:NO
	*Return Ref:No
	*
************************************************************************/
void Power_Off_Fun(void)
{
	LED_Mode_Key_Off();
	LED_Power_Key_Off();
    LED_WIFI_ICON_OFF();

	//Power_Off_Led();
   gctl_t.mode_flag = 0;
   gpro_t.gPower_On = power_off;

   gctl_t.plasma_flag = 0;
   gctl_t.ultrasonic_flag =0;
   gctl_t.ptc_flag = 0;
   //timer timing flag
 

	gpro_t.wifi_led_fast_blink_flag=0;
	gctl_t.gSet_timer_hours =0;

	gctl_t.ptc_warning = 0;
	gctl_t.fan_warning=0;

	Ultrasonic_Pwm_Stop();
	Plasma_Off();
    Ptc_Off();

	
	//clear set timer timing value and flag 
}
