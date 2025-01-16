#include "bsp.h"


/**********************************************************************************************************
    **
    *Function Name:void Power_Key_Detected(void)
    *Function : 
    *Input Ref:lightNum--LED ,filterNum -filter number, unionNum - smart menu number
    *Return Ref:NO
    *
*********************************************************************************************************/
void Wifi_Fast_Led_Blink(void)
{

  static uint8_t wifi_fast_blink;
  if(gpro_t.gPower_On == power_on){
  if(gpro_t.wifi_led_fast_blink_flag==1 && wifi_link_net_state()==0){
  

       if(gpro_t.gTimer_pro_wifi_fast_led  > 100 ){ //50ms
         gpro_t.gTimer_pro_wifi_fast_led=0;
         // osDelay(50); //WT.EDIT 2024.12.25
          wifi_fast_blink ++ ;
          if(wifi_fast_blink ==1){

	         LED_WIFI_ICON_ON();
             Mode_Led_InWifi_Blink_On_Fun();
             
          }
          else{
             wifi_fast_blink =0 ;
             LED_WIFI_ICON_OFF();
             Mode_Led_InWifi_Blink_Off_Fun();
             
          }
            
	 }
   }

  if(wifi_t.gTimer_linking_tencent_duration > 120 && gpro_t.wifi_led_fast_blink_flag==1 && wifi_link_net_state()==0){
      gpro_t.wifi_led_fast_blink_flag=0; 

   }

   }
 
}

void direct_wifi_led_fast_blink_handler(void)
{

  if(gpro_t.gPower_On == power_on){
  if(gpro_t.wifi_led_fast_blink_flag==1){
    
        LED_WIFI_ICON_OFF();
        osDelay(50);

        LED_WIFI_ICON_ON();
        osDelay(50); 
        LED_WIFI_ICON_OFF();
	     osDelay(50);   
        LED_WIFI_ICON_ON();
        osDelay(50); 
        LED_WIFI_ICON_OFF();
        osDelay(50);   
        LED_WIFI_ICON_ON();
	  }
}
}




/**********************************************************************************************************
    **
    *Function Name:void Power_Key_Detected(void)
    *Function : 
    *Input Ref:lightNum--LED ,filterNum -filter number, unionNum - smart menu number
    *Return Ref:NO
    *
*********************************************************************************************************/
void wifi_led_slowly_blink_handler(void)
{

   static uint8_t wifi_led_slowly_blink_handler;
   if(wifi_link_net_state()==0 && gpro_t.gPower_On == power_on && gpro_t.wifi_led_fast_blink_flag==0){
       if(gpro_t.mode_key_run_item_step != mode_key_select){
                    
        if(gpro_t.gTimer_pro_wifi_led > 1){
            
             gpro_t.gTimer_pro_wifi_led=0;
             wifi_led_slowly_blink_handler ++;   //WT.EDIT 2024.12.24
             if(wifi_led_slowly_blink_handler == 1){
                LED_WIFI_ICON_ON();
             }
             else{
                 wifi_led_slowly_blink_handler =0;

                 LED_WIFI_ICON_OFF();


             }
        }
      
    }
   }

}




