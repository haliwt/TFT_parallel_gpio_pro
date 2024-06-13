#ifndef __BSP_VOICE_H
#define __BSP_VOICE_H
#include "main.h"

#define  RX_BUF_SIZE  8



#define VOICE_OUTPUT_SOUND_ENABLE()       do{VOICE_MUTE_GPIO_Port->BSRR |= VOICE_MUTE_Pin;}while(0)
#define VOICE_OUTPUT_SOUND_DISABLE()      do{VOICE_MUTE_GPIO_Port->BSRR |= (uint32_t)VOICE_MUTE_Pin<<16;}while(0)


extern uint8_t voice_inputBuf[1];




typedef enum{
	voice_power_on=0x01,
	voice_power_off=0x02,
	voice_link_wifi=0x03,
	voice_open_ptc=0x04,
	voice_open_ptc_2= 0x05,
	voice_close_ptc=0x06,
	voice_close_ptc_2= 0x07,
	voice_open_plasma=0x08,
	voice_open_plasma_2= 0x09,
	voice_close_plasma=0x0A,
	voice_close_plasma_2=0x0B,
	voice_open_rat=0x0C,
	voice_open_rat_2=0x0D,
	voice_close_rat=0x0E,
	voice_close_rat_2=0x0F,
}voice_cmd;






typedef struct{

    
	uint8_t voice_ctl_flag;
	uint8_t voice_rxBuf[RX_BUF_SIZE];
    uint8_t voice_data[2];
	uint8_t rx_voice_data_enable;
	uint8_t rxCounter;
	uint8_t voice_to_buzzer_flag ;
    uint8_t sound_rx_data_success_flag;

	uint8_t voice_wakeword_enable;

	uint8_t rx_enable_voice_output ;
	uint8_t transOngoingFlag;

	
	uint8_t voice_wakeword_counter;
	

	//voice ctl display function "icon" 
	uint8_t voice_ptc_flag;
	uint8_t voice_plasma_flag;
	uint8_t voice_bug_flag;

	uint8_t voice_set_timer_timing_value;
	
   uint8_t gTimer_voice_time_counter_start ;

	
}voice_sound_t;


extern voice_sound_t v_t;



void Voice_Decoder_Handler(void);





void Voice_Warning_Sound_Ptc(void);


void Voice_Warning_Sound_Fan(void);




#endif 


