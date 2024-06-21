#include "bsp_wifi_fun.h"
#include "bsp.h"



WIFI_FUN   wifi_t;


uint8_t (*wifi_link_net_state)(void); //函数指针


static uint8_t wifi_link_default_fun(void);



/****************************************************************
     * 
     * Function Name:void Wifi_Init(void)
     * Function: 
     * Input Ref: NO
     * Return Ref:NO
     * 
****************************************************************/
void Wifi_Init(void)
{
  Wifi_Link_Net_Handler(wifi_link_default_fun);


}

/***************************************************************************************
     * 
     * Function Name:void Wifi_Link_Net_Handler(uint8_t (*wifi_link_handler)(void))
     * Function: 
     * Input Ref: NO
     * Return Ref:NO
     * 
***************************************************************************************/
void Wifi_Link_Net_Handler(uint8_t (*wifi_link_handler)(void))
{
    wifi_link_net_state = wifi_link_handler;
}


static uint8_t wifi_link_default_fun(void)
{

    if(wifi_t.esp8266_login_cloud_success ==1)return 1;
	else return 0;


}
/***************************************************************************************
     * 
     * Function Name:void Wifi_Pro_Runing_Init(void)
     * Function: 
     * Input Ref: NO
     * Return Ref:NO
     * 
***************************************************************************************/
void Wifi_Pro_Runing_Init(void)
{
	
	static uint8_t update_step ;
	
	if(wifi_link_net_state() ==1 && wifi_t.smartphone_app_power_on_flag==0){
	   if(wifi_link_net_state() ==1 && wifi_t.repeat_login_tencent_cloud_init_ref ==0 ){
	   	  wifi_t.repeat_login_tencent_cloud_init_ref ++;
		  update_step =1;
	      MqttData_Publish_Init();
	      wifi_t.gTimer_main_pro_times=0;

	   }
 
	     
	   if(wifi_link_net_state() ==1 && update_step==1 &&  wifi_t.gTimer_main_pro_times > 0){
	   	 
	   	  update_step ++ ;

		   MqttData_Publish_SetOpen(0x01);
	       wifi_t.gTimer_main_pro_times=0;
	   
	   }

	   if(wifi_link_net_state() ==1 && update_step==2 &&  wifi_t.gTimer_main_pro_times > 0){
	  	  update_step++;
    	  Publish_Data_ToTencent_Initial_Data();
	       wifi_t.gTimer_main_pro_times=0;
        
       }

       if(wifi_link_net_state() ==1 && update_step==3 &&  wifi_t.gTimer_main_pro_times > 0){
	  	  update_step++;
	  	
    	  Subscriber_Data_FromCloud_Handler();

		    wifi_t.gTimer_main_pro_times=0;
	     
        
       }

	   
      if(wifi_link_net_state() ==1 && update_step==4 &&  wifi_t.gTimer_main_pro_times > 0){
	  	  update_step++;
	       gpro_t.run_process_step=pro_disp_dht11_value;
	       wifi_t.runCommand_order_lable = wifi_publish_update_tencent_cloud_data;
      }
      }



}


void GetBeijing_Time(void)
{



}

