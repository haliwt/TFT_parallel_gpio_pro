#include "bsp.h"

PRO_T pro_t;

uint8_t led_blink_times;
uint8_t update_step;
uint8_t fan_continuce_run_flag;


static void TFT_Pocess_Command_Handler(void);

static void Power_On_Fun(void);
static void Power_Off_Fun(void);



static void Key_Interrup_Handler(void);




void bsp_Init(void);

uint16_t power_off_counter;

static uint8_t confirm_key_data;



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

   pro_t.mode_key_run_item_step=0xff;
  // Buzzer_Sound_Fun_Init();


}
/*
*********************************************************************************************************
*	函 数 名: bsp_Idle
*	功能说明: 空闲时执行的函数。一般主程序在for和while循环程序体中需要插入 CPU_IDLE() 宏来调用本函数。
*			 本函数缺省为空操作。用户可以添加喂狗、设置CPU进入休眠模式的功能。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void bsp_Idle(void)
{

	if(pro_t.power_on_first ==0){
      
	 
	
	    Update_DHT11_Value();
	    TFT_Disp_Temp_Value(0,gctl_t.dht11_temp_value);
        TFT_Disp_Humidity_Value(gctl_t.dht11_hum_value);
        TFT_Display_Handler();
		 pro_t.power_on_first =1;

	}
	
	/* --- 喂狗 */
    if(pro_t.gTimer_pro_feed_dog > 3){ //16s
	pro_t.gTimer_pro_feed_dog = 0;	
	iwdg_feed();

    }

    TFT_Disp_Timer_Split_Symbol();
    Wifi_Fast_Led_Blink();
	/* --- 让CPU进入休眠，由Systick定时中断唤醒或者其他中断唤醒 */

	/* 例如 emWin 图形库，可以插入图形库需要的轮询函数 */
	//GUI_Exec();

	/* 例如 uIP 协议，可以插入uip轮询函数 */
	//TOUCH_CapScan();
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
void TFT_Process_Handler(void)
{
	
   static uint8_t fan_continuce_flag;
	if(pro_t.buzzer_sound_flag ==1 && pro_t.power_on_first==1){
		pro_t.buzzer_sound_flag=0;
		Buzzer_KeySound();
	}
	
	Key_Speical_Power_Fun_Handler();

	switch(pro_t.gPower_On){
	
	case power_on:
		
	    Key_Speical_Mode_Fun_Handler();
    	Key_Interrup_Handler();
	    TFT_Pocess_Command_Handler();
		v_t.voice_soun_output_enable = 1;
	break;

	case power_off:

	if(pro_t.power_off_flag == 1){
		pro_t.power_off_flag =0;
	    wifi_t.power_off_step=0; 
	    fan_continuce_flag =1;
		pro_t.gTimer_pro_fan =0;
		//LCD_Clear(BLACK);
		TFT_BACKLIGHT_OFF();
		Power_Off_Fun();
		Device_NoAction_Power_Off();
		LED_Mode_Key_Off();
		
		
	}
	if(wifi_link_net_state() ==1  && wifi_t.gTimer_main_pro_times > 50){
		wifi_t.gTimer_main_pro_times=0;	
		MqttData_Publish_PowerOff_Ref();
		
    }

	if(fan_continuce_flag ==1){

	    if(pro_t.gTimer_pro_fan <61){
            Fan_Run();        

		}
		else{
			fan_continuce_flag++;

            Fan_Stop();
		}
      

	}
	if(v_t.voice_cmd_power_off_flag ==1){
		v_t.voice_cmd_power_off_flag ++;;

        v_t.gTimer_voice_time=0;


	}
	if(v_t.gTimer_voice_time > 2 && v_t.voice_cmd_power_off_flag==2){
		v_t.voice_cmd_power_off_flag++;
		//VOICE_SOUND_DISABLE();
    }
    gctl_t.ptc_warning=0;
	gctl_t.fan_warning =0;
	wifi_t.repeat_login_tencent_cloud_init_ref=0;
	

	wifi_t.smartphone_app_power_on_flag=0; //手机定时关机和开机，设置参数的标志位
	
	Power_Off_Retain_Beijing_Time();
	
	Breath_Led();
	


	break;


	}
	
}




/******************************************************************************
	*
	*Function Name:void TFT_Pocess_Command_Handler(void)
	*Funcion: display of TFT lcd 
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
static void TFT_Pocess_Command_Handler(void)
{
   static uint8_t ptc_first_on ,ptc_on_flag;;
	if(power_on_state() == power_on){
  
    switch(pro_t.run_process_step){


	 case 0:
	 	ptc_on_flag =1;
		pro_t.gKey_value =0XFF;
		TFT_Display_WorksTime();
		Power_On_Fun();
	    Fan_Run();
		Device_Action_No_Wifi_Power_On_Handler();

		TFT_BACKLIGHT_ON();

		v_t.voice_soun_output_enable = 1;
		pro_t.run_process_step=pro_disp_dht11_value;
		pro_t.gTimer_pro_ptc_delay_time=0;
		pro_t.gTimer_pro_display_dht11_value=30; //at once display dht11 value
		gctl_t.gTimer_ctl_dma_state =0;
        pro_t.add_or_dec_is_cofirm_key_flag =0; //key set example "ptc,kill,driver rat" function. don't compart tempartur value
		//test item 
		//gctl_t.ptc_warning=1;
		//gctl_t.fan_warning = 1;
		
	 break;

	 case pro_disp_dht11_value: //1 //display works time + "temperature value " + "humidity value"

	   Wifi_Fast_Led_Blink();
	

	   if(pro_t.gTimer_pro_display_dht11_value > 28 &&  pro_t.wifi_led_fast_blink_flag==0){
	   	   pro_t.gTimer_pro_display_dht11_value=0;
         
		    Update_DHT11_Value();
		
            if(pro_t.mode_key_run_item_step != mode_key_temp){
			   TFT_Disp_Temp_Value(0,gctl_t.dht11_temp_value);
            }
			TFT_Disp_Humidity_Value(gctl_t.dht11_hum_value);

	   }
       if(pro_t.gTimer_pro_action_publis > 4 && wifi_link_net_state()==1){
	   	  pro_t.gTimer_pro_action_publis=0;
	       Device_Action_Publish_Handler();

       }
	   
	   pro_t.run_process_step=pro_run_main_fun;
	   
	case pro_run_main_fun: //02
	
   
	  Wifi_Fast_Led_Blink();

	   Fan_Pro_Handler();
	   pro_t.run_process_step=pro_disp_works_time;
	 break;

	 case pro_disp_works_time: //display works times and timer timing .

		Wifi_Fast_Led_Blink();
	
	
	    TimeTimer_Pro_Handler();
			

		pro_t.run_process_step=pro_set_temperature;


    case pro_set_temperature:


	  Temperature_Ptc_Pro_Handler();
		
    
      pro_t.run_process_step=pro_disp_wifi_led;

	break;

	case pro_disp_wifi_led: //4
	
		  if(wifi_link_net_state() ==0){
		  
			if(pro_t.mode_key_select_label ==1){

				LED_WIFI_ICON_OFF();

            }
            else{
             switch(pro_t.wifi_led_fast_blink_flag){

			 case 1:
				Wifi_Fast_Led_Blink();

			 	pro_t.run_process_step=1;
             break;

             case 0:
				if(pro_t.gTimer_pro_wifi_led > 1 && pro_t.gTimer_pro_wifi_led < 3){
					
					LED_WIFI_ICON_ON();
				}
				else if(pro_t.gTimer_pro_wifi_led > 2){

					pro_t.gTimer_pro_wifi_led=0;
					LED_WIFI_ICON_OFF();
				}
				
			 break;

             }

            }
			

		}
		else{

		  LED_WIFI_ICON_ON();

		}
		  
	  pro_t.run_process_step=pro_wifi_init;
	 break; 
		  
      // handler of wifi 
	  case pro_wifi_init: //7
	   
      Wifi_Pro_Runing_Init();
		pro_t.run_process_step=pro_disp_dht11_value;

	  break;


    default:
    break;
   	}
   }
  
   
}
/************************************************************************
	*
	*Function Name: static void Power_On_Fun(void)
	*Function : power on
	*Input Ref:NO
	*Return Ref:No
	*
************************************************************************/
static void Power_On_Fun(void)
{
  //led on 
  LED_Mode_Key_On();
  LED_Power_Key_On();
  Power_On_Led_Init();
  //smart phone control power on 
  if(wifi_t.smartphone_app_power_on_flag==0){
	  gctl_t.ptc_flag = 1;
      gctl_t.plasma_flag = 1;
	  gctl_t.ultrasonic_flag =1;
  }
   
   gctl_t.gSet_temperature_value =40;

 
   //timer timing
   gctl_t.mode_flag = works_time;
   gctl_t.timer_time_define_flag = 0;
    pro_t.timer_mode_flag=works_time;
	 gctl_t.gSet_timer_hours =0;
	 gctl_t.gSet_timer_minutes =0;

	 //mode key long times 
	  pro_t.mode_key_run_item_step=0xff;
	 gctl_t.mode_key_long_time_flag=0;
	 pro_t.long_key_flag =0;


    
     //works time
	gctl_t.gTimer_ctl_total_continue_time =0; //works total is two hours recoder.
	gctl_t.gTimer_ctl_disp_works_time_second=0; //works time seconds 
    pro_t.gTimer_pro_display_dht11_value = 30; //powe on display sensoe dht11 of value .
    if(wifi_link_net_state()==0){
		 gctl_t.disp_works_hours =0;
	     gctl_t.disp_works_minutes=0;
	     gctl_t.gTimer_ctl_disp_works_time_second=0;

   }

 }
     


static void Power_Off_Fun(void)
{
	LED_Mode_Key_Off();
	LED_Power_Key_Off();

	Power_Off_Led();
   gctl_t.mode_flag = 0;
   pro_t.gPower_On = power_off;
   pro_t.mode_key_run_item_step=0xff;
   gctl_t.plasma_flag = 0;
   gctl_t.ultrasonic_flag =0;
   gctl_t.ptc_flag = 0;
   //timer timing flag
     pro_t.timer_mode_flag=works_time;
	pro_t.timer_mode_flag=0;
	pro_t.wifi_led_fast_blink_flag=0;
	gctl_t.gSet_timer_hours =0;

	gctl_t.ptc_warning = 0;
	gctl_t.fan_warning=0;
	if(wifi_link_net_state()==0){
		 gctl_t.disp_works_hours =0;
	     gctl_t.disp_works_minutes=0;

   }
	//clear set timer timing value and flag 
}
void power_off_fan_run(void)
{
	
    LED_Mode_Key_Off();
	Breath_Led();
	

}

/******************************************************************************
	*
	*Function Name:void TFT_Pocess_Command_Handler(void)
	*Funcion: display of TFT lcd 
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
static void Key_Interrup_Handler(void)
{
     switch(pro_t.gKey_value){

        case add_key_id:
		 	
		 	if(ADD_KEY_VALUE()==KEY_DOWN){
			    HAL_Delay(10);
			if(ADD_KEY_VALUE()==KEY_DOWN)
			      ADD_Key_Fun();//DEC_Key_Fun();
			  

		 	}
			 pro_t.gKey_value =0XFF;

		break;

		case dec_key_id:
           if(DEC_KEY_VALUE()==KEY_DOWN){
			 HAL_Delay(10);
			if(DEC_KEY_VALUE()==KEY_DOWN)
			    DEC_Key_Fun();//ADD_Key_Fun();
			 }

			 pro_t.gKey_value =0XFF;
        break;


	 }

	// Mode_Key_Config_Fun_Handler();

   
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
   if(pro_t.wifi_led_fast_blink_flag==1 && wifi_link_net_state()==0){
wifi_led: if(pro_t.gTimer_pro_wifi_led < 166){//2'46s

	if( pro_t.gTimer_pro_wifi_fast_led < 80 ){ //50ms

	         LED_WIFI_ICON_ON();
	}
	else if(pro_t.gTimer_pro_wifi_fast_led > 79 && pro_t.gTimer_pro_wifi_fast_led< 161){

        
		LED_WIFI_ICON_OFF();
	}
	else{

		pro_t.gTimer_pro_wifi_fast_led=0;
		goto wifi_led;

		
	  }
   }
   else{
	
	 pro_t.wifi_led_fast_blink_flag=0;
   }

  }
}
	



