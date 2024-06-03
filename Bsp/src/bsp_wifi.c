#include "bsp.h"

uint8_t sub_send_power_on_times;
uint8_t get_rx_beijing_time_enable;

uint8_t step_flag;
uint8_t beijing_step;
uint8_t auto_det_flag;


static void MainBoard_Self_Inspection_PowerOn_Fun(void);
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
  	MainBoard_Self_Inspection_PowerOn_Fun();
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
static void MainBoard_Self_Inspection_PowerOn_Fun(void)
{
   static uint8_t counter, power_on_det_net;
	if(counter < 2 && wifi_link_net_state()==0 && wifi_t.rx_error_codes_flag==0){
		

		
	  InitWifiModule_Hardware();//InitWifiModule();
      SmartPhone_TryToLink_TencentCloud();
		

		if(counter ==0){
			counter++;
          wifi_t.gTimer_wifi_pub_power_off =0;
       }
       
    }

	if(wifi_t.gTimer_wifi_pub_power_off > 12  && wifi_link_net_state()==0 && counter==1){
		counter++;
		

   }


	if(wifi_link_net_state()==1 &&   power_on_det_net   < 2){
       power_on_det_net++;

        counter++;

   
       wifi_t.runCommand_order_lable=wifi_tencent_publish_init_data; //0x01

       MqttData_Publish_Update_Data();
       HAL_Delay(200);

       Subscriber_Data_FromCloud_Handler();
       HAL_Delay(200);
       wifi_t.link_net_tencent_data_flag=1;

      

     
       

    }
    else if(gpro_t.gPower_On == power_off && (power_on_det_net > 1 && power_on_det_net <3)){

            power_on_det_net++;
            counter++;

        if(wifi_link_net_state()==1  ){
             power_on_det_net++;

            MqttData_Publish_SetOpen(0);  
    		HAL_Delay(350);//300

          MqttData_Publish_PowerOff_Ref();
           HAL_Delay(350);
           wifi_t.link_net_tencent_data_flag=1;

          }
        

       wifi_t.runCommand_order_lable=wifi_publish_update_tencent_cloud_data; //0x01
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

