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


static uint8_t transferSize;
static uint8_t outputBuf[MAX_BUFFER_SIZE];

void (*rx_voice_data)(uint8_t data);

static void sendData_VoiceSound_Warning_Ptc(void);
static void sendData_VoiceSound_Warning_Fan(void);



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
//算法是:data4 + data6 = element
static uint8_t const voice_sound_data[55]={

	0x22,0x24,0x26,0x28,0x2a, //0x24 = power_on ,0x26= power_off
	0x2c,0x2e,0x30,0x32,0x34,
    0x36,0x38,0x3a,0x3c,0x3e,
	0x40,0x42,0x44,0x46,0x48,
	0x4a,0x4c,0x4e,0x50,0x52,
	0x54,0x56,0x58,0x5a,0x5c,
	0x5e,
    0x60,0x62,0x64,0x66,0x68,
	0x6a,0x6c,0x6e,0x70,0x72,
	0x74,0x76,0x78,0x7a,0x7c,
	0x7e,0x80,0x82,0x84,0x86,
	0x88,0x8a,0x8c,0x8e

};




static int8_t BinarySearch_Voice_Data(const uint8_t *pta,uint8_t key);

static void voice_cmd_fun(uint8_t cmd);
static void  voice_set_temperature_value(uint8_t value);
static void voice_set_timer_timing_value(uint8_t time);

uint8_t key;
int8_t result;




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

  static uint8_t voice_cmd_flag;

  if(v_t.voice_decoder_flag == 1){
  	  v_t.voice_decoder_flag=0;

  if(v_t.voice_wakeword_enable ==1 && v_t.gTimer_voice_time_counter_start < 16){
	   voice_cmd_flag=1;
	if(v_t.voice_power_on_cmd==1 || pro_t.gPower_On == power_on){
		VOICE_OUTPUT_SOUND_ENABLE();
		

	key= v_t.RxBuf[0] + v_t.RxBuf[1]; //key= data4+ data6 = ; //A5 FA 00 81 01 00 21 FB 

	result = BinarySearch_Voice_Data(voice_sound_data,key);


    if(result < 0x0A && result > 0){
	   voice_cmd_fun(result);

    }
	
   if(result > 9 && result < 31){ //set temperature value 
		   
            voice_set_temperature_value(result);
			
		

	}

	if(result > 30 && result <55){ //set timer timing value 
	

		voice_set_timer_timing_value(result);
		
		

	 }
    }
	
   
	}
  }
  
  if(v_t.gTimer_voice_time_counter_start > 15 && voice_cmd_flag==1){
	   voice_cmd_flag++;
       v_t.voice_wakeword_counter++;
       v_t.voice_decoder_flag=0;
       v_t.voice_wakeword_enable =0;
	 
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
	  
	   	if(pro_t.gPower_On == power_on){
			
        
	
            
        }
		else{
         
	
		    pro_t.gPower_On = power_on;   
            pro_t.long_key_flag =0;
            pro_t.run_process_step=0;

			gctl_t.ptc_warning =0;
		    gctl_t.fan_warning =0;
			
		}
		
	break;

	case voice_power_off:
		if(pro_t.gPower_On == power_off){
			
			

		}
		else{
		
		    pro_t.power_off_flag=1;
			pro_t.gPower_On = power_off; 
		
		
			
		}
	break;

	case voice_link_wifi:
		
		if(wifi_link_net_state()==0){
		    
		    wifi_t.link_tencent_step_counter=0;
			wifi_t.esp8266_login_cloud_success =0;
			pro_t.wifi_led_fast_blink_flag=1 ;
			wifi_t.runCommand_order_lable=wifi_link_tencent_cloud;
			wifi_t.wifi_config_net_lable= wifi_set_restor;
			wifi_t.gTimer_login_tencent_times=0;
			wifi_t.gTimer_linking_tencent_duration=0; //166s -2分7秒
           
		}
		
		
		

	
	break;

	case voice_open_ptc:

	
     if(ptc_state()==1 ){
   
	 }
	 else{
	
		gctl_t.ptc_flag =1;
		Ptc_On();
	    LED_PTC_ICON_ON();

	 }
	
	
     
   
    break;

	case voice_close_ptc:
	
		 if(ptc_state() == 0){
          
       

		 }
		 else{
		
			gctl_t.ptc_flag =0;
			Ptc_Off();
		    LED_PTC_ICON_OFF();
		 }
		
		
	break;

	case voice_open_plasma:
		
		 if(plasma_state()==1){
			//buzzer_sound();//SendData_Buzzer();
			
		}
		else{

		// buzzer_sound();//SendData_Buzzer();
		 gctl_t.plasma_flag=1;
		 Plasma_On();
		 LED_KILL_ICON_ON() ;
		}
		
	
  
	break;
   case voice_close_plasma:
 
   	 if(plasma_state()==0){
		// buzzer_sound();//SendData_Buzzer();

	 }
	 else{
   	    // buzzer_sound();
	     gctl_t.plasma_flag=0;
		 Plasma_Off();
		 LED_KILL_ICON_OFF() ;
	 }
   	
	
	break;

	case voice_open_rat:

		 if(ultrasonic_state() ==1){
		//	buzzer_sound();//SendData_Buzzer();

         }
		 else{
		//	buzzer_sound();
            gctl_t.ultrasonic_flag =1;
			Ultrasonic_Pwm_Output();
		    LED_RAT_ICON_ON();
		 }
		
		
	break;
	case voice_close_rat:

		if(ultrasonic_state() ==0){
		//	buzzer_sound();//SendData_Buzzer();

		}
		else{
	   //  buzzer_sound();
		  gctl_t.ultrasonic_flag =0;
		 Ultrasonic_Pwm_Stop();
		 LED_RAT_ICON_OFF();
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
  
			value = 10+value;
		
			gctl_t.gSet_temperature_value = value;
			pro_t.gTimer_pro_set_tem_value_blink=0;
			gctl_t.gSet_temperature_value_item=set_temp_value_item;
	        v_t.voice_set_temperature_value_flag=1;
	        TFT_Disp_Voice_Temp_Value(0,gctl_t.gSet_temperature_value); 

    

}
/***********************************************************
 *  *
    *Function Name: static void voice_set_timer_timing_value(uint8_t time)
    *Function: 
    *Input Ref: set timer timing time value 
    *Return Ref:  NO
    * 
***********************************************************/
static void voice_set_timer_timing_value(uint8_t time)
{

	pro_t.mode_key_pressed_flag =0;
//	Buzzer_KeySound();
	pro_t.gTimer_pro_mode_key_be_select = 0; 

    v_t.voice_set_timer_timing_value = time - 30;
    
	gctl_t.gSet_timer_hours = v_t.voice_set_timer_timing_value ;
	v_t.voice_input_timer_flag =1;

	pro_t.mode_key_run_item_step = mode_key_timer_time;
	pro_t.timer_mode_flag=timer_set_time; //set timer mode enable
	//to switch works or timer item flag  dis chines words
	 gctl_t.timer_timing_words_changed_flag ++;
	 gctl_t.timing_words_changed_flag++;
	 /*************************************/
	
	

	TFT_Disp_Voice_Set_TimerTime_Init();
    

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
/**********************************************************************************
 *  *
    *Function Name: void Voice_Hello_Word_Handler(uint8_t(*hello_handler)(void))
    *Function: decoder to command 
    *Input Ref: NO
    *Return Ref:  NO
    * 
***********************************************************************************/

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
	outputBuf[4]=0X38; // // one command parameter
	outputBuf[5]=0X00;
	outputBuf[6]=0XDA;
	outputBuf[7]=0XFB;
	
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=8;
	if(transferSize)
	{
		while(v_t.transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		v_t.transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}



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
	outputBuf[4]=0X39; // // one command parameter
	outputBuf[5]=0X00;
	outputBuf[6]=0XDB;
	outputBuf[7]=0XFB;
	
	//for(i=3;i<6;i++) crc ^= outputBuf[i];
	//outputBuf[i]=crc;
	transferSize=8;
	if(transferSize)
	{
		while(v_t.transOngoingFlag); //UART interrupt transmit flag ,disable one more send data.
		v_t.transOngoingFlag=1;
		HAL_UART_Transmit_IT(&huart1,outputBuf,transferSize);
	}



}


