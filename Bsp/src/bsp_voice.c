#include "bsp_voice.h"
#include "bsp.h"
#include <string.h>


#define HELLO     							0x01 
#define HELLO_CHECK         				0x21 

#define POWER_ON_DATA      					0x02
#define POWER_ON_DATA_CHECK        			0x22

#define POWER_OFF_DATA                      0x3
#define POWER_OFF_DATA_CHECK                0x23

#define LINK_NET_DATA                      0x4
#define LINK_NET_DATA_CHECK                0x24

#define OPEN_PTC_DATA                      0x5
#define OPEN_PTC_DATA_CHECK                0x25

#define CLOSE_PTC_DATA                      0x6
#define CLOSE_PTC_DATA_CHECK                0x26

#define OPEN_PLASMA_DATA                      0x7
#define OPEN_PLASMA_DATA_CHECK                0x27

#define CLOSE_PLASMA_DATA                      0x8
#define CLOSE_PLASMA_DATA_CHECK                0x28

#define OPEN_RAT_DATA                      0x9
#define OPEN_RAT_DATA_CHECK                0x29

#define CLOSE_RAT_DATA                      0x0A
#define CLOSE_RAT_DATA_CHECK                0x2A

//set temperature of voice command 

#define MAX_BUFFER_SIZE  8

         

voice_sound_t v_t;
uint8_t input_set_timer_timing_flag;

static uint8_t transferSize;
static uint8_t outputBuf[MAX_BUFFER_SIZE];

void (*rx_voice_data)(uint8_t data);

static void sendData_VoiceSound_Warning_Ptc(void);
static void sendData_VoiceSound_Warning_Fan(void);
static void voice_send_turn_on_power_on_cmd(void);
static void voice_send_function_cmd(uint8_t cmd1,uint8_t cmd2);
static void send_tx_set_temp_data(uint8_t temp);
static void  send_tx_set_timer_value(uint8_t set_temp);



uint8_t key;
int8_t result = 0xff;
#if 0

static uint8_t const voice_ctl_value[9]={
	0x24,0x26,0x28,0x2a,0x2c,
	0x2e,0x30,0x32,0x34
	

};



static uint8_t const voice_temp_value[21]={
	0x36,0x38,0x3a,0x3c,0x3e,
	0x40,0x42,0x44,0x46,0x48,
	0x4a,0x4c,0x4e,0x50,0x52,
	0x54,0x56,0x58,0x5a,0x5c,
	0x5e

};

static uint8_t const voice_timer_array[24]={
	0x60,0x62,0x64,0x66,0x68,
	0x6a,0x6c,0x6e,0x70,0x72,
	0x74,0x76,0x78,0x7a,0x7c,
	0x7e,0x80,0x82,0x84,0x86,
	0x88,0x8a,0x8c,0x8e
	
};

#endif 
/*
   0x28 - link wifi net
   0x2a -dry on 
   0x2c -dry on
   0x2e - dry off
   0x30 - dry off

   0x32 - kill on
   0x34 - kill on
   0x36 -- kill off
   0x38 ---kill off

   0x3a -- rat on
   0x3c -- rat on
   0x3e -- rat off
   0x40 -- rat off
   
   

*/
//算法是:data4 + data6 = element
static uint8_t const voice_sound_data[63]={

	0x22,0x24,0x26,0x28,0x2a, //0x24 = power_on ,0x26= power_off
	0x2c,0x2e,0x30,0x32,0x34,
    0x36,0x38,0x3a,0x3c,0x3e,
	0x40,
	//温度设置20~40度（16~36）
	0x42,0x44,0x46,0x48,0x4a,
	0x4c,0x4e,0x50,0x52,0x54,
	0x56,0x58,0x5a,0x5c,0x5e,
    0x60,0x62,0x64,0x66,0x68,
	0x6a,
    //定时时间设置(37 ~ 61)
	0x6c,0x6e,0x70,0x72,0x74,
	0x76,0x78,0x7a,0x7c,0x7e,
	0x80,0x82,0x84,0x86,0x88,
	0x8a,0x8c,0x8e,0x90,0x92,
	0x94,0x96,0x98,0x9a,0x9c,
	0x9e

};


//static const uint8_t voice_sound_set_temp[]={

//	    0x42,0x44,0x46,0x48,0x4a,
//		0x4c,0x4e,0x50,0x52,0x54,
//		0x56,0x58,0x5a,0x5c,0x5e,
//		0x60,0x62,0x64,0x66,0x68,
//		0x6a,

//};




static int8_t BinarySearch_Voice_Data(const uint8_t *pta,uint8_t key);

static void voice_cmd_fun(uint8_t cmd);
static void  voice_set_temperature_value(uint8_t value);
static void voice_set_timer_timing_value(uint8_t time);

static void voice_send_has_been_power_on_cmd(void);
static void voice_send_power_on_cmd(void);

static void voice_send_has_been_power_off_cmd(void); 

static void voice_send_power_off_cmd(void);


static void voice_cancel_timer_timing(void);




uint8_t key;
int8_t result;

//uint8_t voice_cmd_flag;


/***********************************************************
 *  *
    *Function Name: void Rx_Voice_Data_Handler(void(*rx_voice_handler)(uint8_t data))
    *Function: 
    *Input Ref: function of pointer o
    *Return Ref:  NO
    * 
***********************************************************/
void Rx_Voice_Data_Handler(void(*rx_voice_handler)(uint8_t data))
{

   rx_voice_data = rx_voice_handler;

}

/***********************************************************
 *  *
    *Function Name: void Voice_Decoder_Handler(void);
    *Function: decoder to command 
    *Input Ref: NO
    *Return Ref:  NO
    * 
***********************************************************/
void Voice_Decoder_Handler(void)
{

    // if(v_t.gTimer_voice_time_counter_start< 15){
	       key= v_t.voice_data[0] + v_t.voice_data[1]; //key= data4+ data6 = ; //A5 FA 00 81 01 00 21 FB 
	    
	
			result = BinarySearch_Voice_Data(voice_sound_data,key);
			
	
	    if(result < 0x10 ){
		   voice_cmd_fun(result);
		
	
		}
		else if(result > 15 && result < 37){ //set temperature value 
			   
			   voice_set_temperature_value(result);
		
	   }
	   else if(result > 36 && result <62){ //set timer timing value 
		
	
			voice_set_timer_timing_value(result);
			
		 }
	     else if(result==62){

			
            voice_cancel_timer_timing();

		 }

	   

	   


}


/***********************************************************************************
 *  *
    *Function Name: static void voice_cmd_fun(uint8_t cmd)
    *Function: voice of command 
    *Input Ref: input command 
    *Return Ref:  NO
    * 
*************************************************************************************/
static void voice_cmd_fun(uint8_t cmd)
{
	
	switch(cmd){

    case voice_power_on:
      if(gpro_t.gPower_On == power_on){
          voice_send_has_been_power_on_cmd();

	  }
	  else{
	  	 // gpro_t.gPower_On = power_on;

		  voice_send_power_on_cmd();
         
          App_PowerOn_Handler();
          
	  }



	break;

	case voice_power_off:

	if(gpro_t.gPower_On == power_on){
		gpro_t.gPower_On = power_off;


        voice_send_power_off_cmd();
      
        App_PowerOff_Handler();
		
		
    }
   else{
			  
	  voice_send_has_been_power_off_cmd(); 
	}

	break;
	


	case voice_link_wifi:

	  if(gpro_t.gPower_On == power_on){
		voice_send_function_cmd(0x04,0xA6);
		
		if(wifi_link_net_state()==0){
		    
		    wifi_t.link_tencent_step_counter=0;
			wifi_t.esp8266_login_cloud_success =0;
			gpro_t.wifi_led_fast_blink_flag=1 ;
			wifi_t.runCommand_order_lable=wifi_link_tencent_cloud;
			wifi_t.wifi_config_net_lable= wifi_set_restor;
			wifi_t.gTimer_login_tencent_times=0;
			wifi_t.gTimer_linking_tencent_duration=0; //166s -2分7秒
           
		}
		
		
		
	   }
	   else{

           voice_send_turn_on_power_on_cmd();

	   }

	
	break;


		
	case voice_open_ptc:

	 if(gpro_t.gPower_On == power_on){

	 voice_send_function_cmd(0x05,0xA7);

     if(ptc_state()==0){
      // buzzer_sound();
   
		gctl_t.ptc_flag =1;
		Ptc_On();
	    //gpro_t.add_or_dec_is_cofirm_key_flag=1;//logic is open compare temperature value WT.EDIT.2024.04.20
	    LED_PTC_ICON_ON();
		 gpro_t.gTimer_pro_temp_delay= 70;
	
         if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
              MqttData_Publish_SetPtc(1);
              HAL_Delay(200);
            }

         

	     }
	 }
	 else{
		 voice_send_turn_on_power_on_cmd();


	 }
	
     
   
    break;

	case voice_open_ptc_2 :
	if(gpro_t.gPower_On == power_on){
	
		voice_send_function_cmd(0x06,0xA8);
	
		if(ptc_state()==0){
		
	   
		   gctl_t.ptc_flag =1;
		   Ptc_On();
		
		   LED_PTC_ICON_ON();
		    gpro_t.gTimer_pro_temp_delay= 70;
		
            
            if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
                MqttData_Publish_SetPtc(1);
                HAL_Delay(200);
             }
    	
    		}
		}
		else{
			voice_send_turn_on_power_on_cmd();
	
	
		}
	   

	

	break;

	case voice_close_ptc:

	    if(gpro_t.gPower_On == power_on){

		voice_send_function_cmd(0x07,0xA9);
	
		 if(ptc_state() == 1){
     
			gctl_t.ptc_flag =0;
 
			Ptc_Off();
	
		    LED_PTC_ICON_OFF();

             if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
                MqttData_Publish_SetPtc(0);
                HAL_Delay(200);
             }
		 }
	    }
		else{

           voice_send_turn_on_power_on_cmd();

	   }
		
		
	break;

	case voice_close_ptc_2:

	    if(gpro_t.gPower_On == power_on){

		voice_send_function_cmd(0x08,0xAA);
	
		 if(ptc_state() == 1){
        
			gctl_t.ptc_flag =0;
   
			Ptc_Off();
	
		    LED_PTC_ICON_OFF();
           if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
                MqttData_Publish_SetPtc(0);
                HAL_Delay(200);
             }
		 }
	    }
		else{

           voice_send_turn_on_power_on_cmd();

	   }
		
		
	break;

	case voice_open_plasma:
	     if(gpro_t.gPower_On == power_on){
		 voice_send_function_cmd(0x09,0xAB);
		 if(plasma_state()==1){
	
			
		}
		else{

    	     gctl_t.manual_operation_ptc_flag = ptc_manual_on;
    		 gctl_t.plasma_flag=1;
    		 Plasma_On();
    		 LED_KILL_ICON_ON() ;
             if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
                    MqttData_Publish_SetPlasma(1);
                    HAL_Delay(200);
                }
    		}
	     }
		 else{

           voice_send_turn_on_power_on_cmd();

	   }
		
	
  
	break;

	case voice_open_plasma_2:
	     if(gpro_t.gPower_On == power_on){
		 voice_send_function_cmd(0x0A,0xAC);
		 if(plasma_state()==1){
		
			
		}
		else{

    	     gctl_t.manual_operation_ptc_flag = ptc_manual_on;
    		 gctl_t.plasma_flag=1;
    		 Plasma_On();
    		 LED_KILL_ICON_ON() ;
              if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
                    MqttData_Publish_SetPlasma(1);
                    HAL_Delay(200);
                }
		    }
	     }
		 else{

           voice_send_turn_on_power_on_cmd();

	   }
		
	
  
	break;
   case voice_close_plasma:
  
	 if(gpro_t.gPower_On == power_on){
	 voice_send_function_cmd(0x0B,0xAD);

	 if(plasma_state()==0){
	

	 }
	 else{
   	     gctl_t.manual_operation_ptc_flag = ptc_manual_off;
	     gctl_t.plasma_flag=0;
		 Plasma_Off();
		 LED_KILL_ICON_OFF() ;
         if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
                    MqttData_Publish_SetPlasma(0);
                    HAL_Delay(200);
                }
	 }

	 }
	 else{

        voice_send_turn_on_power_on_cmd();

	  }
   	
	
	break;

	case voice_close_plasma_2:
  
	 if(gpro_t.gPower_On == power_on){
	 voice_send_function_cmd(0x0C,0xAE);

	 if(plasma_state()==0){
	

	 }
	 else{
   	     gctl_t.manual_operation_ptc_flag = ptc_manual_off;
	     gctl_t.plasma_flag=0;
		 Plasma_Off();
		 LED_KILL_ICON_OFF() ;
         if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
              MqttData_Publish_SetPlasma(0);
              HAL_Delay(200);
           }
	 }

	 }
	 else{

        voice_send_turn_on_power_on_cmd();

	  }
   	
	
	break;

	case voice_open_rat:

	 if(gpro_t.gPower_On == power_on){
		 voice_send_function_cmd(0x0D,0xAF);

		 if(ultrasonic_state() ==1){
		

         }
		 else{
	
            gctl_t.ultrasonic_flag =1;
			Ultrasonic_Pwm_Output();
		    LED_RAT_ICON_ON();
            if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
                    MqttData_Publish_SetUltrasonic(1);
                    HAL_Delay(200);
                }
		 }
	 }
	 else{

        voice_send_turn_on_power_on_cmd();

	  }
		
		
	break;

	case voice_open_rat_2:

	 if(gpro_t.gPower_On == power_on){
		 voice_send_function_cmd(0x0E,0xB0);

		 if(ultrasonic_state() ==1){
		

         }
		 else{
	
            gctl_t.ultrasonic_flag =1;
			Ultrasonic_Pwm_Output();
		    LED_RAT_ICON_ON();
            if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
                    MqttData_Publish_SetUltrasonic(1);
                    HAL_Delay(200);
                }
		 }
	 }
	 else{

        voice_send_turn_on_power_on_cmd();

	  }
		
		
	break;
	case voice_close_rat:

	 if(gpro_t.gPower_On == power_on){
	 	voice_send_function_cmd(0x0F,0xB1);
		if(ultrasonic_state() ==0){
		//	buzzer_sound();//SendData_Buzzer();

		}
		else{
	   //  buzzer_sound();
		  gctl_t.ultrasonic_flag =0;
		 Ultrasonic_Pwm_Stop();
		 LED_RAT_ICON_OFF();
         if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
                    MqttData_Publish_SetUltrasonic(0);
                    HAL_Delay(200);
                }
		}
	 }
	 else{

           voice_send_turn_on_power_on_cmd();

	   }
	 
		
		
	break;

	case voice_close_rat_2:

	 if(gpro_t.gPower_On == power_on){
	 	voice_send_function_cmd(0x10,0xB2);
		if(ultrasonic_state() ==0){
		//	buzzer_sound();//SendData_Buzzer();

		}
		else{
	   //  buzzer_sound();
		  gctl_t.ultrasonic_flag =0;
		 Ultrasonic_Pwm_Stop();
		 LED_RAT_ICON_OFF();
         if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
                    MqttData_Publish_SetUltrasonic(0);
                    HAL_Delay(200);
                }
		}
	 }
	 else{

           voice_send_turn_on_power_on_cmd();

	   }
	 
		
		
	break;
	

	}
}

/***********************************************************************************
 *  *
    *Function Name: static void voice_ctl_fun(unit8_t data,uint8_t check_code)
    *Function: voice of command 
    *Input Ref: data:voice command , check_code : check code
    *Return Ref:  NO
    * 
*************************************************************************************/
static void  voice_set_temperature_value(uint8_t value)
{
       if(gpro_t.gPower_On == power_on){

	        value = 4+value;
			send_tx_set_temp_data(value);
		
			gctl_t.gSet_temperature_value = value;
		
			gctl_t.gSet_temperature_value_item=disp_do_setting_ptc_value_item;
	   
		
	       TFT_Disp_Voice_Temp_Value(0,gctl_t.gSet_temperature_value);
          

		   gpro_t.gTimer_pro_set_tem_value_blink=0;

		   if(gctl_t.gSet_temperature_value > gctl_t.dht11_temp_value){

		    	if(ptc_state()==0){
					gctl_t.ptc_flag =1;
					Ptc_On();
					LED_PTC_ICON_ON();
               
				

                    if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
                        MqttData_Publish_SetPtc(1);
                        osDelay(100);//HAL_Delay(200);
                    
                    }
		    	}

			}
			else{
		   		if(ptc_state()==1){
                    gctl_t.ptc_flag = 0;
               
			   		Ptc_Off();
			   		LED_PTC_ICON_OFF();
				

                    if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
                        MqttData_Publish_SetPtc(0);
                        osDelay(100);//HAL_Delay(200);
                    
                    }
				}
			}

             if(wifi_link_net_state()==1 && wifi_t.link_net_tencent_data_flag ==3){
                  MqttData_Publis_SetTemp(gctl_t.gSet_temperature_value);
                  osDelay(100); //HAL_Delay(200);
              }
       }
		else{

           voice_send_turn_on_power_on_cmd();

	     }

        
		
}
/***********************************************************
 *  *
    *Function Name: static void voice_set_timer_timing_value(uint8_t time)
    *Function: 
    *Input Ref: set timer timing time value 
    *Return Ref:  NO
    * 
***********************************************************/
static void voice_set_timer_timing_value(uint8_t set_hours)
{
     uint8_t set_value;
 
       if(gpro_t.gPower_On == power_on){

	   set_value = set_hours - 36;
	   
	   send_tx_set_timer_value(set_value);
   
	   if(set_value == 3)set_value=2;
		else if(set_value > 3){

			set_value--;

		}
        gpro_t.set_timer_timing_success = 1;
		gctl_t.gSet_timer_hours = set_value ;
        gpro_t.gTimer_pro_set_long_key_tims=0;
        gctl_t.gTimer_ctl_set_timer_time_senconds =0;
	    gpro_t.disp_works_timer_timing_mode_item = timer_set_time; //disp set timer timing value 
	  	
	
		TFT_Disp_Voice_Set_TimerTime_Init();
		
       }
	   else{

           voice_send_turn_on_power_on_cmd();

	   }
   }


static void voice_cancel_timer_timing(void)
{
 	if(gpro_t.gPower_On == power_on){

	     voice_send_function_cmd(0x3F,0xE1);

		 if(gpro_t.disp_works_timer_timing_state()==timer_time){
	  
		     gpro_t.disp_works_timer_timing_mode_item= works_time;
             gpro_t.set_timer_timing_success = 0; //WT.EDIT 2024.06.13
		
			TFT_Display_WorksTime_Voice();
	   	}
       }
	   else{

           voice_send_turn_on_power_on_cmd();
		}
}
/****************************************************************************************
 *  *
    *Function Name: static int8_t BinarySearch_Voice_Data(const uint8_t *pta,uint8_t key)
    *Function: from small to big sort
    *Input Ref: *pta is array ,look up key data
    *Return Ref:  NO
    * 
*****************************************************************************************/
static int8_t BinarySearch_Voice_Data(const uint8_t *pta,uint8_t key)
{
   static uint8_t n;
   uint8_t left,right,mid;
   
   n = sizeof(voice_sound_data)/(sizeof(voice_sound_data[0]));
   
   left = 0;
   right = n-1;

   while(left<right || left == right){

      mid = (left + right )/2;

      if(pta[mid] == key){
         return mid;
	  }
	  else if(pta[mid]> key){

	      right = right -1; //在左边查找

	  }
	  else if(pta[mid] < key){

          left = mid +1 ;

	  }

   }

   return -1;

}

/***********************************************************************************
 *  *
    *Function Name: static void voice_cmd_fun(uint8_t cmd)
    *Function: voice of command 
    *Input Ref: input command 
    *Return Ref:  NO
    * 
*************************************************************************************/
void Voice_Warning_Sound_Ptc(void)
{

  sendData_VoiceSound_Warning_Ptc();


}

/********************************************************************************
	**
	*Function Name:sendData_Real_TimeHum(uint8_t hum,uint8_t temp)
	*Function :
	*Input Ref: humidity value and temperature value
	*Return Ref:NO
	*
*******************************************************************************/
static void sendData_VoiceSound_Warning_Ptc(void)
{

	//crc=0x55;
	outputBuf[0]=0xA5; //master
	outputBuf[1]=0XFA; //41
	outputBuf[2]=0X00; //44	// 'D' data
	outputBuf[3]=0X03; //	// 'R' rotator motor for select filter
	outputBuf[4]=0X40; // // one command parameter
	outputBuf[5]=0X00;
	outputBuf[6]=0XE2;
	outputBuf[7]=0XFB;
	
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=8;
     HAL_UART_Transmit(&huart1,outputBuf,transferSize,0xffff);
    #if 0
	if(transferSize)
	{
		while(v_t.transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		v_t.transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}

   #endif 

}


/********************************************************************************
	**
	*Function Name:sendData_Real_TimeHum(uint8_t hum,uint8_t temp)
	*Function :
	*Input Ref: humidity value and temperature value
	*Return Ref:NO
	*
*******************************************************************************/
void Voice_Warning_Sound_Fan(void)
{
	sendData_VoiceSound_Warning_Fan();
}

/********************************************************************************
	**
	*Function Name:sendData_Real_TimeHum(uint8_t hum,uint8_t temp)
	*Function :
	*Input Ref: humidity value and temperature value
	*Return Ref:NO
	*
*******************************************************************************/
static void sendData_VoiceSound_Warning_Fan(void)
{

	//crc=0x55;
	outputBuf[0]=0xA5; //master
	outputBuf[1]=0XFA; //41
	outputBuf[2]=0X00; //44	// 'D' data
	outputBuf[3]=0X03; //	// 'R' rotator motor for select filter
	outputBuf[4]=0X41; // // one command parameter
	outputBuf[5]=0X00;
	outputBuf[6]=0XE3;
	outputBuf[7]=0XFB;
	
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=8;
    HAL_UART_Transmit(&huart1,outputBuf,transferSize,0xffff);
    #if 0
	if(transferSize)
	{
		while(v_t.transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		v_t.transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
    #endif 


}

static void voice_send_has_been_power_on_cmd(void)
{
	
	outputBuf[0]=0xA5; //master
	outputBuf[1]=0XFA; //41
	outputBuf[2]=0X00; //44	// 'D' data
	outputBuf[3]=0X03; //	// 'R' rotator motor for select filter
	outputBuf[4]=0X43; // // one command parameter
	outputBuf[5]=0X00;
	outputBuf[6]=0XE5;
	outputBuf[7]=0XFB;
	
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=8;
    HAL_UART_Transmit(&huart1,outputBuf,transferSize,0xffff);
    #if 0
	if(transferSize)
	{
		while(v_t.transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		v_t.transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
    #endif 

}
static void voice_send_power_on_cmd(void)
{
	
	outputBuf[0]=0xA5; //master
	outputBuf[1]=0XFA; //41
	outputBuf[2]=0X00; //44	// 'D' data
	outputBuf[3]=0X03; //	// 'R' rotator motor for select filter
	outputBuf[4]=0X02; // // one command parameter
	outputBuf[5]=0X00;
	outputBuf[6]=0XA4;
	outputBuf[7]=0XFB;
	

	transferSize=8;
    HAL_UART_Transmit(&huart1,outputBuf,transferSize,0xffff);
    #if 0
	if(transferSize)
	{
		while(v_t.transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		v_t.transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
    #endif 


}

static void voice_send_has_been_power_off_cmd(void)
{
	outputBuf[0]=0xA5; //master
	outputBuf[1]=0XFA; //41
	outputBuf[2]=0X00; //44	// 'D' data
	outputBuf[3]=0X03; //	// 'R' rotator motor for select filter
	outputBuf[4]=0X44; // // one command parameter
	outputBuf[5]=0X00;
	outputBuf[6]=0XE6;
	outputBuf[7]=0XFB;
	
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=8;
    HAL_UART_Transmit(&huart1,outputBuf,transferSize,0xffff);
    #if 0
	if(transferSize)
	{
		while(v_t.transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		v_t.transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
    #endif 

}

static void voice_send_power_off_cmd(void)
{
	outputBuf[0]=0xA5; //master
	outputBuf[1]=0XFA; //41
	outputBuf[2]=0X00; //44	// 'D' data
	outputBuf[3]=0X03; //	// 'R' rotator motor for select filter
	outputBuf[4]=0X03; // // one command parameter
	outputBuf[5]=0X00;
	outputBuf[6]=0XA5;
	outputBuf[7]=0XFB;
	
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=8;
	HAL_UART_Transmit(&huart1,outputBuf,transferSize,0xffff);
	#if 0
  //  HAL_UART_Transmit(&huart1,outputBuf,transferSize,0xffff);
	if(transferSize)
	{
		while(v_t.transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		v_t.transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}
    #endif 

}


static void voice_send_turn_on_power_on_cmd(void)
{

	    outputBuf[0]=0xA5; //master
		outputBuf[1]=0XFA; //41
		outputBuf[2]=0X00; //44 // 'D' data
		outputBuf[3]=0X03; //	// 'R' rotator motor for select filter
		outputBuf[4]=0X42; // // one command parameter
		outputBuf[5]=0X00;
		outputBuf[6]=0XE4;
		outputBuf[7]=0XFB;
		
	
		transferSize=8;
        HAL_UART_Transmit(&huart1,outputBuf,transferSize,0xffff);
        #if 0
		if(transferSize)
		{
			while(v_t.transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
			v_t.transOngoingFlag=1;
			HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
		}

        #endif 


}

static void voice_send_function_cmd(uint8_t cmd1,uint8_t cmd2)
{
	
   outputBuf[0]=0xA5; //master
   outputBuf[1]=0XFA; //41
   outputBuf[2]=0X00; //44 // 'D' data
   outputBuf[3]=0X03; //   // 'R' rotator motor for select filter
   outputBuf[4]=cmd1; // // one command parameter
   outputBuf[5]=0X00;
   outputBuf[6]=cmd2;
   outputBuf[7]=0XFB;
   

   transferSize=8;

   HAL_UART_Transmit(&huart1,outputBuf,transferSize,0xffff);
   #if 0
   if(transferSize)
   {
	   while(v_t.transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
	   v_t.transOngoingFlag=1;
	   HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
   }
   #endif 



}

static void send_tx_set_temp_data(uint8_t temp)
{
   switch(temp){

    case 20:
		voice_send_function_cmd(0x11,0xB3);
    break;

	case 21:
		voice_send_function_cmd(0x12,0xB4);
    break;

	case 22:
		voice_send_function_cmd(0x13,0xB5);
    break;

	case 23:
		voice_send_function_cmd(0x14,0xB6);
    break;

	case 24:
		voice_send_function_cmd(0x15,0xB7);
    break;

	case 25:
		voice_send_function_cmd(0x16,0xB8);
    break;

	case 26:
		   voice_send_function_cmd(0x17,0xB9);
	   break;
   
	case 27:
	voice_send_function_cmd(0x18,0xBA);
	break;

	case 28:
	voice_send_function_cmd(0x19,0xBB);
	break;

	case 29:
	voice_send_function_cmd(0x1A,0xBC);
	break;

	case 30:
	voice_send_function_cmd(0x1B,0xBD);
	break;

	case 31:
	voice_send_function_cmd(0x1C,0xBE);
	break;

	case 32:
	voice_send_function_cmd(0x1D,0xBF);
	break;
   
	case 33:
	voice_send_function_cmd(0x1E,0xC0);
	break;

	case 34:
	voice_send_function_cmd(0x1F,0xC1);
	break;

	case 35:
	voice_send_function_cmd(0x20,0xC2);
	break;

	case 36:
	voice_send_function_cmd(0x21,0xC3);
	break;

	case 37:
	voice_send_function_cmd(0x22,0xC4);
	break;


	case 38:
	voice_send_function_cmd(0x23,0xC5);
	break;

	case 39:
	voice_send_function_cmd(0x24,0xC6);
	break;

	case 40:
	voice_send_function_cmd(0x25,0xC7);
	break;

	}


}

static void send_tx_set_timer_value(uint8_t set_hours)
{

	switch(set_hours){
	
		case 1:
			voice_send_function_cmd(0x26,0xC8);
		break;
	
		case 2: //2 HOURS
			voice_send_function_cmd(0x27,0xC9);
		break;
	
		case 3: //2小时
			voice_send_function_cmd(0x28,0xCA);
		break;
	
		case 4://3
			voice_send_function_cmd(0x29,0xCB);
		break;
	
		case 5://4
			voice_send_function_cmd(0x2A,0xCC);
		break;
	
		case 6://5
			voice_send_function_cmd(0x2B,0xCD);
		break;
	
		case 7://6
			   voice_send_function_cmd(0x2C,0xCE);
		   break;
	   
		case 8://7
		voice_send_function_cmd(0x2D,0xCF);
		break;
	
		case 9://8
		voice_send_function_cmd(0x2E,0xD0);
		break;
	
		case 10://9
		voice_send_function_cmd(0x2F,0xD1);
		break;
	
		case 11://10
		voice_send_function_cmd(0x30,0xD2);
		break;
	
		case 12://11
		voice_send_function_cmd(0x31,0xD3);
		break;
	
		case 13://12
		voice_send_function_cmd(0x32,0xD4);
		break;
	   
		case 14://13
		voice_send_function_cmd(0x33,0xD5);
		break;
	
		case 15://14
		voice_send_function_cmd(0x34,0xD6);
		break;
	
		case 16://15
		voice_send_function_cmd(0x35,0xD7);
		break;
	
		case 17://16
		voice_send_function_cmd(0x36,0xD8);
		break;
	
		case 18://17
		voice_send_function_cmd(0x37,0xD9);
		break;
	
	
		case 19://18
		voice_send_function_cmd(0x38,0xDA);
		break;
	
		case 20://19
		voice_send_function_cmd(0x39,0xDB);
		break;
	
		case 21://20
		voice_send_function_cmd(0x3A,0xDC);
		break;

		case 22://21
		voice_send_function_cmd(0x3B,0xDD);
		break;

		case 23://22
		voice_send_function_cmd(0x3C,0xDE);
		break;

		case 24://23
		voice_send_function_cmd(0x3D,0xDF);
		break;

		case 25://24
		voice_send_function_cmd(0x3E,0xE0);
		break;
	
		}




}



