#include "bsp.h"

PRO_T gpro_t;

uint8_t led_blink_times;
uint8_t update_step;
uint8_t fan_continuce_run_flag;

//static void power_on_init_set_ref(void);

static void TFT_Pocess_Command_Handler(void);


static void Power_On_Init(void);

static void Power_Off_Fun(void);


uint16_t power_off_counter;



/*
*********************************************************************************************************
*	函 数 名: bsp_Idle
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/

void bsp_Init(void)
{

   TimerTiming_Init();
   Buzzer_Sound_Fun_Init();
   gpro_t.disp_works_timer_timing_mode_item = works_time;
   gpro_t.mode_key_run_item_step = mode_key_set_temp;

}

/*******************************************************************************************************
*	函 数 名: bsp_Idle
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参: 无
*	返 回 值: 无
**********************************************************************************************************/
void bsp_run_iwdg(void)
{
   
}


void bsp_run_Idle(void)
{

   static uint8_t power_on_first;

    if(power_on_first ==0){
        
          power_on_first ++;
          Update_DHT11_Value();
          
          TFT_Disp_Temp_Value(0,gctl_t.dht11_temp_value);
          TFT_Disp_Humidity_Value(gctl_t.dht11_hum_value);
          TFT_Display_Handler();
         
  
    }


   if(gpro_t.gPower_On == power_on){
   if(gpro_t.disp_works_timer_timing_mode_item == timer_time || gpro_t.disp_works_timer_timing_mode_item ==timer_set_time){
    
     TFT_DonnotDisp_Works_Time();

    }
   }

}
/*
*********************************************************************************************************
*
*	函 数 名: void TFT_Process_Handler(void)
*	功能说明: TFT显示的内容		 
*	形    参: 无
*	返 回 值: 无
*
*********************************************************************************************************
*/
void PowerOn_Process_Handler(void)
{
	
  
    switch(gpro_t.run_process_step){


	 case 0:
        DISABLE_INT();
        gpro_t.power_off_flag = 1;
        gpro_t.disp_works_timer_timing_mode_item = works_time;
	    gpro_t.gTimer_pro_disp_temphum = 0; //
	  
	    gpro_t.gTimer_pro_update_dht11_data=60;
        gpro_t.gTimer_read_humidity_value = 30;
        gctl_t.disp_ntc_res_switch_normal_ptc_counter  = 0 ;
        gpro_t.gTimer_run_total=0;

        //wifi of ref
	    wifi_t.gTimer_get_beijing_time=0;
        wifi_t.get_rx_beijing_time_enable=0;

        
	  Device_Action_No_Wifi_Power_On_Handler(); 
	  Power_On_Init();
	
      gpro_t.run_process_step=pro_disp_dht11_value;
	  Fan_Run();
      LED_Power_Key_On();

      ENABLE_INT();
	 
	 break;

	 case pro_disp_dht11_value: //1 //display works time + "temperature value " + "humidity value"

	   if(gpro_t.gTimer_pro_disp_temphum > 5){
           
		   gpro_t.gTimer_pro_disp_temphum=0;
            

		     Update_DHT11_Value();
            TFT_Disp_Only_Temp_Numbers(0,gctl_t.dht11_temp_value);
           
           
       }

      if(gpro_t.gTimer_read_humidity_value > 10){

           gpro_t.gTimer_read_humidity_value=0;

          TFT_Disp_Only_Humidity_Numbers(gctl_t.dht11_hum_value);


       }


      
       //update data to tencent cloud.
	   if(gpro_t.gTimer_pro_update_dht11_data > 34  && wifi_link_net_state() ==1){
		   gpro_t.gTimer_pro_update_dht11_data=0;

			Update_Dht11_Totencent_Value();

	   }



	 gpro_t.run_process_step=pro_run_main_fun;
	   
	case pro_run_main_fun: //02
	

	    RunMain_And_Interval_Handler();
	  
	   gpro_t.run_process_step=pro_disp_wifi_led;
	 break;

    case pro_disp_wifi_led: //4
	 
       if(wifi_link_net_state() ==1){
	      LED_WIFI_ICON_ON();
		  ModeKey_Select_Default_LedOnOff();
            
		}
		

	  gpro_t.run_process_step=pro_wifi_publish_init;
	 break; 
		  
      // handler of wifi 
	  case pro_wifi_publish_init: //7

      if(wifi_link_net_state()==1 && gpro_t.gTimer_get_data_from_tencent_data > 10){

            gpro_t.gTimer_get_data_from_tencent_data =0;

            Subscriber_Data_FromCloud_Handler();
            osDelay(200);//HAL_Delay(200)

      }
        
		

        if(wifi_link_net_state()==1 && wifi_t.smartphone_app_power_on_flag==0 && wifi_t.link_net_tencent_data_flag ==1){ //after send publish datat to tencent .){
               wifi_t.link_net_tencent_data_flag ++;
		   
		     MqttData_Publish_SetOpen(0x01);
		     HAL_Delay(200);
            
           
            

		}
		if(wifi_link_net_state()==1 && wifi_t.smartphone_app_power_on_flag==0 && wifi_t.link_net_tencent_data_flag ==2 ){
            
		    wifi_t.link_net_tencent_data_flag ++; 
		    MqttData_Publish_Update_Data();
		    HAL_Delay(200);
           
		}
	 
	   gpro_t.run_process_step=pro_check_time_out;

	 break;

	  case pro_check_time_out:
		
	    if(gpro_t.gTimer_run_total > 119){ //120 minutes
              gpro_t.gTimer_run_total =0;
              gpro_t.gTimer_run_time_out=0;  //time out recoder start 10 minutes
        
              gpro_t.interval_stop_run_flag  =1 ;
             
             gpro_t.run_process_step=pro_run_main_fun;
       }
       else{
         gpro_t.run_process_step=pro_disp_dht11_value;

       }

	   break;

    default:
    break;
   	}
 }
  
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

    gpro_t.gTimer_power_off_send_data_tencent=0;

    gctl_t.fan_continuce_flag =1;

    gpro_t.power_off_flag ++;
   // Power_Off_Fun();

    }
    
	if(wifi_link_net_state() ==1  ){
		
       TFT_DonnotDisp_Works_Time();
		
    }

    if(gctl_t.fan_continuce_flag ==1){

	    if(gpro_t.gTimer_countdown_one_minute   > 0 || gpro_t.gTimer_countdown_one_minute==0){
            Fan_Run();
            gpro_t.gTimer_power_off_send_data_tencent=0;
           
		}
		else{
			gctl_t.fan_continuce_flag++;
            TFT_BACKLIGHT_OFF();
            Fan_Stop();
            LCD_Clear(BLACK);
           
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
		wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;
	     
		}


      }

    if(gctl_t.fan_continuce_flag==1){



       TFT_Disp_CountDown_60s(gpro_t.gTimer_countdown_one_minute);

    }
   
    Breath_Led();

    if(wifi_link_net_state() ==1 && gpro_t.gTimer_power_off_send_data_tencent    > 34){

       gpro_t.gTimer_power_off_send_data_tencent =0;
	
       MqttData_Publish_PowerOff_Ref();
       osDelay(200);
      
	   Subscriber_Data_FromCloud_Handler(); //WT.EDIT 2024.07.22
       osDelay(200);

       wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;
	     
	 }
     else if(wifi_link_net_state() ==1){


        wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;

     }
	
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
	     gctl_t.fan_continuce_flag =1;
		 //clear control falg
        gpro_t.set_timer_timing_success = 0;
        
       
		//wifi ref 
        wifi_t.link_net_tencent_data_flag=1;
        wifi_t.smartphone_app_power_on_flag=0;
        wifi_t.power_off_step=0; 
        

        //display ptc temperature value 
        gctl_t.gSet_temperature_value_item = disp_ptc_temp_value_item;
        gctl_t.ptc_flag = 0;
        gctl_t.plasma_flag = 0;
	    gctl_t.ultrasonic_flag =0;
        gctl_t.set_ptc_temp_value_success =0;
        gctl_t.manual_operation_ptc_flag = ptc_manual_on;
        
        //main process ref
  
		gpro_t.gTimer_pro_wifi_fast_led=0;
    gpro_t.disp_works_timer_timing_mode_item = timer_countdown; //display count down timing .
	   

        Power_Off_Fun();
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
	*Function Name:void TFT_Pocess_Command_Handler(void)
	*Funcion: display of TFT lcd 
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
void PowerOnOff_Init_Ref_Fun(void)
{
   
       
  if(gpro_t.gPower_On == power_off){
          
         // gpro_t.gPower_On = power_on;  

          gpro_t.power_on_or_off_flag = power_on;
        
  
          gpro_t.run_process_step=0;
	    
   }
   else if(gpro_t.gPower_On == power_on){//POWER OFF
           
            gpro_t.power_off_flag=1;
            gpro_t.run_process_step=0;
            Power_Off_Fun();
            gpro_t.power_on_or_off_flag = power_off;


   }

 }


void power_off_handler(void)
{

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
void power_on_init_set_ref(void)
{
  
  if(wifi_link_net_state() ==1){
		    Donot_Disp_CountDown_60s();
		    TFT_Display_WorksTime_Voice();
            LED_WIFI_ICON_ON(); //WT.EDIT 2024.07.23
             
	}
   else{
            Donot_Disp_CountDown_60s();
            TFT_Display_PowerOn_WorksTime_Init();
           
	 }

       
         if(wifi_t.smartphone_app_power_on_flag==0){
		       power_on_action_led_init();
         }
         
        Update_DHT11_Value();
        TFT_Display_PowerOn_Init_Handler();
       
         TFT_Disp_Only_Temp_Numbers(0,gctl_t.dht11_temp_value);
         TFT_Disp_Only_Humidity_Numbers(gctl_t.dht11_hum_value);
         LED_Mode_Key_On();
	     LED_Power_Key_On();
		 TFT_BACKLIGHT_ON();

         gpro_t.disp_works_timer_timing_mode_item = works_time ;
         if(gctl_t.get_beijing_time_success == 1){

             LED_WIFI_ICON_ON(); //WT.EDIT 2024.07.23

            //gctl_t.disp_works_hours = gctl_t.save_time_hours[0] ;
            //gctl_t.disp_works_minutes  = gctl_t.save_time_hours[1]  ;
		  }
          else{

              gctl_t.disp_works_hours = 0;
              gctl_t.disp_works_minutes  = 0;
              gctl_t.gTimer_ctl_disp_works_time_second =0;
              

          }

         gctl_t.gSet_temperature_value=40;
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
	*Function Name: static void Power_On_Fun(void)
	*Function : power on
	*Input Ref:NO
	*Return Ref:No
	*
************************************************************************/
static void Power_On_Init(void)
{
  //led on 
 
  //smart phone control power on 
  if(wifi_t.smartphone_app_power_on_flag==0){
	  gctl_t.ptc_flag = 1;
      gctl_t.plasma_flag = 1;
	  gctl_t.ultrasonic_flag =1;


    if(wifi_link_net_state()==1)
	    LED_WIFI_ICON_ON()  ;             

	//PTC ICON LED
	LED_PTC_ICON_ON() ;  
    Ptc_On();
    gctl_t.ptc_flag =1;


	//KILL ICON LED
	LED_KILL_ICON_ON()  ; 
    Plasma_On();
    gctl_t.plasma_flag = 1;



	//ULTRSONIC ICO LED
	LED_RAT_ICON_ON()  ; 
    Ultrasonic_Pwm_Output();
    gctl_t.ultrasonic_flag = 1;

      
  }
  else{
   //  MqttData_Publish_Update_Data();
     Fan_Run();
     Device_Action_No_Wifi_Handler();


  }
 
   
   gctl_t.gSet_temperature_value =40;

 
   //timer timing
   gctl_t.mode_flag = works_time;
 
   gctl_t.gSet_timer_hours =0;

//works time
    if(wifi_link_net_state()==0 || gctl_t.get_beijing_time_success ==0){
		 gctl_t.disp_works_hours =0;
	     gctl_t.disp_works_minutes=0;
	     gctl_t.gTimer_ctl_disp_works_time_second=0;

     }
     else {
       gctl_t.power_on_dis_work_time =0;
       

     }
    
    

 }
     

/************************************************************************
	*
	*Function Name: static void Power_Off_Fun(void)
	*Function : power off
	*Input Ref:NO
	*Return Ref:No
	*
************************************************************************/
static void Power_Off_Fun(void)
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

/**********************************************************************************************************
    **
    *Function Name:void Power_Key_Detected(void)
    *Function : 
    *Input Ref:lightNum--LED ,filterNum -filter number, unionNum - smart menu number
    *Return Ref:NO
    *
*********************************************************************************************************/
void Wifi_Fast_Led_Blink(void)
{

  if(gpro_t.wifi_led_fast_blink_flag==1 && wifi_link_net_state()==0){
    if(gpro_t.gTimer_pro_wifi_led < 166){//2'46s

	if( gpro_t.gTimer_pro_wifi_fast_led < 80 ){ //50ms

	         LED_WIFI_ICON_ON();
            
	}
	else if(gpro_t.gTimer_pro_wifi_fast_led > 79 && gpro_t.gTimer_pro_wifi_fast_led< 161){

        
		LED_WIFI_ICON_OFF();
     
	}
	else{

		gpro_t.gTimer_pro_wifi_fast_led=0; 
    }
   }
   else{
	
	 gpro_t.wifi_led_fast_blink_flag=0;
   }

  }

  if(wifi_t.gTimer_linking_tencent_duration > 166 && gpro_t.wifi_led_fast_blink_flag==1 && wifi_link_net_state()==0){
      gpro_t.wifi_led_fast_blink_flag=0; 

   }
 
}



/**********************************************************************************************************
    **
    *Function Name:void Power_Key_Detected(void)
    *Function : 
    *Input Ref:lightNum--LED ,filterNum -filter number, unionNum - smart menu number
    *Return Ref:NO
    *
*********************************************************************************************************/
void WIFI_LED_Blink(void)
{

   if(wifi_link_net_state()==0 && gpro_t.gPower_On == power_on){
       if(gpro_t.mode_key_run_item_step != mode_key_select){
                    
        if(gpro_t.gTimer_pro_wifi_led > 1 && gpro_t.gTimer_pro_wifi_led < 3){

        LED_WIFI_ICON_ON();
        }
        else if(gpro_t.gTimer_pro_wifi_led > 2 && gpro_t.gTimer_pro_wifi_led < 5){

        gpro_t.gTimer_pro_wifi_led=0;
        LED_WIFI_ICON_OFF();
        }
        else if(gpro_t.gTimer_pro_wifi_led > 4){

            gpro_t.gTimer_pro_wifi_led=0;
             LED_WIFI_ICON_ON();

        }
        }

    }

}





