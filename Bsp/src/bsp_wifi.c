#include "bsp.h"

uint8_t sub_send_power_on_times;
uint8_t get_rx_beijing_time_enable;

uint8_t step_flag;
uint8_t beijing_step;
uint8_t auto_det_flag;

uint8_t counter, power_on_det_net;

static void RunWifi_Command_Handler(void);
static void auto_repeat_link_netware_fun(void);
static void auto_repeat_init_link_net(void);
static void auto_repeat_tencnet_net(void);

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
  	//
    RunWifi_Command_Handler();
    if(wifi_t.get_rx_beijing_time_enable==0){
     Tencent_Cloud_Rx_Handler();
	 Json_Parse_Command_Fun();
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
  // static uint8_t counter, power_on_det_net;


   
	if(counter < 2 && wifi_link_net_state()==0){
		
      Auto_InitWifiModule_Hardware();//InitWifiModule();
      Auto_SmartPhone_TryToLink_TencentCloud();
	  wifi_t.linking_tencent_cloud_doing = 1;

		if(counter ==0){
			counter++;
          wifi_t.gTimer_auto_detected_net=0;
       }
       
    }
    if(wifi_link_net_state()==1    && power_on_det_net ==0){
             power_on_det_net++;
             counter++;
             
           wifi_t.linking_tencent_cloud_doing = 0;

           wifi_t.power_on_login_tencent_cloud_flag =0;
           wifi_t.link_net_tencent_data_flag=1;
          // gpro_t.power_off_flag =1;
        
          if(gpro_t.gPower_On == power_off) {
		     MqttData_Publish_PowerOff_Ref();
                HAL_Delay(200);

          }



           Subscriber_Data_FromCloud_Handler();
            HAL_Delay(200);
		  wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;
	     
		 
		  
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
  static uint8_t link_net_beijing_time_flag ;

    
 	switch(wifi_t.runCommand_order_lable){

   case wifi_link_tencent_cloud: //01

	if(power_on_state() == power_on){
     
		Wifi_SoftAP_Config_Handler();

		SmartPhone_LinkTencent_Cloud();

	}
	
	if(wifi_link_net_state()==0){
		if(wifi_t.gTimer_linking_tencent_duration < 166 ){
		   gctl_t.get_beijing_time_success = 0; 
		   wifi_t.runCommand_order_lable = wifi_link_tencent_cloud;
		}
		else if(wifi_link_net_state()==0 && wifi_t.gTimer_linking_tencent_duration >166){
		  //auto link wifi net 
		  	wifi_t.gTimer_linking_tencent_duration=0;
		    gpro_t.wifi_led_fast_blink_flag=0; //fast blink wifi of dication led
			wifi_t.esp8266_login_cloud_success =0; //link tencent cloud flag 

		
			wifi_t.gTimer_auto_detected_net_state_times=0;
		
			wifi_t.get_rx_beijing_time_enable=0;

            wifi_t.gTimer_auto_detected_net_state_times =0; //
            wifi_t.gTimer_get_beijing_time =0;

			 wifi_t.runCommand_order_lable = wifi_auto_to_link_cloud; //wifi_auto_to_link_cloud
		   
		 }
       }

       


	if(wifi_link_net_state()==1   && power_on_state() == power_on ){
        gpro_t.wifi_led_fast_blink_flag=0;
        wifi_t.get_rx_beijing_time_enable=0;
        wifi_t.runCommand_order_lable = wifi_tencent_publish_init_data;

        wifi_t.gTimer_auto_detected_net_state_times =0;
        wifi_t.gTimer_linking_tencent_duration=0;
        wifi_t.auto_link_login_tencent_cloud_flag = 1; //WT.EDIT.2024.06.13
        // MqttData_Publish_SetOpen(0x01);
        wifi_t.gTimer_get_beijing_time =0;
        wifi_t.gTimer_publish_dht11=0; 
        wifi_t.link_tencent_thefirst_times=0;
        MqttData_Publish_Update_Data();//Publish_Data_ToTencent_Initial_Data();
        osDelay(200);//HAL_Delay(200);
        Subscriber_Data_FromCloud_Handler();
        osDelay(200);//HAL_Delay(200);

        wifi_t.link_net_tencent_data_flag = 1;
    }
    
	break;


	case wifi_tencent_publish_init_data://02

       if(wifi_t.gTimer_publish_dht11 >10 && gpro_t.gPower_On == power_on ){
	 
	      // MqttData_Publish_Update_Data();//Publish_Data_ToTencent_Initial_Data();
		   wifi_t.gTimer_publish_dht11=0;
		   wifi_t.gTimer_get_beijing_time =0;
	       wifi_t.runCommand_order_lable = wifi_subscriber_form_tencent_data;

       }


    break;

			
     case wifi_subscriber_form_tencent_data: //03

	   if( wifi_t.gTimer_publish_dht11 >12 ){

	      Subscriber_Data_FromCloud_Handler();
		 wifi_t.gTimer_publish_dht11=0;
	     wifi_t.gTimer_get_beijing_time =0;
	     wifi_t.get_rx_auto_repeat_net_enable=0;
		 wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;
	   }


	break;



	case wifi_publish_update_tencent_cloud_data://06

	if(wifi_t.gTimer_get_beijing_time > 100 && wifi_t.get_rx_auto_repeat_net_enable==0){
        
            if(wifi_link_net_state()==1){

                wifi_t.gTimer_get_beijing_time=0;
    		    //wifi_t.link_beijing_times_flag =1;
    		    link_net_beijing_time_flag = 1;
                gpro_t.gTimer_pro_update_dht11_data =0; //disable publish to data to tencent .

            }
            else{
               wifi_t.gTimer_get_beijing_time=0;
               wifi_t.runCommand_order_lable=  wifi_auto_to_link_cloud;//7

               wifi_t.gTimer_auto_detected_net_state_times =290;

            }
	
	 }

	if(link_net_beijing_time_flag ==1){

       link_net_beijing_time_flag =0;
	   
       
	   wifi_t.three_times_link_beijing++ ;
       gpro_t.gTimer_pro_update_dht11_data =0; //disable publish to data to tencent .
	   

	   if(wifi_t.three_times_link_beijing >0 && gctl_t.get_beijing_time_success == 0){
		
	   //	wifi_t.set_beijing_time_flag =1;

        wifi_t.link_beijing_times_flag =1;
	      
		wifi_t.gTimer_auto_detected_net_state_times =0;
	
		wifi_t.get_rx_beijing_time_enable=0;//disenable get beijing timing
		
		wifi_t.runCommand_order_lable= wifi_get_beijing_time;
       }
	   else{
        if(wifi_t.three_times_link_beijing % 4 == 0){
			
			//wifi_t.set_beijing_time_flag =1;
			wifi_t.link_beijing_times_flag =1;
			wifi_t.gTimer_auto_detected_net_state_times =0;
		
			wifi_t.get_rx_beijing_time_enable=0;//disenable get beijing timing
		
		
			wifi_t.runCommand_order_lable= wifi_get_beijing_time;

        }
		else{
            if(wifi_t.three_times_link_beijing % 3 == 0){
			     wifi_t.runCommand_order_lable= wifi_auto_repeat_link_cloud;//09
            }
            else{

                wifi_t.runCommand_order_lable = wifi_publish_update_tencent_cloud_data;

            }

		}

	}
	}
	else{

      if(wifi_link_net_state()==1){

         wifi_t.runCommand_order_lable= wifi_auto_repeat_link_cloud;//09
       }
       else{

        wifi_t.runCommand_order_lable=  wifi_auto_to_link_cloud;
       }

	}

	if(wifi_t.real_hours < 25 && wifi_t.real_minutes < 61 && beijing_step ==3){

		beijing_step ++;

    
		
		     if(wifi_t.real_hours == 0x08 && (wifi_t.real_minutes < 0x05)){

                     


              }
              else{
			
             gctl_t.save_time_hours[0] = wifi_t.real_hours;                     
             gctl_t.disp_works_hours = gctl_t.save_time_hours[0];

             
            gctl_t.save_time_hours[1] = wifi_t.real_minutes;

			gctl_t.disp_works_minutes = gctl_t.save_time_hours[1];

            gctl_t.save_time_hours[2] =  wifi_t.real_seconds;

           gctl_t.gTimer_ctl_disp_works_time_second =   gctl_t.save_time_hours[2];

			gctl_t.get_beijing_time_success = 1; //WT.2024.04.25

            wifi_t.auto_link_login_tencent_cloud_flag = 1; //WT.EDIT.2024.06.13

            if(gpro_t.disp_works_timer_timing_mode_item==works_time && gpro_t.gPower_On == power_on){

                TFT_Display_WorksTime_Voice();

            }

                }

           Subscriber_Data_FromCloud_Handler(); //WT.EDIT 2024.06.15
           osDelay(200);
		
          wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;//06
	
	}

	

	break;

	case wifi_auto_to_link_cloud: //0x07
	
	if(wifi_t.gTimer_auto_detected_net_state_times > 180){

		wifi_t.gTimer_auto_detected_net_state_times=0;

         if(wifi_link_net_state()==0){
           auto_det_flag=1;
           auto_repeat_link_netware_fun();
         }
        else if(wifi_link_net_state()==1){

          auto_det_flag=0 ;
		wifi_t.wifi_uart_counter=0; //clear USART2 counter is zero
		  
		
		 wifi_t.get_rx_auto_repeat_net_enable=0;
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
		
         }

       

		wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;//06

	}
	else{

		wifi_t.runCommand_order_lable=wifi_publish_update_tencent_cloud_data;//06

	}


	break;

	case wifi_auto_repeat_link_cloud://09

	if(wifi_t.gTimer_counter_repeat_link_net>180){

       wifi_t.gTimer_counter_repeat_link_net=0;

	switch(wifi_t.auto_link_login_tencent_cloud_flag){


	case 1:  //link is OK
	   // Wifi_Rx_Auto_Link_Net_Handler();
	    wifi_t.runCommand_order_lable = wifi_publish_update_tencent_cloud_data; //06 
	    Wifi_Link_Tencent_Net_State();
        osDelay(100);//HAL_Delay(100);
        
		wifi_t.get_rx_auto_repeat_net_enable=0;
		wifi_t.gTimer_publish_dht11=0;
		wifi_t.gTimer_auto_detected_net_state_times=0;  

	break;

	case 0:  //don't link to tencet cloud net.
   
		auto_repeat_link_netware_fun();

        if(wifi_link_net_state() == 1){

          Subscriber_Data_FromCloud_Handler();
          osDelay(200);//HAL_Delay(200);

         }
		wifi_t.runCommand_order_lable = wifi_publish_update_tencent_cloud_data;//wifi_tencent_publish_init_data;//wifi_tencent_publish_init_data;
	    wifi_t.gTimer_auto_detected_net_state_times=0;  


	break;

	}

	}
	else
       wifi_t.runCommand_order_lable = wifi_publish_update_tencent_cloud_data;
	break;


	case wifi_get_beijing_time://8


	wifi_t.linking_tencent_cloud_doing =0;
    if(wifi_t.link_beijing_times_flag ==1){   //&& wifi_t.gTimer_beijing_time>1){
        
		
		wifi_t.link_beijing_times_flag ++;
        //disable publish data to tencent cloud.
        gpro_t.gTimer_pro_update_dht11_data =0;
        gpro_t.gTimer_read_humidity_value =0;
        gpro_t.gTimer_pro_temp_delay=0;
    
		wifi_t.get_rx_beijing_time_enable=1; //enable beijing times
		wifi_t.wifi_uart_counter=0;
		Get_BeiJing_Time_Cmd();
		HAL_Delay(100);
		wifi_t.gTimer_read_beijing_time=0;
		beijing_step =1;
		wifi_t.gTimer_wifi_counter_link_beijing_times = 0;
       

	}

	//if(wifi_t.gTimer_read_beijing_time > 2 && wifi_t.gTimer_read_beijing_time < 4){
	if(wifi_t.gTimer_read_beijing_time > 0 && beijing_step ==1){//
		beijing_step =2;

         //disable publish data to tencent cloud.
        gpro_t.gTimer_pro_update_dht11_data =0;
        gpro_t.gTimer_read_humidity_value =0;
        gpro_t.gTimer_pro_temp_delay=0;
		
		Get_Beijing_Time();
		HAL_Delay(100);
		wifi_t.gTimer_read_beijing_time=0;


	}

	if(beijing_step ==2 && wifi_t.gTimer_read_beijing_time > 2){

		
	    beijing_step =3;

		wifi_t.real_hours = (wifi_t.wifi_data[134]-0x30)*10 + wifi_t.wifi_data[135]-0x30;
		wifi_t.real_minutes =(wifi_t.wifi_data[137]-0x30)*10 + wifi_t.wifi_data[138]-0x30;
		wifi_t.real_seconds = (wifi_t.wifi_data[140]-0x30)*10 + wifi_t.wifi_data[141]-0x30;

		wifi_t.get_rx_beijing_time_enable=0; //enable beijing times

   

		//wifi_t.runCommand_order_lable=wifi_publish_update_tencent_cloud_data;
		
		
	}

	
	if(beijing_step== 3){
	    wifi_t.runCommand_order_lable=wifi_publish_update_tencent_cloud_data;
		wifi_t.get_rx_beijing_time_enable=0; //enable beijing times
	}


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
	*Functin Name:static void auto_repeat_link_netware_fun(void)
	*Functin :
	*Input Ref: NO
	*Return Ref:NO
	*
********************************************************************************/
static void auto_repeat_link_netware_fun(void)
{

  
  if(auto_det_flag==1){
        //Buzzer_KeySound();
      
		//InitWifiModule_Hardware();//InitWifiModule();
        auto_repeat_init_link_net();
        //SmartPhone_TryToLink_TencentCloud();
        auto_repeat_tencnet_net();
  }

    if(wifi_link_net_state()==1){

		  wifi_t.runCommand_order_lable= wifi_publish_update_tencent_cloud_data;//04
		  
		  auto_det_flag=0;
         // wifi_t.link_net_tencent_data_flag = 1;

            if(power_on_state() == 1 ){
                MqttData_Publish_Update_Data();//Publish_Data_ToTencent_Initial_Data();
                HAL_Delay(200);

            }
            else if(power_on_state() == 0){

               MqttData_Publish_PowerOff_Ref();
               HAL_Delay(200);


            }
            Subscriber_Data_FromCloud_Handler();
            HAL_Delay(200);
            
		  wifi_t.gTimer_auto_detected_net_state_times=0;  
	 }

	
	
   
}


void Wifi_Rx_Auto_Link_Net_Handler(void)
{

   
	   strncpy((char *)wifi_t.auto_det_data, (const char *)wifi_t.wifi_data,150);
	   wifi_t.get_rx_auto_repeat_net_enable =1;


	
	   
	   if(strstr((const char*)wifi_t.auto_det_data,"QTTSTATE:0")){

			 wifi_t.esp8266_login_cloud_success=0;
             wifi_t.gTimer_auto_detected_net_state_times=0;
		     wifi_t.auto_link_login_tencent_cloud_flag =1;
				  
	    }
		else if(strstr((const char*)wifi_t.auto_det_data,"+TCMQTTRECONNECTING")){

			 wifi_t.esp8266_login_cloud_success=0;
             wifi_t.gTimer_auto_detected_net_state_times=0;
		     wifi_t.auto_link_login_tencent_cloud_flag =1;
				  
	    }
		else if(strstr((const char*)wifi_t.auto_det_data,"ONNECTING")){

			 wifi_t.esp8266_login_cloud_success=0;
             wifi_t.gTimer_auto_detected_net_state_times=0;
		     wifi_t.auto_link_login_tencent_cloud_flag =1;
				  
	    }
		else if(strstr((const char*)wifi_t.auto_det_data,"QTTSTATE:1")){

		   wifi_t.esp8266_login_cloud_success= 1;
		   wifi_t.auto_link_login_tencent_cloud_flag =0;


       }
       
         wifi_t.wifi_uart_counter=0;
		
         wifi_t.gTimer_auto_detected_net_state_times=0;
		
    
}


static void auto_repeat_init_link_net(void)
{
	    WIFI_IC_DISABLE();
		HAL_Delay(1000);
		//HAL_Delay(1000);
		//HAL_Delay(1000);
		WIFI_IC_ENABLE();
		//at_send_data("AT+RESTORE\r\n", strlen("AT+RESTORE\r\n"));
		at_send_data("AT+RST\r\n", strlen("AT+RST\r\n"));
		HAL_Delay(1000);



}

static void auto_repeat_tencnet_net(void)
{
            wifi_t.linking_tencent_cloud_doing =1;
          
			wifi_t.wifi_uart_counter=0;
	        wifi_t.soft_ap_config_flag =0;
	        HAL_UART_Transmit(&huart2, "AT+TCMQTTCONN=1,5000,240,0,1\r\n", strlen("AT+TCMQTTCONN=1,5000,240,0,1\r\n"), 5000);//开始连接
		    HAL_Delay(1000);
			HAL_Delay(1000);
			HAL_Delay(1000);
		
		
		 


}




