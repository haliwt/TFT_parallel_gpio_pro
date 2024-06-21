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
	       wifi_t.runCommand_order_lable = wifi_get_beijing_timing_data;
      }
      }



}

/***************************************************************************************
 * 
 * Function Name : void GetBeijing_Time(void)
 * Function: get beijing timing
 * Input Ref :NO
 * Return Ref:NO
 * 
****************************************************************************************/
void  GetBeijing_Time_Fun(void)
{
    static uint8_t link_net_beijing_time_flag,beijing_step;

    if(wifi_t.gTimer_get_beijing_time > 100 && wifi_t.get_rx_auto_repeat_net_enable==0){
        
            if(wifi_link_net_state()==1){

                wifi_t.gTimer_get_beijing_time=0;
    		     link_net_beijing_time_flag = 1;
               beijing_step =0;
                gpro_t.gTimer_pro_update_dht11_data =0; //disable publish to data to tencent .

            }
            else{
                link_net_beijing_time_flag = 0;
               wifi_t.gTimer_get_beijing_time=0;
               wifi_t.runCommand_order_lable=  wifi_auto_to_link_cloud;//7

               wifi_t.gTimer_auto_detected_net_state_times =290;
               return ;
            }
	
	 }

     switch(link_net_beijing_time_flag){

      case 1:
         link_net_beijing_time_flag ++;
	   
       
	   wifi_t.three_times_link_beijing++ ;
       gpro_t.gTimer_pro_update_dht11_data =0; //disable publish to data to tencent .
	   

	   if(wifi_t.three_times_link_beijing >0 && gctl_t.get_beijing_time_success == 0){
		
	

          wifi_t.link_beijing_times_flag =1;
	      
		wifi_t.gTimer_auto_detected_net_state_times =0;
	
		wifi_t.get_rx_beijing_time_enable=0;//disenable get beijing timing
		
		wifi_t.runCommand_order_lable= wifi_get_beijing_time;
     }
	else{
          if(wifi_t.three_times_link_beijing % 2 == 0){
                    
               
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

                         wifi_t.runCommand_order_lable = wifi_get_beijing_timing_data;

                    }

               }

	}
	
     break;

     case 2:
       
     if(wifi_t.link_beijing_times_flag ==1 ){

     switch(beijing_step){

      case 0:
       //disable publish data to tencent cloud.
        gpro_t.gTimer_pro_update_dht11_data =0;
        gpro_t.gTimer_read_humidity_value =0;
        gpro_t.gTimer_pro_temp_delay=0;
    
		wifi_t.get_rx_beijing_time_enable=1; //enable beijing times
		wifi_t.wifi_uart_counter=0;
		Get_BeiJing_Time_Cmd();
		HAL_Delay(100);
        
		wifi_t.gTimer_read_beijing_time=0;
          wifi_t.gTimer_wifi_counter_link_beijing_times = 0;
          beijing_step =1;

     break;

     case 1:

      //if(wifi_t.gTimer_read_beijing_time > 2 && wifi_t.gTimer_read_beijing_time < 4){
	if(wifi_t.gTimer_read_beijing_time > 0 ){
		

         //disable publish data to tencent cloud.
        gpro_t.gTimer_pro_update_dht11_data =0;
        gpro_t.gTimer_read_humidity_value =0;
        gpro_t.gTimer_pro_temp_delay=0;
		
		Get_Beijing_Time();
		HAL_Delay(100);
		wifi_t.gTimer_read_beijing_time=0;
          beijing_step =2;


     }

     break;

     case 2:

	if( wifi_t.gTimer_read_beijing_time > 1){

		wifi_t.real_hours = (wifi_t.wifi_data[134]-0x30)*10 + wifi_t.wifi_data[135]-0x30;
		wifi_t.real_minutes =(wifi_t.wifi_data[137]-0x30)*10 + wifi_t.wifi_data[138]-0x30;
		wifi_t.real_seconds = (wifi_t.wifi_data[140]-0x30)*10 + wifi_t.wifi_data[141]-0x30;

		wifi_t.get_rx_beijing_time_enable=0; //enable beijing times



          if(wifi_t.real_hours < 25 && wifi_t.real_minutes < 61 ){
             
             if(wifi_t.real_hours == 0x08 && wifi_t.real_minutes ==0x01){


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

           Subscriber_Data_FromCloud_Handler(); //WT.EDIT 2024.06.15
           HAL_Delay(100);//osDelay(200);

          
		
          }
		
            beijing_step =3;
          }
          else{
           beijing_step =3;

          }
     

         } //endif  if(wifi_t.gTimer_read_beijing_time > 1)
          break; //case 2 - break;
       }
      
	}//endif 

     break;



	
     }

}

