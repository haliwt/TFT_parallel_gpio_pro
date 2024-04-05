#ifndef __BSP_VOICE_H
#define __BSP_VOICE_H
#include "main.h"

#define  RX_BUF_SIZE  10

//#define VOICE_SOUND_OUTPUT()       HAL_GPIO_WritePin(VOICE_BROAD_GPIO_Port ,VOICE_BROAD_Pin,GPIO_PIN_RESET) 
//#define VOICE_SOUND_DISABLE()      HAL_GPIO_WritePin(VOICE_BROAD_GPIO_Port ,VOICE_BROAD_Pin,GPIO_PIN_SET) 


extern uint8_t voice_inputBuf[1];




typedef enum{
	voice_power_on=0x01,
	voice_power_off=0x02,
	voice_link_wifi=0x03,
	voice_open_ptc=0x04,
	voice_close_ptc=0x05,
	voice_open_plasma=0x06,
	voice_close_plasma=0x07,
	voice_open_rat=0x08,
	voice_close_rat=0x09,
}voice_cmd;






typedef struct{

    
	uint8_t voice_ctl_flag;
	uint8_t RxBuf[RX_BUF_SIZE];
	uint8_t rx_voice_data_enable;
	uint8_t rxCounter;
	uint8_t voice_to_buzzer_flag ;
	uint8_t voice_soun_output_enable;
	uint8_t rx_voice_cmd_enable;
	uint8_t voice_cmd_power_off_flag;
	uint8_t recoder_cmd_counter;
	uint8_t rx_enable_voice_output ;
	uint8_t transOngoingFlag;
	

	//voice ctl display function "icon" 
	uint8_t voice_ptc_flag;
	uint8_t voice_plasma_flag;
	uint8_t voice_bug_flag;
	uint8_t voice_set_temperature_value_flag;
	uint8_t voice_set_timer_timing_value;
	
   
	uint8_t RxStatus;
	uint8_t voice_input_timer_flag;
	uint8_t gTimer_voice_time;
	uint8_t gTimer_voice_sound_input_time;

	
}voice_sound_t;


extern voice_sound_t v_t;



void Voice_Decoder_Handler(void);



//void Voice_GPIO_Dir_Output_Init(void);


//void Voice_GPIO_Dir_Iniput_Init(void);


void Voice_Warning_Sound_Ptc(void);


void Voice_Warning_Sound_Fan(void);




#endif 

