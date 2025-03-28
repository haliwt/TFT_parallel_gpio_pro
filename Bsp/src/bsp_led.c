#include "bsp_led.h"
#include "bsp.h"




//static uint8_t  Delay(int16_t count);

volatile uint32_t led_k,led_i;


void LED_Power_Key_On(void)
{
    LED_POWER_KEY_SetLow() ;


}
void LED_Power_Key_Off(void)
{
	LED_POWER_KEY_SetHigh() ;
}

//mos 
void LED_Mode_Key_On(void)
{
  LED_MODE_KEY_SetHigh();	

}
void LED_Mode_Key_Off(void)
{

	LED_MODE_KEY_SetLow();


}
//LED_NET

void LED_Net_On(void)
{
   LED_WIFI_ICON_ON();

}
void LED_Net_Off(void)
{
	LED_WIFI_ICON_OFF() ;
	
}



#if 0
static uint8_t  Delay(int16_t count)
{
   
    if(count ==0 || count <0){
       return 1;
    
    }
    else{
    while(count){

       count--;
   }
   }

   return 0;

}
#endif 

void power_on_action_led_init(void)
{

  
	 gctl_t.ptc_flag =1;
     gctl_t.plasma_flag = 1;
     gctl_t.ultrasonic_flag = 1;
   


	LED_PTC_ICON_ON() ;  
    
    LED_KILL_ICON_ON()  ; 
    
     LED_RAT_ICON_ON()  ; 
     LED_Mode_Key_On();
     LED_Power_Key_On();
     TFT_BACKLIGHT_ON();

      Ptc_On();
      Plasma_On();
      Ultrasonic_Pwm_Output();


}

void Power_Off_Led(void)
{


	LED_WIFI_ICON_OFF()	;			  

	LED_PTC_ICON_OFF() ; 	
    
    LED_KILL_ICON_OFF()	;
    
   LED_RAT_ICON_OFF()  ;
   LED_Mode_Key_Off(); 


     gctl_t.plasma_flag = 0;

     gctl_t.ptc_flag =0;
   
    gctl_t.ultrasonic_flag = 0;

    Ptc_Off();
    Plasma_Off();
    Ultrasonic_Pwm_Stop();




}

/***************************************************************
*
*Function Name: void LED_TempHum(void)
*Function : display temperature and humidity and times led 
*
*LED_Power_Key_On();
*
**************************************************************/
void Breath_Led(void)
{
  static uint8_t breath_led;
   if(gpro_t.gTimer_breath_led  > 0){
     gpro_t.gTimer_breath_led=0;
     breath_led++;
     if(breath_led ==1){
       LED_Power_Key_On();
     }
     else{
       breath_led=0;
       LED_Power_Key_Off();
      }

    }
}


    #if 0
     static uint8_t flag,switch_flag,dec_led_flag;
        static uint8_t i,j;
        led_k++;
        
        
        if(led_k <25 && dec_led_flag==1){
            i++;
         if(switch_flag ==1 || i > 4){
            switch_flag =0;
          
            //LED_POWER_OFF();
            LED_Power_Key_Off();
            flag = Delay(25-i);
           
          }
          if(flag ==1){
            flag =0;
            LED_Power_Key_On();
             j=i;
             if(j< 5){
               switch_flag = Delay(j);
    
             }
             switch_flag =1;
          
    
            }
          
        }
        else if(led_k < 25 && dec_led_flag==0){
            i++;
         if(switch_flag ==1 || i < 10){
            switch_flag =0;
        #if 0
            LED_POWER_OFF();
            LED_POWER_OFF();
            LED_Power_On();
            flag = Delay(30-i);
        #endif 
            LED_Power_Key_On();;
            flag = Delay(25-i);
            
    
          }
          if(flag ==1){
            flag =0;
          
               if(j< 25){
               //LED_POWER_OFF();

                LED_Power_Key_Off();
               switch_flag = Delay(i);
    
                }
                else{
                // LED_POWER_OFF();
                 //LED_POWER_OFF();
                 //LED_POWER_OFF();
                /// LED_POWER_OFF();

                 LED_Power_Key_Off();
                 LED_Power_Key_Off();
                 LED_Power_Key_Off();
                 LED_Power_Key_Off();
                 switch_flag = 1;
    
    
                }
            }
         }
        
        if(led_k > 25 && dec_led_flag==0){
    
     
        
            led_k=0;
             i=0;
          
             dec_led_flag=1;
            
         //  }
            
          }
          else if(led_k>25 && dec_led_flag==1){
            
         
           
            led_k=0;
             i=0;
        
             dec_led_flag=0;
            
        }
   #endif 






