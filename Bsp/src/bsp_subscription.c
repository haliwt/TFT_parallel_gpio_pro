#include "bsp_subscription.h"
#include <string.h>
#include <stdlib.h>
#include "bsp.h"


static void smartphone_app_timer_power_on_handler(void);


uint8_t TCMQTTRCVPUB[40];

uint8_t login_cloud_counter;



//处理腾讯云下发的数据
/*******************************************************************************
   **
   *Function Name:void Receive_Data_FromCloud_Data(int type, char *str)
   *Function: receive data from tencent cloud-subscription of topic
   *Input Ref: module , str ->data
   *Return Ref:NO
   *
********************************************************************************/
void Receive_Data_FromCloud_Data(int type, char *str)
{
   uint8_t   iNameLen = 0;
    char  *p_cName = 0, *p_cPos = str;

       wifi_t.rx_data_len=strlen(str);

      if (type == JSOBJECT) {
        /* Get Key */
        p_cPos = strchr(p_cPos, '"');
        if (!p_cPos) {
            return ;
        }
        p_cName = ++p_cPos;
        p_cPos = strchr(p_cPos, '"');
        if (!p_cPos) {
            return ;
        }
        iNameLen = p_cPos - p_cName;
      wifi_t.rx_data_name_len=iNameLen;

        /* Get Value */
        p_cPos = strchr(p_cPos, ':');
      
      
       }

   
   }

 /*******************************************************************************
**
*Function Name:void Publish_Data_ToCloud(void)
*Function: dy
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscriber_Data_FromCloud_Handler(void)
{
         static uint32_t device_id;
	     uint8_t *device_massage;

         device_massage = (uint8_t *)malloc(128);
         device_id=HAL_GetUIDw0();
      
         sprintf((char *)device_massage,"AT+TCMQTTSUB=\"$thing/down/property/%s/UYIJIA01-%d\",0\r\n", PRODUCT_ID, device_id);
         HAL_UART_Transmit(&huart2, device_massage, strlen((const char *)device_massage), 5000); 
         free(device_massage);
  



   
 }
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: 
*Return Ref:NO
*
********************************************************************************/
void Subscribe_Rx_Interrupt_Handler(void)
{

  
    switch(wifi_t.rx_data_state)
      {
 

      case 0:
      
         if((wifi_t.usart2_dataBuf[0]== 'p')  ||wifi_t.usart2_dataBuf[0]=='C' || wifi_t.usart2_dataBuf[0] =='T')//hex :54 - "T" -fixed
            wifi_t.rx_data_state=1; //=1
          else{
               wifi_t.rx_counter=0;
                wifi_t.rx_data_state=0;
            }
            
         break;
      case 1: //#1
             if((wifi_t.usart2_dataBuf[0]== 'a')||wifi_t.usart2_dataBuf[0]=='M' || wifi_t.usart2_dataBuf[0] =='C')  //hex :4B - "K" -fixed
            wifi_t.rx_data_state=2; //=1
         else{
            wifi_t.rx_data_state =0;
             wifi_t.rx_counter=0;
         }
         break;
            
        case 2:
            if((wifi_t.usart2_dataBuf[0]== 'r')||wifi_t.usart2_dataBuf[0]=='E' || wifi_t.usart2_dataBuf[0] =='M')    //hex :4B - "K" -fixed
            wifi_t.rx_data_state=3; //=1
         else{
           wifi_t.rx_data_state =0;
             wifi_t.rx_counter=0;
         }
        
        break;
        
        case 3:
            if((wifi_t.usart2_dataBuf[0]== 'a')  ||wifi_t.usart2_dataBuf[0]==' ' || wifi_t.usart2_dataBuf[0] =='Q')  //hex :4B - "K" -fixed
            wifi_t.rx_data_state=4; //=1
         else{
            wifi_t.rx_data_state =0;
             wifi_t.rx_counter=0;
         }
        
        break;

      case 4:
       if((wifi_t.usart2_dataBuf[0]== 'm') ||wifi_t.usart2_dataBuf[0]=='E' || wifi_t.usart2_dataBuf[0] =='T')   //hex :4B - "K" -fixed
         wifi_t.rx_data_state=5; //=1
         else{
           wifi_t.rx_data_state=0;
            wifi_t.rx_counter=0;
         }
            
      break;

      
      case 5:
       if((wifi_t.usart2_dataBuf[0]== 's')||wifi_t.usart2_dataBuf[0]=='R' || wifi_t.usart2_dataBuf[0] =='T')    //hex :4B - "K" -fixed
         wifi_t.rx_data_state=6; //=1
         else{
           wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
         }
            
      break;

      case 6:
       if((wifi_t.usart2_dataBuf[0]== '"')||wifi_t.usart2_dataBuf[0]=='R'   || wifi_t.usart2_dataBuf[0] =='S'){  //hex :4B - "K" -fixed
         wifi_t.rx_data_state=7; //=1
    	}
		else if(wifi_t.usart2_dataBuf[0]==':' ){

             wifi_t.rx_data_state=8;
			 
		}
		else{
           wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
         }
           
      break;

       case 7:
       if((wifi_t.usart2_dataBuf[0]== ':') ||wifi_t.usart2_dataBuf[0]=='O' || wifi_t.usart2_dataBuf[0] =='T'  ) //hex :4B - "K" -fixed
         wifi_t.rx_data_state=8; //=1
         else{
           wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
         }

		
            
      break;


      case 8:
       if((wifi_t.usart2_dataBuf[0]== '{') ||wifi_t.usart2_dataBuf[0]=='R' || wifi_t.usart2_dataBuf[0] =='A'){ //hex :4B - "K" -fixed
         
         if(wifi_t.usart2_dataBuf[0]=='R') wifi_t.rx_data_state=11;
         else if(wifi_t.usart2_dataBuf[0]=='A') wifi_t.rx_data_state=12;
         else
            wifi_t.rx_data_state=9; //=1
       	}
        else{
           wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
         }
            
      break;
         
     case 9:
        
         if(wifi_t.rx_data_success==0){
		 	
            wifi_t.wifi_data[wifi_t.rx_counter] = wifi_t.usart2_dataBuf[0];
            wifi_t.rx_counter++ ;



            if(wifi_t.usart2_dataBuf[0]=='}' || wifi_t.usart2_dataBuf[0]==0x0A) //0x7D='}', 0x0A = line feed // end
            {
            wifi_t.rx_data_success=1;
            wifi_t.rx_data_state=0;
            wifi_t.received_data_from_tencent_cloud = wifi_t.rx_counter;
            wifi_t.rx_counter=0;


            }
            else 
            wifi_t.rx_data_state=9; 
         }
         else{
			wifi_t.rx_data_success=0;

            wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
			wifi_t.received_data_from_tencent_cloud =0;

         }
       
            
      break;

      case 11:

      if(wifi_t.rx_error_data_success==0){
		 	
            wifi_t.wifi_data[wifi_t.rx_counter] = wifi_t.usart2_dataBuf[0];
            wifi_t.rx_counter++ ;

            if(wifi_t.rx_counter > 4 || wifi_t.usart2_dataBuf[0]==0x0A){
                
                wifi_t.rx_error_data_success=1;
                wifi_t.rx_data_state=0;
                wifi_t.received_data_from_tencent_cloud = wifi_t.rx_counter;
                wifi_t.rx_counter=0;


            }
            else 
            wifi_t.rx_data_state=9; 
         }
         else{
			wifi_t.rx_error_data_success=0;

            wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
			wifi_t.received_data_from_tencent_cloud =0;

         }
       


      break;

      case 12:


       if(wifi_t.usart2_dataBuf[0]== 'T')   //hex :4B - "K" -fixed
         wifi_t.rx_data_state=13; //=1
         else{
           wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
         }

     break;

     case 13:


       if(wifi_t.usart2_dataBuf[0]== 'E')   //hex :4B - "K" -fixed
         wifi_t.rx_data_state=14; //=1
         else{
           wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
         }

     break;


      case 14:


       if(wifi_t.usart2_dataBuf[0]== ':')   //hex :4B - "K" -fixed
         wifi_t.rx_data_state=15; //=1
         else{
           wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
         }

     break;


     case 15:


       if(wifi_t.usart2_dataBuf[0]== '0'){   //hex :4B - "K" -fixed
            wifi_t.esp8266_login_cloud_success =0;
            gctl_t.get_beijing_time_success = 0;
            wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
        }
        else if(wifi_t.usart2_dataBuf[0]== '1'){

            wifi_t.esp8266_login_cloud_success =1;
            login_cloud_counter++;
            wifi_t.rx_data_state =0;
            wifi_t.rx_counter=0;
        }
        else{

          wifi_t.rx_data_state =0;
           wifi_t.rx_counter=0;


        }

     break;
        
    default:
    break;
    }
    

}
 
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: +TCMQTTCONN:OK
*Return Ref:NO
*
********************************************************************************/
void Wifi_Rx_InputInfo_Handler(void)
{
    
          strcpy((char *)wifi_t.data, (const char *)wifi_t.wifi_data);
          wifi_t.data_size = wifi_t.wifi_uart_counter;


		   if(wifi_t.soft_ap_config_flag==1){

               if(strstr((const char*)wifi_t.data,"+TCSAP:WIFI_CONNECT_SUCCESS")){
              		wifi_t.soft_ap_config_success=1;
					
                   
			        
					wifi_t.gTimer_auto_detected_net_state_times=0;
					wifi_t.repeat_login_tencent_cloud_init_ref=0; //init hardware 
					wifi_t.esp8266_login_cloud_success=1;
					wifi_t.linking_tencent_cloud_doing=0; //release this flag.
					wifi_t.soft_ap_config_flag=0;
               	}

			
            else{
				  if(strstr((const char*)wifi_t.data,"+TCMQTTCONN:OK")){

				     wifi_t.repeat_login_tencent_cloud_init_ref=0;
	                 wifi_t.esp8266_login_cloud_success=1;
				  
                     wifi_t.auto_link_login_tencent_cloud_flag=1;
				  //link to tencent cloud is success .
	              wifi_t.linking_tencent_cloud_doing=0; //release this flag. usart
				
				  wifi_t.soft_ap_config_flag=0;
				  wifi_t.gTimer_auto_detected_net_state_times=0;
				  
			  }
           
           }
		  }
		  else{

		     if(strstr((const char*)wifi_t.data,"+TCMQTTCONN:OK")){
			 	
                 
				  wifi_t.esp8266_login_cloud_success=1;
			      wifi_t.repeat_login_tencent_cloud_init_ref=0;
				
	              wifi_t.linking_tencent_cloud_doing=0;
				 
                   wifi_t.auto_link_login_tencent_cloud_flag=1;
		
				  wifi_t.soft_ap_config_flag=0;
				  wifi_t.gTimer_auto_detected_net_state_times=0;
			  }



		  }
       
         wifi_t.wifi_uart_counter=0;
         
        
            
}
/*******************************************************************************
    **
    *Function Name:void Tencent_Cloud_Rx_Handler(void)
    *Function: 
    *Input Ref: +TCMQTTCONN:OK
    *Return Ref:NO
    *
********************************************************************************/
void Tencent_Cloud_Rx_Handler(void)
{

   

    if(wifi_t.rx_data_success==1){
         wifi_t.rx_data_success=0;
         //wifi_t.set_beijing_time_flag =0; //WT.EDIT 2023.06.12
         wifi_t.link_beijing_times_flag =0;
		 // wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
	
     if(wifi_t.received_data_from_tencent_cloud ==0x25){ //36
	    wifi_t.received_data_from_tencent_cloud=0;
		wifi_t.get_rx_beijing_time_enable=0;
		wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
		wifi_t.response_wifi_signal_label = APP_TIMER_POWER_ON_REF;
	    __HAL_UART_CLEAR_OREFLAG(&huart2);
		strcpy((char*)TCMQTTRCVPUB,(char *)wifi_t.wifi_data);
	    
	
	}
	else{


   if(strstr((char *)wifi_t.wifi_data,"open\":0")){
		  wifi_t.response_wifi_signal_label = OPEN_OFF_ITEM;
		 
	}
	else if(strstr((char *)wifi_t.wifi_data,"open\":1")){
	   
	   wifi_t.response_wifi_signal_label = OPEN_ON_ITEM;
	}


	
	if(strstr((char *)wifi_t.wifi_data,"ptc\":0")){
            if(power_on_state() ==power_on){
			//	  gctl_t.ptc_flag=0;
	           wifi_t.response_wifi_signal_label = PTC_OFF_ITEM;
	         
             }
			
    }
    else if(strstr((char *)wifi_t.wifi_data,"ptc\":1")){
            if(power_on_state() ==power_on){
	       //   gctl_t.ptc_flag=1;
			  wifi_t.response_wifi_signal_label = PTC_ON_ITEM;
				
            }

    }
	
    if(strstr((char *)wifi_t.wifi_data,"Anion\":0")){
          if(power_on_state() ==power_on){
	      //   gctl_t.plasma_flag=0;
			wifi_t.response_wifi_signal_label = ANION_OFF_ITEM;
		    
             }
		 
    }
    else if(strstr((char *)wifi_t.wifi_data,"Anion\":1")){
            if(power_on_state() ==power_on){
         //   gctl_t.plasma_flag=1;
			wifi_t.response_wifi_signal_label = ANION_ON_ITEM;
		
            }
    }
	
    if(strstr((char *)wifi_t.wifi_data,"sonic\":0")){
            if(power_on_state() ==power_on){
           // gctl_t.ultrasonic_flag=0;
			wifi_t.response_wifi_signal_label = SONIC_OFF_ITEM;
        
                
            }
		
    }
    else if(strstr((char *)wifi_t.wifi_data,"sonic\":1")){
            if(power_on_state() ==power_on){
          //  gctl_t.ultrasonic_flag=1;
			wifi_t.response_wifi_signal_label = SONIC_ON_ITEM;
       
           }
			
    }

	
    if(strstr((char *)wifi_t.wifi_data,"state\":1")){
           if(power_on_state() ==power_on){
            gctl_t.mode_flag=works_time;
			wifi_t.response_wifi_signal_label = STATE_AI_MODEL_ITEM;
        	}
		  
    }
    else if(strstr((char *)wifi_t.wifi_data,"state\":2")){
            if(power_on_state() ==power_on){
            gctl_t.mode_flag=timer_time;
			wifi_t.response_wifi_signal_label = STATE_TIMER_MODEL_ITEM;
            }
			
    }
	
    if(strstr((char *)wifi_t.wifi_data,"temperature")){

	        if(power_on_state() ==power_on){
			wifi_t.response_wifi_signal_label = TEMPERATURE_ITEM;
            
	        }
			
    }
   if(strstr((char *)wifi_t.wifi_data,"find")){

		 if(power_on_state()==power_on){

			wifi_t.response_wifi_signal_label= FAN_ITEM;
		}
	}
 
    }
    }
    else if(wifi_t.rx_error_data_success==1){

        wifi_t.rx_error_data_success=0;

        strcpy((char*)TCMQTTRCVPUB,(char *)wifi_t.wifi_data);

        if(strstr((char *)wifi_t.wifi_data,":208")){
           
    		 wifi_t.esp8266_login_cloud_success =0;
    	}


    }
 }

/****************************************************************************
**
   *Function Name:void Json_Parse_Command_Fun(void)
   *Function: parse setment of wifi receive data from tencent 
   *Input Ref: NO
   *Return Ref:NO
   *

*****************************************************************************/
void Json_Parse_Command_Fun(void)
{

     uint8_t i;
    static uint8_t wind_hundred, wind_decade,wind_unit,temp_decade,temp_unit;
	
     

   switch(wifi_t.response_wifi_signal_label){
       
	case OPEN_OFF_ITEM:

       
		wifi_t.esp8266_login_cloud_success=1;
		App_PowerOff_Handler();
        MqttData_Publish_SetOpen(0);  
	    osDelay(200);//HAL_Delay(200);//350
            
		
	    wifi_t.gTimer_auto_detected_net_state_times=0; //don't need check wifi if has or not
		wifi_t.response_wifi_signal_label = 0xff;
        
	  break;

	  case OPEN_ON_ITEM:
   
		wifi_t.esp8266_login_cloud_success=1;
	
		MqttData_Publish_SetOpen(1);  
		osDelay(200);//HAL_Delay(200);//300
		App_PowerOn_Handler();
		
		

		gctl_t.ptc_warning =0;
		gctl_t.fan_warning =0;
		wifi_t.gTimer_auto_detected_net_state_times=0;//don't need check wifi if has or not
		wifi_t.response_wifi_signal_label = 0xff;

	  break;

	  case PTC_ON_ITEM:
	  if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
	    if(ptc_error_state() ==0){
			
		 buzzer_sound()	;
          gctl_t.manual_operation_ptc_flag  =  ptc_manual_on;
     
		 Ptc_On();
	     LED_PTC_ICON_ON();
         MqttData_Publish_SetPtc(0x01);
	  	 osDelay(100);//HAL_Delay(100);//350ms
	     gctl_t.ptc_flag=1;
		
		}
		
          
		 wifi_t.gTimer_auto_detected_net_state_times=0;//don't need check wifi if has or not
         wifi_t.response_wifi_signal_label=0xff;
	  	}
	    
	   break;

	  case PTC_OFF_ITEM:
	  	if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
		 buzzer_sound()	;
       
         gctl_t.manual_operation_ptc_flag  =  ptc_manual_off;
		 Ptc_Off();
	     LED_PTC_ICON_OFF();
		 
         MqttData_Publish_SetPtc(0);
		 osDelay(100);//HAL_Delay(100);
	     gctl_t.ptc_flag=0;

		wifi_t.response_wifi_signal_label = 0xff;
		 wifi_t.gTimer_auto_detected_net_state_times=0;
		 	
			 wifi_t.linking_tencent_cloud_doing =0;


	  	}
	  	break;

	  case ANION_OFF_ITEM: //"杀菌" //5
	  	if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
			buzzer_sound();
		    Plasma_Off();
	        LED_KILL_ICON_OFF();
			
            MqttData_Publish_SetPlasma(0);
			osDelay(100);//HAL_Delay(100);
           gctl_t.plasma_flag=0;
		   wifi_t.gTimer_auto_detected_net_state_times=0;
		   	wifi_t.linking_tencent_cloud_doing =0;
		}
       wifi_t.response_wifi_signal_label = 0xff;
		
	   break;
		
	  case ANION_ON_ITEM: //plasma 
	  	if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
			buzzer_sound();
			Plasma_On();
	        LED_KILL_ICON_ON();
            MqttData_Publish_SetPlasma(1);
		    osDelay(100);//HAL_Delay(50);//350
           gctl_t.plasma_flag=1;
		    wifi_t.gTimer_auto_detected_net_state_times=0;
			
			 wifi_t.linking_tencent_cloud_doing =0;
		   
			
		
	  	}
       
	   wifi_t.response_wifi_signal_label=0xff;
	    break;

	  case SONIC_OFF_ITEM://ultransonic off
        if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
            buzzer_sound();
			Ultrasonic_Pwm_Stop();
	        LED_RAT_ICON_OFF();
            MqttData_Publish_SetUltrasonic(0);
			HAL_Delay(50);//
            gctl_t.ultrasonic_flag=0;
			
	
		
        }
        
	   wifi_t.response_wifi_signal_label=0xff;
		 wifi_t.gTimer_auto_detected_net_state_times=0;
		 	
			 wifi_t.linking_tencent_cloud_doing =0;
	  	break;

	  case SONIC_ON_ITEM://ultransonic on
	    if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
			buzzer_sound();
			LED_RAT_ICON_ON();
			Ultrasonic_Pwm_Output();
            MqttData_Publish_SetUltrasonic(1);
			osDelay(100);//HAL_Delay(50);
            gctl_t.ultrasonic_flag=1;
		}
      
	   wifi_t.response_wifi_signal_label=0xff;
	   wifi_t.gTimer_auto_detected_net_state_times=0;
       wifi_t.linking_tencent_cloud_doing =0;
	  	break;

	  case STATE_TIMER_MODEL_ITEM: //display timer timing of value  don't AI mode
	  #if 0
	  if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
	  	    buzzer_sound();
	        gctl_t.mode_flag=timer_time;
            MqttData_Publish_SetState(2); //timer model  = 2
			osDelay(100);//HAL_Delay(50);
            //do someting
			 gpro_t.disp_works_timer_timing_mode_item = timer_time;
		     gctl_t.timer_timing_words_changed_flag++;
			 TFT_Disp_Set_TimerTime(0);
			
			
			
			
        }
       #endif 
	   wifi_t.response_wifi_signal_label = 0xff;
	   wifi_t.gTimer_auto_detected_net_state_times=0;
    
	   
	  break;
		
	  case STATE_AI_MODEL_ITEM: //display works timing --AI-mode
	   #if 0
	  	 if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
		    buzzer_sound();
		    gctl_t.mode_flag=works_time;
            MqttData_Publish_SetState(1); //beijing timing = 1
			osDelay(200);//HAL_Delay(50);
			//do something
			
			 //gpro_t.timer_mode_flag=works_time;//0
			 gpro_t.disp_works_timer_timing_mode_item = works_time;
			 gctl_t.timing_words_changed_flag++;
			 TFT_Display_WorksTime();
          
        }
        #endif 

	       wifi_t.response_wifi_signal_label = 0xff;
		  wifi_t.gTimer_auto_detected_net_state_times=0;
		  
			 wifi_t.linking_tencent_cloud_doing =0;
	  	break;

	  case TEMPERATURE_ITEM:
	   if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
		
			buzzer_sound();
            temp_decade=wifi_t.wifi_data[14]-0x30;
            temp_unit=wifi_t.wifi_data[15]-0x30;
           
            gctl_t.gSet_temperature_value = temp_decade*10 +  temp_unit;
            if(gctl_t.gSet_temperature_value > 40)   gctl_t.gSet_temperature_value=40;
            if(gctl_t.gSet_temperature_value <20 )   gctl_t.gSet_temperature_value=20;
            MqttData_Publis_SetTemp(gctl_t.gSet_temperature_value);
			osDelay(100);//HAL_Delay(100);//350
			gctl_t.gSet_temperature_value_item =disp_do_setting_ptc_value_item;
			gpro_t.gTimer_pro_temp_delay= 100;
			gpro_t.gTimer_pro_mode_key_be_select = 0;
       
             gpro_t.gTimer_pro_set_tem_value_blink=0; //temperature numbers is blink.
			 TFT_Disp_Temp_Value(0,gctl_t.gSet_temperature_value);
             osDelay(100);
		}
      wifi_t.response_wifi_signal_label = 0xff;
	    wifi_t.gTimer_auto_detected_net_state_times=0;
			
			 wifi_t.linking_tencent_cloud_doing =0;
	  break;

	  case FAN_ITEM:
	    if(power_on_state() ==power_on && ptc_error_state()==0 && fan_error_state()==0){
			buzzer_sound();

		     if(gctl_t.fan_warning ==0){

           		 wind_hundred =wifi_t.wifi_data[7]-0x30;
	       		 wind_decade=wifi_t.wifi_data[8]-0x30;
                 wind_unit = wifi_t.wifi_data[9]-0x30;
            
                if(wind_hundred ==1 && wind_decade==0 && wind_unit==0) wifi_t.set_wind_speed_value=100;
                else
                   wifi_t.set_wind_speed_value = wind_hundred*10 + wind_decade;
			
         
			MqttData_Publis_SetFan( wifi_t.set_wind_speed_value);
			osDelay(100);//HAL_Delay(50);//
    	   
          
		    }
			else{
				 wifi_t.set_wind_speed_value=0;

			    MqttData_Publis_SetFan(wifi_t.set_wind_speed_value);
				osDelay(100);//HAL_Delay(50);
				//do seomthing 


			}
            
		}
        
	 
	    wifi_t.response_wifi_signal_label = 0xff;
		 wifi_t.gTimer_auto_detected_net_state_times=0;
		 	
			 wifi_t.linking_tencent_cloud_doing =0;
	  	break;

	  case APP_TIMER_POWER_ON_REF :

	       //wifi_t.set_beijing_time_flag=0;
	       wifi_t.link_beijing_times_flag = 0;
		   wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
         
		   smartphone_app_timer_power_on_handler();

	      wifi_t.response_wifi_signal_label=0xff;
		  wifi_t.gTimer_auto_detected_net_state_times=0;
		 
		  wifi_t.linking_tencent_cloud_doing =0;

	  break;


   }


   if(wifi_t.response_wifi_signal_label==0xff){
        
        wifi_t.response_wifi_signal_label=0xf0;

		for(i=0;i<20;i++){
		   wifi_t.wifi_data[i]=0;
		   

        }
      
		
	}

 }
/***************************************************************************************
*
*Function Name:void Wifi_Rx_Beijing_Time_Handler(void)
*Function: from tencet get beijing times.
*
*
****************************************************************************************/
void Wifi_Rx_Beijing_Time_Handler(void)
{

   
    switch(wifi_t.rx_data_state)
      {
      case 0:  //#0

            
         if(wifi_t.usart2_dataBuf[0]== 0x01 || wifi_t.usart2_dataBuf[0]== 0x02 ||wifi_t.usart2_dataBuf[0]== 0x03 ||wifi_t.usart2_dataBuf[0]== 0x04 
		 	|| wifi_t.usart2_dataBuf[0]== 0x05 ||wifi_t.usart2_dataBuf[0]== 0x06 ||wifi_t.usart2_dataBuf[0]== 0x07 ||wifi_t.usart2_dataBuf[0]== 0x08 
		 	|| wifi_t.usart2_dataBuf[0]== 0x09 ||wifi_t.usart2_dataBuf[0]== 0x0a ||wifi_t.usart2_dataBuf[0]== 0x0b ||wifi_t.usart2_dataBuf[0]== 0x0c ){ 
             wifi_t.rx_data_state=1; //=1
		  }
		  else{
               wifi_t.rx_counter=0;
              // wifi_t.UART_Flag = 0;
			   wifi_t.data_size=0;
               wifi_t.wifi_uart_counter=0;              
            }
         break;

	  case 1:
	  	 if(wifi_t.usart2_dataBuf[0]== ' ')
		  	 wifi_t.rx_data_state=2; //=1
		  else{
		      wifi_t.rx_data_state=0; //=1

          }
	  	break;

      case 2:
      
           wifi_t.real_hours=wifi_t.usart2_dataBuf[0];
           wifi_t.rx_data_state=3; //=1
         break;

	  case 3:
	  	  if(wifi_t.usart2_dataBuf[0]== ':')
		  	 wifi_t.rx_data_state=4; //=1
		  else{
		      wifi_t.rx_data_state=0; //=1

          }
	  	break;
      case 4: //#1
      
            wifi_t.real_minutes = wifi_t.usart2_dataBuf[0];
            wifi_t.rx_data_state=5; //=1
       
         break;

	  case 5:
	  	 if(wifi_t.usart2_dataBuf[0]== ':')
		  	 wifi_t.rx_data_state=6; //=1
		  else{
		      wifi_t.rx_data_state=0; //=1

          }
	  break;
            
        case 6:
           wifi_t.real_seconds = wifi_t.usart2_dataBuf[0];
           wifi_t.rx_data_state=7; //=1
           
           
        
        break;

		case 7:
			if(wifi_t.usart2_dataBuf[0]== ' ')
		  	 wifi_t.rx_data_state=8; //=1
		  else{
		      wifi_t.rx_data_state=0; //=1

          }

		break;
		  
		case 8:
			 if(wifi_t.usart2_dataBuf[0]==20){
                wifi_t.get_rx_beijing_time_enable=0 ;
				wifi_t.rx_data_state=0; //=1
				  
		     }
			 
			
			
		break;
        
        

        default:
         
         
       break;
      }




}

/*****************************************************************************
	*
	*Function Name:static void smartphone_app_timer_power_on_handler(void)
	*Function:
	*Inpur Ref:
	*Retern Ref:
	*
*****************************************************************************/
static void smartphone_app_timer_power_on_handler(void)
{
   //BaseType_t xHigherPriorityTaskWoken = pdFALSE;
   static uint8_t app_step;


    if(app_step==0 ){

	   app_step=1;
    if(strstr((char *)TCMQTTRCVPUB,"open\":1")){
		wifi_t.smartphone_app_power_on_flag=1;
     }

	 if(wifi_t.smartphone_app_power_on_flag==1){

	 if(strstr((char *)TCMQTTRCVPUB,"ptc\":1")){
			
			gctl_t.ptc_flag=1;
		}
		else if(strstr((char *)TCMQTTRCVPUB,"ptc\":0")){
			
			gctl_t.ptc_flag=0;
		}

	 if(strstr((char *)TCMQTTRCVPUB,"sonic\":1")){

			 gctl_t.ultrasonic_flag=1;

		}
        else if(strstr((char *)TCMQTTRCVPUB,"sonic\":0")){

             gctl_t.ultrasonic_flag=0;

		}

		if(strstr((char *)TCMQTTRCVPUB,"Anion\":1")){
			 gctl_t.plasma_flag=1;
        }
		else if(strstr((char *)TCMQTTRCVPUB,"Anion\":0")){
			 gctl_t.plasma_flag=0;
        }

		
     
      }

     
    }
   

   

	if(app_step==1){
	    app_step=0;
		 // buzzer_sound();

    #if 0
	
      //wifi_t.link_net_tencent_data_flag =3; //has been publish and subscription of data
        wifi_t.smartphone_app_power_on_flag=1;
        MqttData_Publish_Update_Data();
       // osDelay(20);//HAL_Delay(20);
        wifi_t.smartphone_app_power_on_timer_flag=1;
        gpro_t.set_timer_timing_success=0;
        App_PowerOn_Handler();

        #endif 
        wifi_t.esp8266_login_cloud_success=1;
        wifi_t.smartphone_app_power_on_flag=1;
        wifi_t.smartphone_app_power_on_timer_flag=1;
	
		//MqttData_Publish_SetOpen(1);  
        MqttData_Publish_Update_Data();
		osDelay(200);//HAL_Delay(200);//300
		App_PowerOn_Handler();
		
		

		gctl_t.ptc_warning =0;
		gctl_t.fan_warning =0;
		wifi_t.gTimer_auto_detected_net_state_times=0;//don't need check wifi if has or not
		
      
      
     
      
     }
    
}
/*******************************************************************************
**
*Function Name:void Subscribe_Rx_IntHandler(void)
*Function: interrupt USART2 receive data fun
*Input Ref: +TCMQTTCONN:OK
*Return Ref:NO
*
********************************************************************************/
void Wifi_Rx_Link_Net_InputInfo_Handler(void)
{
    
          strcpy((char *)wifi_t.data, (const char *)wifi_t.wifi_data);
          wifi_t.data_size = wifi_t.wifi_uart_counter;


		   if(wifi_t.soft_ap_config_flag==1){

               if(strstr((const char*)wifi_t.data,"+TCSAP:WIFI_CONNECT_SUCCESS")){
              		wifi_t.soft_ap_config_success=1;
					
//                    wifi_t.auto_link_login_tencent_cloud_flag=0;
//			        
//					wifi_t.gTimer_auto_detected_net_state_times=0;
//					wifi_t.repeat_login_tencent_cloud_init_ref=0; //init hardware 
//					wifi_t.esp8266_login_cloud_success=1;
//					wifi_t.linking_tencent_cloud_doing=0; //release this flag.
//					wifi_t.soft_ap_config_flag=0;
               	}

			
            else{
				  if(strstr((const char*)wifi_t.data,"+TCMQTTCONN:OK")){

				     wifi_t.repeat_login_tencent_cloud_init_ref=0;
	                 wifi_t.esp8266_login_cloud_success=1;
				     wifi_t.auto_link_login_tencent_cloud_flag=1;
				  //link to tencent cloud is success .
	              wifi_t.linking_tencent_cloud_doing=0; //release this flag. usart
				
				  wifi_t.soft_ap_config_flag=0;
				  wifi_t.gTimer_auto_detected_net_state_times=0;
				  
			  }

				  
		   		if(strstr((char*)wifi_t.wifi_temp_data,"+CME ERROR:208")){
						
					   
						   wifi_t.esp8266_login_cloud_success =0;
						
						
						   wifi_t.gTimer_auto_detected_net_state_times=0;
						
						   wifi_t.get_rx_beijing_time_enable=0;
					   
		   
				 }
           
           }
		  }
		  else{

		     if(strstr((const char*)wifi_t.data,"+TCMQTTCONN:OK")){
			 	
                 
				  wifi_t.esp8266_login_cloud_success=1;
			      wifi_t.repeat_login_tencent_cloud_init_ref=0;
				
	              wifi_t.linking_tencent_cloud_doing=0;
				  wifi_t.auto_link_login_tencent_cloud_flag=1;
		
				  wifi_t.soft_ap_config_flag=0;
				  wifi_t.gTimer_auto_detected_net_state_times=0;
			  }

			  if(strstr((char*)wifi_t.wifi_temp_data,"+CME ERROR:208")){
				
			
				wifi_t.esp8266_login_cloud_success =0;
			
			
				wifi_t.gTimer_auto_detected_net_state_times=0;
			
				wifi_t.get_rx_beijing_time_enable=0;
			

			  }

			 


		  }
       
         wifi_t.wifi_uart_counter=0;
         
        
            
}

	
