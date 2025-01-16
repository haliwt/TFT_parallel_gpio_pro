#include "bsp.h"

PRO_T gpro_t;




//static void power_on_init_set_ref(void);




static void Power_On_Init(void);








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

  // dht11_init();
   EE_IIC_Init();
   TimerTiming_Init();
   Buzzer_Sound_Fun_Init();
   gpro_t.disp_works_timer_timing_mode_item = works_time;
   gpro_t.mode_key_run_item_step = mode_key_set_temp;

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
void PowerOn_Process_Handler(void)
{
	
  
    switch(gpro_t.run_process_step){


    case 0:

    
       gpro_t.run_process_step =1;
    
     
       gpro_t.power_off_flag = 1;
       gpro_t.disp_works_timer_timing_mode_item = works_time;
       gpro_t.gTimer_pro_disp_temphum = 0; //
    
       gpro_t.gTimer_pro_update_dht11_data=60;
       gpro_t.gTimer_read_humidity_value = 30;
       gctl_t.disp_ntc_res_switch_normal_ptc_counter  = 0 ;
       gpro_t.gTimer_run_total=0;
    
       //wifi of ref
       wifi_t.gTimer_get_beijing_time=0;
       wifi_t.get_rx_beijing_time_enable=0;
       wifi_t.gTimer_auto_detected_net_state_times=0;
       wifi_t.set_wind_speed_value=100;//gpro_t.set_wind_speed_value=10; //WT.EDIT 2025.01.10
       wifi_t.adj_speed_flag ++ ;
    
       Power_On_Init();
    
    
     //  Fan_Run(); //WT.EDIT 2024.12.024
       LED_Power_Key_On();


    break;


    case 1:


    if(gpro_t.gTimer_pro_disp_temphum > 5 && gpro_t.wifi_led_fast_blink_flag==0){
           
		   gpro_t.gTimer_pro_disp_temphum=0;
            

		    Update_DHT11_Value();
            TFT_Disp_Only_Temp_Numbers(0,gctl_t.dht11_temp_value);
           
           
       }

      gpro_t.run_process_step=2;

    break;


	

     case 2:

     if(gpro_t.gTimer_read_humidity_value > 6 && gpro_t.wifi_led_fast_blink_flag==0){

           gpro_t.gTimer_read_humidity_value=0;

            TFT_Disp_Only_Humidity_Numbers(gctl_t.dht11_hum_value);
           }

        gpro_t.run_process_step=3;
     break;


     case 3:

         if(gpro_t.gTimer_pro_update_dht11_data > 11  && wifi_link_net_state() ==1){
              gpro_t.gTimer_pro_update_dht11_data=0;
      
               Update_Dht11_Totencent_Value();
      
          }

     gpro_t.run_process_step=4;

     break;


   case 4: //02,device works two hours have a rest 10 minutes.
	
        if(gpro_t.gTimer_run_main_action > 5 && gpro_t.wifi_led_fast_blink_flag==0){
          gpro_t.gTimer_run_main_action=0;
	      RunMain_And_Interval_Handler();
          if(gpro_t.interval_stop_run_flag ==0){//WT.EDIT .2024.12.25 ENABLE FAN RUN 
             //Fan_Run(); //WT.EDIT .2024.12.25 ENABLE FAN RUN 
             fan_adj_speed_handler();
          }
        }
	   gpro_t.run_process_step=5;
	 break;

    case 5: //4
	 
       if(wifi_link_net_state() ==1){
	      LED_WIFI_ICON_ON();
		  ModeKey_Select_Default_LedOnOff();
            
		}
		

	  gpro_t.run_process_step=6;
	 break; 
		  
      // handler of wifi 
	  case 6: //7

        if(wifi_link_net_state()==1 && wifi_t.smartphone_app_power_on_flag==0 && wifi_t.link_net_tencent_data_flag ==1){ //after send publish datat to tencent .){
          
		    wifi_t.link_net_tencent_data_flag = 3;  //must is wifi_t.link_net_tencent_data_flag = 3;
		    MqttData_Publish_Update_Data();
		    HAL_Delay(200);
            
         }

         gpro_t.run_process_step=7;

	 break;

	  case 7:
		
	    if(gpro_t.gTimer_run_total > 119){ //120 minutes
              gpro_t.gTimer_run_total =0;
              gpro_t.gTimer_run_time_out=0;  //time out recoder start 10 minutes
        
              gpro_t.interval_stop_run_flag  =1 ;
             
             gpro_t.run_process_step=1;
             gpro_t.fan_run_the_first_flag ++ ;//WT.EDIT 2024.12.23
       }
       else{
         gpro_t.run_process_step=1;

       }

	   break;

    default:
    break;
   	}
 }


void wifi_app_power_on_handler(void)
{
    Fan_Run(); //WT.EDIT 2024.12.024
    power_on_init_set_ref();
    
                
                 
             if(wifi_t.smartphone_app_power_on_flag==2){
                      wifi_t.smartphone_app_power_on_flag++;
                      Device_Action_No_Wifi_Handler(); //smart phone app is power on .
    
              }
             gpro_t.fan_run_the_first_flag++;//WT.EDIT .2024.12.23
             
             gpro_t.run_process_step=0;
    
            
}

/******************************************************************************
	*
	*Function Name:void power_on_init_set_ref(void)
	*Funcion: display of TFT lcd 
	*Input Ref:NO
	*Return Ref:NO
	*
******************************************************************************/
void power_on_init_set_ref(void)
{
  
  if(wifi_link_net_state() ==1){
		    Donot_Disp_CountDown_60s();
		    TFT_Display_WorksTime_Voice();
            LED_WIFI_ICON_ON(); //WT.EDIT 2024.07.23
             
	}
   else{
           // Donot_Disp_CountDown_60s(); //WT.EDIT 2024.09.06
            TFT_Display_PowerOn_WorksTime_Init();
          
	 }

       
         if(wifi_t.smartphone_app_power_on_flag==0){
		       power_on_action_led_init();
         }
         else{

         
           LED_Mode_Key_On();
           LED_Power_Key_On();
           TFT_BACKLIGHT_ON();

         }

       do{
       
        Update_DHT11_Value(); //WT.EDIT 2024.09.06
         
        TFT_Display_PowerOn_Init_Handler();
        
         TFT_Disp_Only_Temp_Numbers(0,gctl_t.dht11_temp_value);
        
         TFT_Disp_Only_Humidity_Numbers(gctl_t.dht11_hum_value);

         }while(0);
       
      //   LED_Mode_Key_On();
	  ///   LED_Power_Key_On();
		/// TFT_BACKLIGHT_ON();

         
        

         gpro_t.disp_works_timer_timing_mode_item = works_time ;
         if(gctl_t.get_beijing_time_success == 1){

             LED_WIFI_ICON_ON(); //WT.EDIT 2024.07.23

            //gctl_t.disp_works_hours = gctl_t.save_time_hours[0] ;
            //gctl_t.disp_works_minutes  = gctl_t.save_time_hours[1]  ;
		  }
          else{

              gctl_t.disp_works_hours = 0;
              gctl_t.disp_works_minutes  = 0;
              gctl_t.gTimer_ctl_disp_works_time_second =0;
              

          }

         gctl_t.gSet_temperature_value=40;
         gpro_t.run_process_step=0;
         gctl_t.disp_ntc_res_switch_normal_ptc_counter=0;


}


/************************************************************************
	*
	*Function Name: static void Power_On_Fun(void)
	*Function : power on
	*Input Ref:NO
	*Return Ref:No
	*
************************************************************************/
static void Power_On_Init(void)
{
  //led on 
 
  //smart phone control power on 
  if(wifi_t.smartphone_app_power_on_flag==0){
	  gctl_t.ptc_flag = 1;
      gctl_t.plasma_flag = 1;
	  gctl_t.ultrasonic_flag =1;


    if(wifi_link_net_state()==1)
	    LED_WIFI_ICON_ON()  ;             

	//PTC ICON LED
	LED_PTC_ICON_ON() ;  
    Ptc_On();
    gctl_t.ptc_flag =1;


	//KILL ICON LED
	LED_KILL_ICON_ON()  ; 
    Plasma_On();
    gctl_t.plasma_flag = 1;



	//ULTRSONIC ICO LED
	LED_RAT_ICON_ON()  ; 
    Ultrasonic_Pwm_Output();
    gctl_t.ultrasonic_flag = 1;

      
  }
  else{
   //  MqttData_Publish_Update_Data();
  
     Device_Action_No_Wifi_Handler();


  }
 
   
   gctl_t.gSet_temperature_value =40;

 
   //timer timing
   gctl_t.mode_flag = works_time;
 
   gctl_t.gSet_timer_hours =0;

//works time
    if(wifi_link_net_state()==0 || gctl_t.get_beijing_time_success ==0){
		 gctl_t.disp_works_hours =0;
	     gctl_t.disp_works_minutes=0;
	     gctl_t.gTimer_ctl_disp_works_time_second=0;

     }
     else {
       gctl_t.power_on_dis_work_time =0;
       

     }
    
 }
     





