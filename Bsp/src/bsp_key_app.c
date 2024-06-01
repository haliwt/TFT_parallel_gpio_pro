#include "bsp.h"


uint8_t ptc_fun_led_init_flg , plasma_fun_led_init_flag, rat_fun_led_init_flag;



/******************************************************************************
	*
	*Function Name:static void Key_Speical_Power_Fun_Handler(void)
	*Funcion: speical of power key function
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
void Key_Speical_Power_Fun_Handler(void)
{
	static uint8_t power_on_off;

	
      if(KEY_POWER_VALUE() ==KEY_DOWN && pro_t.key_power_be_pressed_flag < 60   &&  pro_t.gPower_On == power_on){
            pro_t.key_power_be_pressed_flag++;

            if(pro_t.key_power_be_pressed_flag > 40   && KEY_POWER_VALUE() == 1){
               pro_t.key_power_be_pressed_flag = 100;
			pro_t.gTimer_pro_wifi_led =0;
            pro_t.wifi_led_fast_blink_flag=1;
			
			//WIFI CONNCETOR process
			wifi_t.link_tencent_step_counter=0;
			wifi_t.esp8266_login_cloud_success =0;
			wifi_t.runCommand_order_lable=wifi_link_tencent_cloud;
			wifi_t.wifi_config_net_lable= wifi_set_restor;
			wifi_t.gTimer_login_tencent_times=0;
			wifi_t.gTimer_linking_tencent_duration=0; //166s -2分7秒
			buzzer_sound();
			
			 
        }

	 }
	 else if(KEY_POWER_VALUE() ==KEY_UP && pro_t.key_power_be_pressed_flag > 0 && pro_t.key_power_be_pressed_flag < 40){
             
            pro_t.key_power_be_pressed_flag=0;


        
           power_on_off = power_on_off ^ 0x01;
			  pro_t.key_power_be_pressed_flag=0;
		  if(power_on_off==1){
		
		  

			buzzer_sound();	
			pro_t.gPower_On = power_on;   
            pro_t.long_key_flag =0;
            pro_t.run_process_step=0;
		   

		
			
		  }
		  else {
			 //pro_t.gKey_value = power_key_id;
			 buzzer_sound();
			 pro_t.power_off_flag=1;
			 pro_t.long_key_flag =0;
			pro_t.run_process_step=0xff;
			pro_t.gPower_On = power_off;   
		
			  
			 }
		}
}

/******************************************************************************
	*
	*Function Name:static void Key_Speical_Mode_Fun_Handler(void)
	*Funcion: speical of mode key fun
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
void Key_Speical_Mode_Fun_Handler(void)
{
 //modke _key_long_time


		//mode key be pressed long times
	if(KEY_MODE_VALUE() ==KEY_DOWN && pro_t.mode_key_pressed_flag < 100){


        pro_t.mode_key_pressed_flag ++;
		if(pro_t.mode_key_pressed_flag >  40 && KEY_MODE_VALUE() == KEY_DOWN){
			pro_t.mode_key_pressed_flag =150;
			pro_t.mode_key_select_label =0;
             buzzer_sound();
		    pro_t.gTimer_pro_mode_long_key=0;
		    pro_t.key_mode_long_time_over_flag=1;
			Mode_Long_Key_Fun();

		   
       }
    }
    else if(KEY_MODE_VALUE() ==KEY_UP && pro_t.mode_key_pressed_flag > 0 && pro_t.mode_key_pressed_flag < 40){
       
		pro_t.mode_key_pressed_flag =0;
	   
		
		pro_t.mode_key_run_item_step = mode_key_select;
	    pro_t.mode_key_select_label =mode_key_select;
	     gctl_t.select_main_fun_numbers++; // 0,1,2
		 if(gctl_t.select_main_fun_numbers > 3){
			  gctl_t.select_main_fun_numbers = 1;
			}
		
		gctl_t.memory_confimr_key_done = 1;
        
		pro_t.buzzer_sound_flag =1;

		pro_t.gTimer_pro_mode_key_be_select = 0; //counter starts after 4 seconds ,cancel this function
		gctl_t.gTimer_ctl_select_led =0;
        buzzer_sound();
	   	
	   }
		
		Mode_Key_Config_Fun_Handler();
}

/******************************************************************************
	*
	*Function Name:static void Mode_Key_Config_Fun_Handler(void)
	*Funcion: speical of mode key fun
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
void Mode_Key_Config_Fun_Handler(void)
{
  static uint8_t confirm_data;
  switch(pro_t.mode_key_run_item_step){


             case mode_key_select: //02

		     if(pro_t.gTimer_pro_mode_key_be_select < 4){ //exit of rule

				Mode_Key_Select_Fun();
				
             }
			 else{
                gctl_t.memory_confimr_key_done = 0;
                pro_t.mode_key_run_item_step = 0xff; //
                pro_t.mode_key_select_label =0;
                pro_t.key_mode_long_time_over_flag =0;//pro_t.mode_key_select_label
			    gctl_t.select_main_fun_numbers--; //return back the first confirm item 
				if(gctl_t.select_main_fun_numbers == 0){
					gctl_t.select_main_fun_numbers = 5;
				}
				pro_t.add_or_dec_is_cofirm_key_flag =0;
                Device_Action_Led_OnOff_Handler();
			 }
             

		   break;


		   case mode_key_confirm: //03//as "+" and "-" key  confirm ation
			    Device_Action_Led_OnOff_Handler();
                Mode_Key_Confirm_Fun();
		    
				if(confirm_data==0){
					confirm_data++;
					pro_t.gTimer_pro_confir_delay=0;

				}

				if(confirm_data==1 && pro_t.gTimer_pro_confir_delay>1){
					confirm_data=0;
					pro_t.gTimer_pro_confir_delay=0;
				 	pro_t.mode_key_select_label=0;
				   pro_t.mode_key_run_item_step = 0xff;
                       pro_t.add_or_dec_is_cofirm_key_flag =1;
				}
			

			break;

			default:

			break;

	    }


       
     
}

/************************************************************************
	*
	*Function Name: static void Mode_Long_Key_Fun(void) 
	*Function : set timer timing is enable 
	*Input Ref:NO
	*Return Ref:No
	*
************************************************************************/
void Mode_Long_Key_Fun(void)  //MODE_KEY_LONG_TIME_KEY://case model_long_key:
{
	  if(power_on_state() ==power_on){
	   if(gctl_t.fan_warning ==0 && ptc_error_state() ==0){
	  	  pro_t.mode_key_run_item_step = mode_key_set_timer_value;
		  pro_t.timer_mode_flag=timer_set_time; //set timer mode enable,
		  gctl_t.timer_timing_words_changed_flag ++;
		  gctl_t.timing_words_changed_flag++;
		  pro_t.gTimer_pro_mode_long_key=0;
		 


	     TFT_Disp_Set_TimerTime_Init();

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
void ADD_Key_Fun(void)
{

 static uint8_t select_flag,disp_temp_value,timer_timing_flag;
 if(power_on_state()==power_on){

	if(gctl_t.ptc_warning ==0 && ptc_error_state() ==0){



		switch(pro_t.mode_key_run_item_step){

		case 0xff:
            if(pro_t.key_mode_long_time_over_flag ==0 && pro_t.mode_key_select_label==0){
			   pro_t.mode_key_run_item_step=mode_key_set_temp;

            }

		case mode_key_set_temp: //set temperature value add number
			//pro_t.buzzer_sound_flag = 1;
			if(pro_t.key_mode_long_time_over_flag ==0 && pro_t.mode_key_select_label ==0){
			//Buzzer_KeySound();
			gctl_t.gSet_temperature_value ++;
			if( gctl_t.gSet_temperature_value < 20)gctl_t.gSet_temperature_value=20;
			
            if(gctl_t.gSet_temperature_value > 40) gctl_t.gSet_temperature_value= 20;

             pro_t.gTimer_pro_set_tem_value_blink =0;
			 gctl_t.gSet_temperature_value_item = set_temp_value_item;
        
			disp_temp_value =1;
			//TFT_Disp_Temp_Value(0,gctl_t.gSet_temperature_value);	
			}
		break;

		case mode_key_set_timer_value:
            //pro_t.buzzer_sound_flag = 1;
		    pro_t.gTimer_pro_mode_long_key=0;
			gctl_t.mode_key_long_time_flag++;
			
			gctl_t.gSet_timer_minutes=0;
			gctl_t.gSet_timer_hours ++ ;//disp_t.disp_timer_time_hours++ ;//pro_t.dispTime_minutes = pro_t.dispTime_minutes + 60;
			if(gctl_t.gSet_timer_hours  > 24){ //if(pro_t.dispTime_minutes > 59){

			gctl_t.gSet_timer_hours =0;//pro_t.dispTime_hours =0;


			}

		   pro_t.gTimer_pro_mode_long_key=0 ; //long key for mode timing
			timer_timing_flag=1;
		 
			

		break;

		

		case mode_key_select:
			
		//	pro_t.buzzer_sound_flag = 1;
			pro_t.mode_key_run_item_step = mode_key_confirm;
			
		
        break; 
		

		}	
	}
	
    if(disp_temp_value ==1){
		disp_temp_value =0;
    TFT_Disp_Temp_Value(0,gctl_t.gSet_temperature_value);

    }
    if(timer_timing_flag ==1){
		timer_timing_flag=0;
	  TFT_Disp_Set_TimerTime(0);


    }

 }
   
	 

}
/************************************************************************
	*
	*Function Name: static void DEC_Key_Fun(void)
	*Function : KEY OF decrease
	*Input Ref:NO
	*Return Ref:No
	*
************************************************************************/
void DEC_Key_Fun(void)
{
    static uint8_t disp_temp_value,timer_timing_flag;
	if(power_on_state() ==power_on){
	   	if(gctl_t.ptc_warning ==0 && ptc_error_state() ==0 ){
	   	
	     switch(pro_t.mode_key_run_item_step ){

		   case 0xff:
            if(pro_t.key_mode_long_time_over_flag ==0 && pro_t.mode_key_select_label ==0){
		 	 pro_t.mode_key_run_item_step= mode_key_set_temp;
            }

		   case mode_key_set_temp:  //default tempearture value 
	        // pro_t.buzzer_sound_flag = 1;
	        if(pro_t.key_mode_long_time_over_flag ==0 && pro_t.mode_key_select_label ==0){
	      //  Buzzer_KeySound();
			 gctl_t.gSet_temperature_value--;
			if( gctl_t.gSet_temperature_value<20)  gctl_t.gSet_temperature_value=40;
	        if( gctl_t.gSet_temperature_value >40) gctl_t.gSet_temperature_value=40;
   
			 pro_t.gTimer_pro_set_tem_value_blink =0;
			 gctl_t.gSet_temperature_value_item = set_temp_value_item;
             disp_temp_value =1;
	        
			//TFT_Disp_Temp_Value(0,gctl_t.gSet_temperature_value);
	        }
			break;

			case mode_key_set_timer_value: //timer timing set "decrease -down"
			   
			  //  pro_t.buzzer_sound_flag = 1;
	            gctl_t.mode_key_long_time_flag++;
			
				gctl_t.gSet_timer_minutes=0;
				gctl_t.gSet_timer_hours --;//disp_t.disp_timer_time_hours -- ;//pro_t.dispTime_minutes = pro_t.dispTime_minutes - 1;
				if(gctl_t.gSet_timer_hours  < 0){//if(pro_t.dispTime_minutes < 0){

				    gctl_t.gSet_timer_hours  =24;//pro_t.dispTime_hours --;
					
					
				}
		
		     pro_t.gTimer_pro_mode_long_key=0 ; //long key for mode timing
		     timer_timing_flag=1;
			 	
			//TFT_Disp_Set_TimerTime(0);
			break;

			 case mode_key_select:
			 
			 //	pro_t.buzzer_sound_flag = 1;
	
				pro_t.mode_key_run_item_step = mode_key_confirm;
				

			   
			
			break;


	    	}
	   	  }
		if(disp_temp_value ==1){
		disp_temp_value =0;
    		TFT_Disp_Temp_Value(0,gctl_t.gSet_temperature_value);

    	}
	    if(timer_timing_flag ==1){
		   timer_timing_flag=0;
	     TFT_Disp_Set_TimerTime(0);

  
       }
	}
       
}

  /*****************************************************************************
 * 
 * Function Name: void Mode_Key_Select_Fun(void)
 * Function:   This function is used to select the working mode of the device.
 * Input Ref: NO
 * Return Ref: NO
 * 
*****************************************************************************/
void Mode_Key_Select_Fun(void)
{
   switch(gctl_t.select_main_fun_numbers){

      case ptc_fun:

		
		ptc_fun_led_init_flg++;
		if(ptc_state()== 1){


		LED_PTC_ICON_OFF();//LED_PTC_ICON_ON();

		}
		else{

		LED_PTC_ICON_ON();//LED_PTC_ICON_OFF();


		}

		if(wifi_link_net_state()==1){

		LED_WIFI_ICON_ON();
		}
		else{

		LED_WIFI_ICON_OFF();

		}

   	      
   
        

  led_blik: if(plasma_state() == 1){
                   // Plasma_On();
				LED_KILL_ICON_ON();
			}
			else{
			//  Plasma_Off();
			LED_KILL_ICON_OFF();

			}

			if(ultrasonic_state()==1){

			// Ultrasonic_Pwm_Output();
			LED_RAT_ICON_ON();

			}
			else{

			// Ultrasonic_Pwm_Stop();
			LED_RAT_ICON_OFF();
		}
			
			
	    if(gctl_t.gTimer_ctl_select_led < 20){ //30x10ms=300ms
 		      LED_PTC_ICON_ON()  ;  
		  }
		  else if(gctl_t.gTimer_ctl_select_led > 19 && gctl_t.gTimer_ctl_select_led < 41){
 		     LED_PTC_ICON_OFF() ; 

		  }
		  else{
		  	gctl_t.gTimer_ctl_select_led=0;
			if(pro_t.mode_key_run_proc_item==mode_key_select)
			   goto led_blik;
			

		  }

        
		

      break;

	  case plasma_fun:
	  	//KILL ICON LED

		 plasma_fun_led_init_flag ++;

		 if(plasma_state() == 1){
                   // Plasma_On();
			LED_KILL_ICON_OFF();//LED_KILL_ICON_ON();
		}
		else{
			//  Plasma_Off();
		   LED_KILL_ICON_ON();//LED_KILL_ICON_OFF();

		}

		
	 if(wifi_link_net_state()==1){

	 LED_WIFI_ICON_ON();
	 }
	 else{

	 LED_WIFI_ICON_OFF();

	 }


		
	 led_blik2:   if(ptc_state()== 1){

	
		LED_PTC_ICON_ON();

		}
		else{
	
		LED_PTC_ICON_OFF();


		}

		if(ultrasonic_state()==1){

	
		LED_RAT_ICON_ON();

		}
		else{

		
		LED_RAT_ICON_OFF();

		}

     if(gctl_t.gTimer_ctl_select_led < 20){ //30x10ms=300ms

		    
				LED_KILL_ICON_ON() ;   
         	 }
			 else if(gctl_t.gTimer_ctl_select_led > 19 && gctl_t.gTimer_ctl_select_led < 41){
				LED_KILL_ICON_OFF() ;
			 }
			 else{
			 	gctl_t.gTimer_ctl_select_led=0;
				if(pro_t.mode_key_run_proc_item==mode_key_select)
				goto led_blik2;

			 }
	  	
		

	  break;

	  case rat_fun:

	  //ULTRSONIC ICO LED
	  rat_fun_led_init_flag++;

	  if(ultrasonic_state()==1){

	
		LED_RAT_ICON_OFF();//LED_RAT_ICON_ON();

		}
		else{
			
			LED_RAT_ICON_ON();//LED_RAT_ICON_OFF();

		}

		
      if(wifi_link_net_state()==1){

		LED_WIFI_ICON_ON();
	  }
	  else{

		LED_WIFI_ICON_OFF();

	  }
	 

	led_blink3:   if(ptc_state()== 1){
	
	   
		LED_PTC_ICON_ON();
	
	 }
	 else{
	   
	   LED_PTC_ICON_OFF();
	}
	
	if(plasma_state() == 1){
		
		  LED_KILL_ICON_ON();
	  }
	  else{
	   
		 LED_KILL_ICON_OFF();
	
	  }
	
	 


	if(gctl_t.gTimer_ctl_select_led < 20){ //30x10ms=300ms
			LED_RAT_ICON_ON(); 
	 }
	 else if(gctl_t.gTimer_ctl_select_led > 19 && gctl_t.gTimer_ctl_select_led < 41){	
		   LED_RAT_ICON_OFF();
	  }
	  else{
		   gctl_t.gTimer_ctl_select_led=0;
		   if(pro_t.mode_key_run_proc_item==mode_key_select)
		   goto led_blink3;

		}

	   
	  break;
	  	
	  	

   }

}
   
  

}

/**************************************************************************
 * 
 * Function Name: void Mode_Key_Confirm_Fun(void)
 * Function : "+" and "-" of key as confirm of key be used to 
 * Input Ref:NO
 * Return Ref:NO
 * 
**************************************************************************/
void Mode_Key_Confirm_Fun(void)
{
   switch(gctl_t.select_main_fun_numbers){

      case ptc_fun:
           if(gctl_t.memory_confimr_key_done ==1){
            if(ptc_state() == 0){
				LED_PTC_ICON_ON(); 
			    Ptc_On();
				gctl_t.ptc_flag = 1;
				
		    }
			else{
				LED_PTC_ICON_OFF() ;
				Ptc_Off();
				gctl_t.ptc_flag = 0;
				
            }

			gctl_t.select_main_fun_numbers--;
			if(gctl_t.select_main_fun_numbers==0)gctl_t.select_main_fun_numbers=4;
		    gctl_t.memory_confimr_key_done = 0;
          }
	  break;

	  case plasma_fun:
	  	   if(gctl_t.memory_confimr_key_done ==1){

		 if(plasma_state() == 0){
			gctl_t.plasma_flag=1;
			LED_KILL_ICON_ON() ;
			Plasma_On();
		}
     	else{
		  gctl_t.plasma_flag=0;
		  LED_KILL_ICON_OFF() ;
		  Plasma_Off();
		}
		gctl_t.select_main_fun_numbers--;
			if(gctl_t.select_main_fun_numbers==0)gctl_t.select_main_fun_numbers=5;
		gctl_t.memory_confimr_key_done = 0;
	  	   }
	  break;

	  case rat_fun: //ball cat-black
       if(gctl_t.memory_confimr_key_done ==1){
	   if(ultrasonic_state() ==0){ //30x10ms=300ms
	   	    gctl_t.ultrasonic_flag=1;
			LED_RAT_ICON_ON(); 
		    Ultrasonic_Pwm_Output();
			
			
	   	}
		else{	
		   gctl_t.ultrasonic_flag=0;
		   LED_RAT_ICON_OFF();
		   Ultrasonic_Pwm_Stop();
		  
				
		}
	  gctl_t.select_main_fun_numbers--;
	  if(gctl_t.select_main_fun_numbers==0)gctl_t.select_main_fun_numbers=6;
	   gctl_t.memory_confimr_key_done = 0;
       }
	   break;

	 }
   
}


/******************************************************************************
	*
	*Function Name:static void ModeKey_Select_Default_LedOnOff(void)
	*Funcion: display of TFT lcd 
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
void ModeKey_Select_Default_LedOnOff(void)
{
    if(ptc_fun_led_init_flg > 0){
	    ptc_fun_led_init_flg=0;
		if(ptc_state()== 1){


		    LED_PTC_ICON_ON();

		}
		else{

		   LED_PTC_ICON_OFF();
		}

	}

	if(plasma_fun_led_init_flag > 0){
		plasma_fun_led_init_flag =0;
		if(plasma_state() == 1){
                  
			LED_KILL_ICON_ON();
		}
		else{
			//  Plasma_Off();
		   LED_KILL_ICON_OFF();

		}
	}

	if(rat_fun_led_init_flag > 0){
		rat_fun_led_init_flag =0;
		 if(ultrasonic_state()==1){
			LED_RAT_ICON_ON();

		}
		else{
    	LED_RAT_ICON_OFF();

		}	
	}

}

