#include "bsp_ptc.h"
#include "bsp.h"

static void display_high_temp_words(uint8_t disp);
static void turn_off_ptc_function(void);

void Ptc_On(void)
{
   
   PTC_SetHigh();
   

}

void Ptc_Off(void)
{

   PTC_SetLow();

}

void Ptc_OnOff_Handler(void)
{
   if(ptc_state()== 1){

     Ptc_On();
	 HAL_Delay(200);
	 LED_PTC_ICON_ON();
     MqttData_Publish_SetPtc(0x01);  
	 HAL_Delay(30);//350

  }
  else{
    Ptc_Off();
	LED_PTC_ICON_OFF();
    MqttData_Publish_SetPtc(0); 
	HAL_Delay(30);//350


  }
}

/********************************************************************************************
	*
	*Function Name:void Temperature_Ptc_Pro_Handler(void)
	*Function: 
	*Input Ref:NO
	*Return Ref:NO
	*
********************************************************************************************/
void Temperature_Ptc_Pro_Handler(void)
{
  
   static uint8_t error_flag,error_ptc,error_fan;
   switch(gctl_t.ptc_warning){

		  case ptc_no_warning:

		   if(gctl_t.gTimer_ctl_ptc_adc_times > 9  && gctl_t.disp_ntc_res_switch_normal_ptc_counter ==0){
              gctl_t.gTimer_ctl_ptc_adc_times =0;

			    Get_PTC_Temperature_Voltage(ADC_CHANNEL_1,20); //Modify :2023.09.03 Get_PTC_Temperature_Voltage(ADC_CHANNEL_1,10);
	        
             }
             else if(gctl_t.disp_ntc_res_switch_normal_ptc_counter == 1 && gctl_t.gTimer_ctl_ptc_adc_times > 9){
                     gctl_t.gTimer_ctl_ptc_adc_times=0;

                Get_Ntc_Resistance_Temperature_Voltage(ADC_CHANNEL_1,20);

                 Judge_NTC_Temperature_Value(gctl_t.ntc_res_read_adc_value);

                    
             } 
         
             
  
		  break;

		  case ptc_waning:

          
          if(fan_error_state()==0){
				turn_off_ptc_function();
			
				if(gctl_t.gTimer_ctl_warning_time < 4){
					display_high_temp_words(0);
				}
				else if(gctl_t.gTimer_ctl_warning_time > 3 && gctl_t.gTimer_ctl_warning_time < 5){
					display_high_temp_words(1);
				}
				else{
					gctl_t.gTimer_ctl_warning_time =0;
					Buzzer_Ptc_Error_Sound();
				    Voice_Warning_Sound_Ptc();
				}
			}
			else{ //fan and ptc is both error 
			 
				turn_off_ptc_function();

			    if(error_flag == 0){
			 
					if(gctl_t.gTimer_ctl_warning_time < 4){
						
	 
	                    	Display_Fan_Notworking_Words(1); //turn off fan error words
							display_high_temp_words(0); //display ptc error words
							if(gctl_t.gTimer_ctl_warning_time==3 && error_ptc ==0){
								error_ptc++;
							   Voice_Warning_Sound_Ptc();

							}
						
					}
					else if(gctl_t.gTimer_ctl_warning_time > 3 && gctl_t.gTimer_ctl_warning_time < 5){
						
					    display_high_temp_words(1); //display ptc error words tunr off 
	                    Display_Fan_Notworking_Words(0); //turn on fan error words
	                   
				   }
					else{
						gctl_t.gTimer_ctl_warning_time =0;
						error_flag ++;
					    error_ptc=0;
					    Buzzer_Ptc_Error_Sound();
						
					    
					}
			    }
				else{

					
					if(gctl_t.gTimer_ctl_warning_time < 4){
						
	 						display_high_temp_words(1); //display ptc error words
	                    	Display_Fan_Notworking_Words(0); //turn on an error words
	                    	if(gctl_t.gTimer_ctl_warning_time==3 && error_fan==0){
								error_fan++;
	                    	  Voice_Warning_Sound_Fan();
	                    	}
							
						
					}
					else if(gctl_t.gTimer_ctl_warning_time > 3 && gctl_t.gTimer_ctl_warning_time < 5){
						
						Display_Fan_Notworking_Words(1); //turn off fan error words
						display_high_temp_words(0); //display ptc error words turn on
						
	                   
					}
					else{
						gctl_t.gTimer_ctl_warning_time =0;
						if(error_flag > 0)error_flag=0;
						error_fan=0;
						Buzzer_Fan_Error_Sound();
					
					    
					}
				}
			}

		 break;
	}

  // SetPtc_TempComare_Value();
}


/********************************************************************************************
	*
	*Function Name:void SetPtc_TempComare_Value(void)
	*Function: 
	*Input Ref:NO
	*Return Ref:NO
	*
********************************************************************************************/
void SetPtc_TempComare_Value(void)
{

static uint8_t times_counter,update_data_flag,update_down,update_init= 0xff,update_down_init=0xff;
   //set up 
   if(ptc_error_state() == 0){

	   switch(gctl_t.gSet_temperature_value_item){

         case disp_ptc_temp_value_item:

		  	if(gpro_t.gTimer_pro_temp_delay > 17){ //WT.EDIT 2023.07.27 over 40 degree shut of ptc off
                gpro_t.gTimer_pro_temp_delay=0;
                	
               TFT_Disp_Only_Temp_Numbers(0,gctl_t.dht11_temp_value); //sensor temperature value 

               
                if(dht11_temp_value() >39){//envirment temperature
					gctl_t.ptc_flag = 0 ;//run_t.gDry = 0;
					Ptc_Off();

					LED_PTC_ICON_OFF();

                    update_down++;
                    
                    times_counter = 1;

                    if(update_init != update_data_flag){
                        update_init = update_data_flag;
					if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
    					MqttData_Publish_SetPtc(0);
    					HAL_Delay(200);
					}

                    }
				}
				else if(gctl_t.manual_operation_ptc_flag ==0){
					if((dht11_temp_value() <38 || dht11_temp_value() == 38) && times_counter == 1){

                     if(wifi_t.smartphone_app_power_on_flag==0){
                          update_data_flag++;
                          gctl_t.ptc_flag = 1;
                          if(gpro_t.interval_stop_run_flag ==0){
    					  Ptc_On();
    				      LED_PTC_ICON_ON();
                          }

                         
                         if(update_down_init != update_down){
                                  update_down_init = update_down;
    	                 if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
    	                      MqttData_Publish_SetPtc(1);
    	                      HAL_Delay(200);
    	                  }
                         }
                         
                     }


				 }
			}
            
          }
          gpro_t.disp_temp_or_set_tmp_switch_flag = 1;
         break;


		   case dsip_set_ptc_temp_value_item:

           if(gpro_t.gTimer_pro_temp_delay > 12   && ptc_error_state()==0 ){
                   gpro_t.gTimer_pro_temp_delay =0;

                    TFT_Disp_Only_Temp_Numbers(0,gctl_t.dht11_temp_value);

    	
    		  
    		  if(set_temp_value() < dht11_temp_value() || set_temp_value()      == dht11_temp_value()){//envirment temperature
    	  
                    gctl_t.ptc_flag = 0 ;//run_t.gDry = 0;
    			    Ptc_Off();
    		        LED_PTC_ICON_OFF();

                    update_down++;
                  if(update_init != update_data_flag){
                        update_init = update_data_flag;
                          
                  if(wifi_link_net_state()==1   && wifi_t.link_net_tencent_data_flag ==3){
                    
                      MqttData_Publish_SetPtc(0);
                      HAL_Delay(200);
                   }

                  }

                  
                }
    			else if(set_temp_value() > dht11_temp_value() &&     gctl_t.manual_operation_ptc_flag ==0){
    	  
                    if(wifi_t.smartphone_app_power_on_flag==0){

                         update_data_flag++;

                        
                    
                        gctl_t.ptc_flag = 1;//run_t.gDry = 1;
                        if(gpro_t.interval_stop_run_flag ==0){
        		         Ptc_On();
        			     LED_PTC_ICON_ON();

                        }

                         if(update_down_init != update_down){
                            update_down_init = update_down;

                         if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
                          MqttData_Publish_SetPtc(1);
                          HAL_Delay(200);
                         }

                        }
        			    
                        }
                    }
                   

              }
			gpro_t.disp_temp_or_set_tmp_switch_flag = 2; 
          break;

		   case disp_do_setting_ptc_value_item:

           gpro_t.gTimer_pro_temp_delay=0; //don't display temperature sensor of value .
	       gpro_t.gTimer_pro_disp_temphum =0;
           
		   if(gpro_t.gTimer_pro_set_tem_value_blink > 1){
			
               gpro_t.gTimer_pro_set_tem_value_blink =0;
             
			  

               TFT_Disp_Only_Temp_Numbers(1,gctl_t.gSet_temperature_value); //don't     display number
               osDelay(300);//HAL_Delay(300);
               TFT_Disp_Only_Temp_Numbers(0,gctl_t.gSet_temperature_value); //don't     display number
			   osDelay(300);
			   gctl_t.manual_operation_ptc_flag= 0; //
               gctl_t.set_ptc_temp_value_success = 1;
               
			
               if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
                    MqttData_Publis_SetTemp(gctl_t.gSet_temperature_value);
                     osDelay(200);//HAL_Delay(200);
                }
			   gpro_t.gTimer_pro_disp_temphum = 0;
               gpro_t.gTimer_pro_temp_delay = 100;//at once display temperature of sensor dht11 of value.
               gctl_t.gSet_temperature_value_item= dsip_set_ptc_temp_value_item;
               gctl_t.manual_operation_ptc_flag=0; //WT.EDIT 2024.07.08
                update_data_flag++;
                update_down++;
               

		   }

            gpro_t.disp_temp_or_set_tmp_switch_flag = 0;
		   break;
            

        }

    }
}
/***********************************************************************************************
	*
	*Function Name:void TFT_Display_WorksTime(void)
	*Function : display of works time value 
	*Input: NO
	*Return: NO 
	*
*************************************************************************************************/
static void display_high_temp_words(uint8_t disp)
{
    //  TFT_Disp_Pic_Warnign_Words(100,188,disp,0);//“高”
	//  TFT_Disp_Pic_Warnign_Words(136,188,disp,1);//“温”
	//  TFT_Disp_Pic_Warnign_Words(172,188,disp,2);//“警”
	//  TFT_Disp_Pic_Warnign_Words(208,188,disp,3);//“告”

      TFT_Disp_Pic_Warnign_Words(100,120,disp,0);//“高”
	  TFT_Disp_Pic_Warnign_Words(136,120,disp,1);//“温”
	  TFT_Disp_Pic_Warnign_Words(172,120,disp,2);//“警”
	  TFT_Disp_Pic_Warnign_Words(208,120,disp,3);//“告”
}

static void turn_off_ptc_function(void)
{

    Ptc_Off();
	LED_PTC_ICON_OFF(); 
	if(fan_error_state()==1){
           Fan_Stop();
	}


}

