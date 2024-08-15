#include "bsp.h"


static void freertos_split_detected_wifi_handler(void);

static void freertos_split_normal_run_handler(void);



/**********************************************************************************************************
*	函 数 名: void wifi_detected_signal_handler(uint8_t blink_flag)
*	功能说明: 
*	形    参: wifi of led fast is bliking is "1" -blink detected wifi signal  "0" --don't blink
*	返 回 值: 无
*   
**********************************************************************************************************/
void wifi_detected_signal_handler(uint8_t blink_flag)
{


   switch(blink_flag){

    case 0: //don't detector wifi signal is normal run process.

        freertos_split_normal_run_handler();

    break;


    case 1: //detected wifi of signal of led is fast blink.
        
        freertos_split_detected_wifi_handler();



    break;





   }



}
/**********************************************************************************************************
*	函 数 名: void freertos_split_detected_wifi_handler(uint8_t blink_flag)
*	功能说明: 
*	形    参: wifi of led fast is bliking is "1" -blink detected wifi signal  "0" --don't blink
*	返 回 值: 无
*   
**********************************************************************************************************/
static void freertos_split_detected_wifi_handler(void)
{
    //wifi signal be detected wifi of led run faster blink. is times 120s in.
       
        RunWifi_Command_Handler();//WIFI_Process_Handler();
        direct_wifi_led_fast_blink_handler();
        Wifi_Fast_Led_Blink();
        detection_net_link_state_handler();
    
     
}
/**********************************************************************************************************
*	函 数 名: void freertos_split_normal_run_handler(uint8_t blink_flag)
*	功能说明: 
*	形    参: wifi of led fast is bliking is "1" -blink detected wifi signal  "0" --don't blink
*	返 回 值: 无
*   
**********************************************************************************************************/
static void freertos_split_normal_run_handler(void)
{
     //don't detected wifi signal and normal run process.
     PowerOn_Process_Handler();
     WIFI_LED_Blink();
     smart_phone_power_on_to_tencent_data();
      
     TimeTimer_Pro_Handler();
     Temperature_Ptc_Pro_Handler();
    
   
}



