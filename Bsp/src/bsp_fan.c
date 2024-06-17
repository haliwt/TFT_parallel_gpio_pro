#include "bsp_fan.h"
#include "bsp.h"

static void turn_off_fan_function(void);


void Fan_Run(void)
{
    
   FAN_CCW_SetHigh(); //brake
   FAN_COM_SetLow(); //PA6
	 

}

 
void Fan_Stop(void)
{
    FAN_CCW_SetLow(); //brake
    FAN_COM_SetLow(); //PA6
  
    
}
/********************************************************************************************
	*
	*Function Name:void Temperature_Ptc_Pro_Handler(void)
	*Function: 
	*Input Ref:NO
	*Return Ref:NO
	*
********************************************************************************************/
void RunMain_And_Interval_Handler(void)
{
   static uint8_t fan_2_hours_stop;

   if(fan_error_state()==0){
	
	switch(gpro_t.interval_stop_run_flag){

	  	case 0:
		
	      if(fan_2_hours_stop==2){ //8s
	         fan_2_hours_stop=0;
		
		     Fan_Run();
			 Device_Action_No_Wifi_Handler();
             gpro_t.gTimer_run_total =0; //repeat begin counter works timing how much? reach is 2 hours?
             
             
	      }
          if(gctl_t.gTimer_maian_run_fun >0 ){

              gctl_t.gTimer_maian_run_fun=0;
             if(gpro_t.interval_stop_run_flag==0){
                Device_Action_No_Wifi_Power_On_Handler();
             }

          }
          if(gctl_t.gTimer_ctl_fan_adc_times > 12 ){
		 	 
	        gctl_t.gTimer_ctl_fan_adc_times =0;
	        Get_Fan_Adc_Fun(ADC_CHANNEL_0,5); //Modify :2023.09.03  //Get_Fan_Adc_Fun(ADC_CHANNEL_0,10); 
	     }
		
	   break;

	   case 1:
          Device_stop_Action_Fun();
            if(fan_2_hours_stop==0){
			 	fan_2_hours_stop=1;
                gpro_t.gTimer_fan_run_one_minute=0;
              

			 }

			 if(gpro_t.gTimer_fan_run_one_minute < 61 && fan_2_hours_stop==1){

                   Fan_Run();
			 }
			 else{

				 fan_2_hours_stop=2;
				 Fan_Stop();

			 }

             if(gpro_t.gTimer_run_time_out > 10 ){ //over ten minutes ,resume main function run .

                 gpro_t.interval_stop_run_flag=0;


             }

		  
	        

	   break;
	  }

   }
   else if(fan_error_state()==1 && ptc_error_state()==0){

        if(ptc_error_state()==0){

           turn_off_fan_function();


			if(gctl_t.gTimer_ctl_warning_time < 4){
			    Display_Fan_Notworking_Words(0);
			}
			else if(gctl_t.gTimer_ctl_warning_time > 3 && gctl_t.gTimer_ctl_warning_time < 5){
			    Display_Fan_Notworking_Words(1);
			}
			else{
				gctl_t.gTimer_ctl_warning_time =0;
				Buzzer_Fan_Error_Sound();
				Voice_Warning_Sound_Fan();
			}


		}

   }

 }


/***********************************************************************************************
	*
	*Function Name:void Display_Fan_Notworking_Words(uint8_t disp)
	*Function : display of works time value 
	*Input: NO
	*Return: NO 
	*
*************************************************************************************************/
void Display_Fan_Notworking_Words(uint8_t disp)
{
    TFT_Disp_Pic_Warnign_Words(100,120,disp,4);//“风”
	TFT_Disp_Pic_Warnign_Words(136,120,disp,5);//“扇”
	TFT_Disp_Pic_Warnign_Words(172,120,disp,6);//“故”
	TFT_Disp_Pic_Warnign_Words(208,120,disp,7);//“障”
}

static void turn_off_fan_function(void)
{

   // Ptc_Off();
	LED_PTC_ICON_OFF();

	Fan_Stop();


}



