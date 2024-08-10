#include "bsp.h"




uint8_t get_beijing_flag, beijing_step;


static void RunWifi_Command_Handler(void);





uint16_t test_counter;
uint8_t auto_link_net_flag;

/********************************************************************************
	*
	*Functin Name:void WIFI_Process_Handler(void)
	*Functin :
	*Input Ref: NO
	*Return Ref:NO
	*
********************************************************************************/
void WIFI_Process_Handler(void)
{
  	
    RunWifi_Command_Handler();

}

void smart_phone_power_on_to_tencent_data(void)
{
    if(wifi_t.net_smart_phone_power_on_flag == 1 ){
         wifi_t.net_smart_phone_power_on_flag ++ ;

          MqttData_Publish_Update_Data();
		  osDelay(200);//HAL_Delay(200);


    }
    else if(wifi_t.net_smart_phone_power_on_flag == 2 ){
         wifi_t.net_smart_phone_power_on_flag ++ ;

          MqttData_Publish_Update_Data();
		  osDelay(200);//HAL_Delay(200);


    }


}
/**********************************************************************
	*
	*Functin Name: void MainBoard_Itself_PowerOn_Fun(void)
	*Function :
	*Input Ref:  key of value
	*Return Ref: NO
	*
**********************************************************************/
void MainBoard_Self_Inspection_PowerOn_Fun(void)
{
   static uint8_t power_on_dc_power,counter;
	if(counter < 2 && wifi_link_net_state()==0){
		
      Auto_InitWifiModule_Hardware();//InitWifiModule();
      Auto_SmartPhone_TryToLink_TencentCloud();
	  wifi_t.linking_tencent_cloud_doing = 1;

		if(counter ==0){
			counter++;
          wifi_t.gTimer_auto_detected_net=0;
       }
       
    }
    if(wifi_link_net_state()==1    && power_on_dc_power ==0){
              power_on_dc_power++;
   
             counter= 5;
             
           wifi_t.linking_tencent_cloud_doing = 0;

           wifi_t.power_on_login_tencent_cloud_flag =0;
           wifi_t.link_net_tencent_data_flag=1;
          // gpro_t.power_off_flag =1;
        
          if(gpro_t.gPower_On == power_off){
		     MqttData_Publish_PowerOff_Ref();
               HAL_Delay(300);

          }
          Subscriber_Data_FromCloud_Handler();
          HAL_Delay(200);
		
	     
		 
		  
	}
 
   
}
/********************************************************************************
   *
   *Function Name: void Wifi_RunCmd(void)
   *Funciton : separately update value 
   *Input Ref:NO
   *Return Ref:NO
   *
*********************************************************************************/
static void RunWifi_Command_Handler(void)
{
  

    
 	switch(wifi_t.runCommand_order_lable){

   case wifi_link_tencent_cloud: //01

	if(power_on_state() == power_on){
        Wifi_Fast_Led_Blink();
		Wifi_SoftAP_Config_Handler();

		SmartPhone_LinkTencent_Cloud();

	}
	
	if(wifi_link_net_state()==0){
		if(wifi_t.gTimer_linking_tencent_duration < 120){
		   gctl_t.get_beijing_time_success = 0; 
          
		   wifi_t.runCommand_order_lable = wifi_link_tencent_cloud;
		}
		else if(wifi_link_net_state()==0 && wifi_t.gTimer_linking_tencent_duration >120){
		  //auto link wifi net 
		  	wifi_t.gTimer_linking_tencent_duration=0;
		    gpro_t.wifi_led_fast_blink_flag=0; //fast blink wifi of dication led
			wifi_t.esp8266_login_cloud_success =0; //link tencent cloud flag 

             wifi_t.get_rx_beijing_time_enable=0;

            wifi_t.gTimer_auto_detected_net_state_times =0; //
            wifi_t.gTimer_get_beijing_time =0;
             gpro_t.gTimer_pro_update_dht11_data=0; //WT.EDIT don't data to tencent .

			
		   
		 }
       }

       


	if(wifi_link_net_state()==1   && power_on_state() == power_on ){
        gpro_t.wifi_led_fast_blink_flag=0;
        wifi_t.get_rx_beijing_time_enable=0;
      

        wifi_t.gTimer_auto_detected_net_state_times =0;
        wifi_t.gTimer_linking_tencent_duration=0;
        wifi_t.linking_tencent_cloud_doing  =0; //receive from tencent command state .
      
        wifi_t.gTimer_get_beijing_time =0;
        wifi_t.gTimer_publish_dht11=0; 
     
        MqttData_Publish_Update_Data();//Publish_Data_ToTencent_Initial_Data();
        osDelay(200);//HAL_Delay(200);
        Subscriber_Data_FromCloud_Handler();
        osDelay(200);//HAL_Delay(200);

        wifi_t.link_net_tencent_data_flag = 1;

         wifi_t.runCommand_order_lable = wifi_subscriber_form_tencent_data;
    }
    
	break;



			
     case wifi_subscriber_form_tencent_data: //03

	

	     Subscriber_Data_FromCloud_Handler();
		 wifi_t.gTimer_publish_dht11=0;
	     wifi_t.gTimer_get_beijing_time =0;
	
		 wifi_t.runCommand_order_lable= 0xff;
	   


	break;



   
      default:

	   break;
	 
  }

  if(wifi_t.gTimer_wifi_counter_link_beijing_times > 20 && wifi_t.get_rx_beijing_time_enable==1){

      wifi_t.gTimer_wifi_counter_link_beijing_times=0;
	  wifi_t.get_rx_beijing_time_enable=0;


  }
   
  
}
 
/********************************************************************************
	*
	*Functin Name:void void wifi_get_beijint_time_handler(void)
	*Functin :
	*Input Ref: NO
	*Return Ref:NO
	*
********************************************************************************/
void wifi_get_beijint_time_handler(void)
{

    static uint8_t alternate_flag,flag_switch;

     if(wifi_t.get_rx_beijing_time_enable==0){
    
         Tencent_Cloud_Rx_Handler();
    	 Json_Parse_Command_Fun();
  
      }

    switch(get_beijing_flag){

     case 0: //WT.EDIT .2024.08.10

      if(wifi_link_net_state()==1 && gpro_t.gTimer_get_data_from_tencent_data > 9){
       
                   gpro_t.gTimer_get_data_from_tencent_data =0;
                   flag_switch++;

               
                 if(gpro_t.gPower_On == power_on){
                     LED_WIFI_ICON_ON();
                  }

                 if(flag_switch == 1){
                    Subscriber_Data_FromCloud_Handler();
                    osDelay(100);//HAL_Delay(200)
                    get_beijing_flag = 1;

                    
                 }

                 
                 if(flag_switch >  1 && gpro_t.gPower_On == power_off){
                    flag_switch=0;
                    Update_Dht11_Totencent_Value();
                    osDelay(50);//HAL_Delay(200) //WT.EDIT 2024.08.10
                    
                     get_beijing_flag = 1;

                 }
                 else{ //WT.EDIT 2024.08.10 ADD ITEM
                     if(flag_switch > 1){
                        flag_switch=0;
                         
                       get_beijing_flag = 1;
                     }

                 }
       
    }
    else
     get_beijing_flag = 1;

   break;


   case 1:

        if(wifi_t.gTimer_get_beijing_time > 100){

         wifi_t.gTimer_get_beijing_time=0;
        
          if(wifi_link_net_state()==1){

     
    		    get_beijing_flag = 2;
           
                alternate_flag++;
                wifi_t.linking_tencent_cloud_doing  =0; //receive from tencent command state .
                gpro_t.gTimer_pro_update_dht11_data =0; //disable publish to data to tencent .

            }
            else{

               get_beijing_flag = 10;
               wifi_t.linking_tencent_cloud_doing  =1; //receive from tencent command state .
             }
	
       }
       else{

            get_beijing_flag = 0;
       }

      break;


      case 2:


	     gpro_t.gTimer_pro_update_dht11_data =0; //disable publish to data to tencent .
	   

	   if(gctl_t.get_beijing_time_success == 0){
		

         get_beijing_flag = 3;
         beijing_step =0; //WT.EDIT 2024.08.10
	      
		wifi_t.gTimer_auto_detected_net_state_times =0;
	
		wifi_t.get_rx_beijing_time_enable=0;//disenable get beijing timing
		
	
       }
	   else{

          
         if(alternate_flag == 1){
		  
			wifi_t.link_beijing_times_flag =1;
			
		
			wifi_t.get_rx_beijing_time_enable=0;//disenable get beijing timing
		
		    get_beijing_flag = 3;
			beijing_step =0; //WT.EDIT 2024.08.10

        }
		else{
              alternate_flag=0;
               gpro_t.gTimer_pro_update_dht11_data=0;
			   get_beijing_flag = 6;

		}

	   }

     
    break;


     case 3:

         switch(beijing_step){

         case 0:
            //disable publish data to tencent cloud.
            gpro_t.gTimer_get_data_from_tencent_data=0;
            gpro_t.gTimer_pro_update_dht11_data =0;
            gpro_t.gTimer_read_humidity_value =0;
            gpro_t.gTimer_pro_temp_delay=0;
        
    		
    		Get_BeiJing_Time_Cmd();
    	    osDelay(20);//HAL_Delay(20); //WT.EDIT .2024.08.10//HAL_Delay(20);
    		wifi_t.gTimer_read_beijing_time=0;
    		wifi_t.gTimer_wifi_counter_link_beijing_times = 0;

            wifi_t.link_beijing_times_flag ++;
            
            beijing_step =1;

           

         break;

         case 1:
             if(wifi_t.gTimer_read_beijing_time > 0 ){//
    		
                wifi_t.gTimer_read_beijing_time=0;
                 //disable publish data to tencent cloud.
                gpro_t.gTimer_get_data_from_tencent_data=0;
                gpro_t.gTimer_pro_update_dht11_data =0;
                gpro_t.gTimer_read_humidity_value =0;
                gpro_t.gTimer_pro_temp_delay=0;
        		
        		Get_Beijing_Time();
                wifi_t.get_rx_beijing_time_enable=1; //enable beijing times
        		wifi_t.wifi_uart_counter=0;
        		osDelay(20);//HAL_Delay(20); //WT.EDIT .2024.08.10
                
        		wifi_t.gTimer_read_beijing_time=0;
                beijing_step =2;
          
            }

          break; 


          case 2:
           

            wifi_t.real_hours = (wifi_t.wifi_data[41]-0x30)*10 + wifi_t.wifi_data[42]-0x30;
            wifi_t.real_minutes =(wifi_t.wifi_data[44]-0x30)*10 + wifi_t.wifi_data[45]-0x30;
            wifi_t.real_seconds = (wifi_t.wifi_data[47]-0x30)*10 + wifi_t.wifi_data[48]-0x30;

            wifi_t.get_rx_beijing_time_enable=0; //enable beijing times

            if(wifi_t.real_hours < 25 && wifi_t.real_minutes < 61 ){
            if(wifi_t.real_hours == 0x08 && (wifi_t.real_minutes < 0x06)){
                get_beijing_flag = 0;

            }
            else{


                gctl_t.disp_works_hours = wifi_t.real_hours;    
                gctl_t.disp_works_minutes = wifi_t.real_minutes;

                gctl_t.gTimer_ctl_disp_works_time_second =  wifi_t.real_seconds;

                gctl_t.get_beijing_time_success = 1; //WT.2024.04.25

                if(gpro_t.disp_works_timer_timing_mode_item==works_time && gpro_t.gPower_On == power_on){

                TFT_Display_WorksTime_Voice();

                }
                get_beijing_flag = 0;
              }
            }
            else{

              get_beijing_flag = 0;

            }
            break;
         }
    break;


    case 6:

       gpro_t.gTimer_pro_update_dht11_data=0;
       Wifi_Link_Tencent_Net_State();
    
       wifi_t.gTimer_publish_dht11=0;
       wifi_t.gTimer_auto_detected_net_state_times=0;  

       get_beijing_flag = 0;
    
    break;

    //auto link net 

    case 10:

     if(wifi_t.gTimer_auto_detected_net_state_times > 100){

		wifi_t.gTimer_auto_detected_net_state_times=0;

    

         if(wifi_link_net_state()==0){
              get_beijing_flag = 11;
              test_counter++;
          
           
            wifi_t.linking_tencent_cloud_doing  =1; //receive from tencent command state .
            wifi_t.wifi_uart_counter=0;
           
      
           
          }
          else{

             wifi_t.linking_tencent_cloud_doing  =0; //receive from tencent command state .
             get_beijing_flag = 0;

          }
         // wifi_t.runCommand_order_lable= wifi_again_link_net_init;//06
       }

     
     if(wifi_link_net_state()==1){
          get_beijing_flag = 0;

       }
       
     break;


     case 11:
         if(wifi_link_net_state()==0 && gpro_t.wifi_led_fast_blink_flag==0){

            wifi_t.linking_tencent_cloud_doing =1;
        

            WIFI_IC_ENABLE();
       
    		at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
            HAL_Delay(1000);
            wifi_t.gTimer_auto_link_net_time =0;
            auto_link_net_flag=1;

            get_beijing_flag = 12;

         }
         else {

             get_beijing_flag = 10;


         }
        
               

     break;


     case 12:

        if(wifi_t.gTimer_auto_link_net_time > 2){


            wifi_t.gTimer_auto_link_net_time=0;
            wifi_t.wifi_uart_counter=0;
	        wifi_t.soft_ap_config_flag =0;
           // DISABLE_INT();
	        HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 0xffff);//开始连接
           /// ENABLE_INT();
            HAL_Delay(1000);
            //HAL_Delay(1000);
            auto_link_net_flag =2;
            //HAL_Delay(1000);
           
		
	    }

        if(wifi_t.gTimer_auto_link_net_time > 1 && auto_link_net_flag==2){

            auto_link_net_flag=0 ;
            get_beijing_flag = 13;
           
        }


     break;


     case 13:
       if(wifi_link_net_state()==1){
       
        

        wifi_t.linking_tencent_cloud_doing  =0; //receive from tencent command state .
		wifi_t.wifi_uart_counter=0; //clear USART2 counter is zero
		  
		

         wifi_t.linking_tencent_cloud_doing = 0;

          if(power_on_state() == power_on){
                MqttData_Publish_Update_Data();//Publish_Data_ToTencent_Initial_Data();
                osDelay(200);//HAL_Delay(200);

            }
            else if(power_on_state() == 0){

               MqttData_Publish_PowerOff_Ref();
               osDelay(200);//HAL_Delay(200);


            }
            Subscriber_Data_FromCloud_Handler();
            osDelay(200);//HAL_Delay(200);

             get_beijing_flag = 0;
		
         }
         else{

             get_beijing_flag = 10;
         }

       break;
	

	
        }

}

