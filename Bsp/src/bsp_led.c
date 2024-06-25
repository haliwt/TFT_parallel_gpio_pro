#include "bsp_led.h"
#include "bsp.h"




static uint8_t  Delay(int16_t count);

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


void Power_On_Led_Init(void)
{

  
	//WIFI ICON LED
	LED_WIFI_ICON_ON()  ;             

	//PTC ICON LED
	LED_PTC_ICON_ON() ;               


	//KILL ICON LED
	LED_KILL_ICON_ON()  ;             


	//ULTRSONIC ICO LED
	LED_RAT_ICON_ON()  ;           


}

void Power_Off_Led(void)
{

//WIFI ICON LED
	LED_WIFI_ICON_OFF()	;			  

	//PTC ICON LED
	LED_PTC_ICON_OFF() ; 			  


	//KILL ICON LED
	LED_KILL_ICON_OFF()	;			  


	//ULTRSONIC ICO LED
	LED_RAT_ICON_OFF()  ;		   




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

}





