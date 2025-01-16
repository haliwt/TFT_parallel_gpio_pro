#include "bsp.h"



/***********************************************************************************************
	*
	*Function Name:void TFT_Disp_Only_Temp_Numbers(uint8_t bc,uint8_t temp_value)
	*Function : display dht11 of sensor temperature value 
	*Input: bc= backgroud color,bc=0 ->display numbers ,bc= 1 ->don't display numbers 
	*Return: NO 
	*
***********************************************************************************************/
void TFT_Disp_Only_Temp_Numbers(uint8_t bc,uint8_t temp_value)
{

  static uint8_t temp_unit,temp_decade;

	temp_decade = temp_value /10;

   temp_unit= temp_value%10; 
  
 

	#if NORMAL_DISPLAY
   	TFT_Disp_Numbers_Pic_413(5,40,bc,temp_decade); //间隔58
   	#else 
	TFT_MainDisp_Numbers_Pic_354(5,40,bc,temp_decade);


	#endif 
	//__disable_irq();
   #if NORMAL_DISPLA
   TFT_Disp_Numbers_Pic_413(63,40,bc,temp_unit);//63 -> 60
   #else 
   TFT_MainDisp_Numbers_Pic_354(59,40,bc,temp_unit);  

   #endif 

}


/********************************************************************************************
	*
	*Function Name:void SetPtc_Disp_TempComare_Value(void)
	*Function: 
	*Input Ref:NO
	*Return Ref:NO
	*
********************************************************************************************/
void SetPtc_Disp_TempComare_Value(void)
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
                        gpro_t.gTimer_get_data_from_tencent_data=0;
    					MqttData_Publish_SetPtc(0);
    					 osDelay(100);//HAL_Delay(100);
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
                              gpro_t.gTimer_get_data_from_tencent_data =0;
    	                      MqttData_Publish_SetPtc(1);
    	                      osDelay(100);//HAL_Delay(100);
    	                  }
                         }
                         
                     }


				 }
			}
            
          }
          gpro_t.disp_temp_or_set_tmp_switch_flag = 1;
         break;


		case dsip_set_ptc_temp_value_item:

           if( gctl_t.ptc_led_blink_flag ==0){

           if(gpro_t.gTimer_pro_temp_delay > 12   && ptc_error_state()==0 ){
                   gpro_t.gTimer_pro_temp_delay =0;

                    TFT_Disp_Only_Temp_Numbers(0,gctl_t.dht11_temp_value);
                    Wifi_Fast_Led_Blink();
    	
    		  
    		  if(set_temp_value() < dht11_temp_value() || set_temp_value()      == dht11_temp_value()){//envirment temperature
    	  
                    gctl_t.ptc_flag = 0 ;//run_t.gDry = 0;
    			    Ptc_Off();
    		        LED_PTC_ICON_OFF();

                    update_down++;
                  if(update_init != update_data_flag){
                        update_init = update_data_flag;
                          
                  if(wifi_link_net_state()==1   && wifi_t.link_net_tencent_data_flag ==3){
                        gpro_t.gTimer_get_data_from_tencent_data =0;
                      MqttData_Publish_SetPtc(0);
                       osDelay(100);//HAL_Delay(100);
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
                          gpro_t.gTimer_get_data_from_tencent_data =0;
                          MqttData_Publish_SetPtc(1);
                          osDelay(100);//HAL_Delay(100);
                         }

                        }
        			    
                        }
                    }
                   

              }
             }
			gpro_t.disp_temp_or_set_tmp_switch_flag = 2; 
          break;

		   case disp_do_setting_ptc_value_item:  //ADD KEY AND DEC KEY SET TEMPERATURE VALUE 

           gpro_t.gTimer_pro_temp_delay=0; //don't display temperature sensor of value .
	       gpro_t.gTimer_pro_disp_temphum =0;
           Wifi_Fast_Led_Blink();
		   if(gpro_t.gTimer_pro_set_tem_value_blink > 1){
			
               gpro_t.gTimer_pro_set_tem_value_blink =0;
             
			  
               Wifi_Fast_Led_Blink();

               TFT_Disp_Only_Temp_Numbers(1,gctl_t.gSet_temperature_value); //don't     display number
               Wifi_Fast_Led_Blink();
               osDelay(100);//HAL_Delay(300);
               Wifi_Fast_Led_Blink();
               TFT_Disp_Only_Temp_Numbers(0,gctl_t.gSet_temperature_value); //display number
               Wifi_Fast_Led_Blink();
			   osDelay(100);
               Wifi_Fast_Led_Blink();
			   gctl_t.manual_operation_ptc_flag= 0; //
               gctl_t.set_ptc_temp_value_success = 1;
               
			
               if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
                    gpro_t.gTimer_get_data_from_tencent_data =0;
                    MqttData_Publis_SetTemp(gctl_t.gSet_temperature_value);
                     osDelay(100);//HAL_Delay(200);
                }
			   gpro_t.gTimer_pro_disp_temphum = 0;
               gpro_t.gTimer_pro_temp_delay = 100;//at once display temperature of sensor dht11 of value.
               gctl_t.gSet_temperature_value_item= dsip_set_ptc_temp_value_item;
               gctl_t.manual_operation_ptc_flag=0; //WT.EDIT 2024.07.08
                update_data_flag++;
                update_down++;
               Wifi_Fast_Led_Blink();

		   }

            gpro_t.disp_temp_or_set_tmp_switch_flag = 3;
		   break;
            

        }

    }
}














