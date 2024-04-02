#ifndef __BSP_NTC_H_
#define __BSP_NTC_H_
#include "main.h"

typedef enum{
   decimals,
   integer

}read_ntc_value;


typedef struct _touchpad_t{

   //
    uint8_t relay_fan_flag ;
    uint8_t relay_tape_flag ;
	uint8_t relay_kill_flag ;
	uint8_t relay_keep_temp_flag ;
	
   

   //smg
   uint8_t gTimer_smg;
   uint8_t decate_temp_value;
   uint8_t uint_tem_value;
   uint8_t decimal_point_temp_value;
   uint8_t  temperature_value;
   uint8_t temperature_decimal_point_value;
   uint8_t read_key_value;
   uint8_t run_process_tag;
   uint8_t keep_heat_flag;

 
   //display net temperatur degree
   uint8_t read_ntc_display_integer_value;
   uint8_t read_ntc_display_decimals_value;
   uint8_t gTimer_read_adc;
   uint8_t gTimer_display;
   uint8_t gTimer_select_fun;
   
   uint8_t confirm_key_select_item_fan;
   uint8_t confirm_key_select_item_tape;
   uint8_t confirm_key_select_item_sterilization;
   uint8_t confirm_key_select_item_keep_heat;
   uint8_t confirm_key_select_item_add_dec;
   uint8_t gTimer_keep_heat_fun;
  
   uint8_t keep_heat_run_flag;
   uint8_t keep_heat_fun_digital_numbers;
  
  

   uint8_t power_on_times;
  
   uint8_t temp_degree;
   
    uint16_t gTimer_smg_turn_on ;
   

	//taouchpad
	 int8_t temperature_rectify_value;
	 int8_t digital_numbers;
	 uint16_t ntc_voltage_value;
	 uint64_t relay_flag_flash_data;
	 uint64_t relay_keep_temp_data;



}touchpad_t;


extern touchpad_t ctl_t;



//void Read_NTC_Temperature_Power_On(void);

void Read_NTC_Temperature_Value_Handler(void);
//void Display_Speicial_Temperature_Value(uint8_t temp);



#endif 
