#ifndef __BSP_KEY_H_
#define __BSP_KEY_H_
#include "main.h"


#define INTERRUPT_KEY                0
#define NORMAL_KEY                   0
#define NORMAL_KEY_2                0
#define NORMAL_KEY_3                 0

#define KEY_POWER_VALUE()            HAL_GPIO_ReadPin(KEY_POWER_GPIO_Port,KEY_POWER_Pin)

#define KEY_MODE_VALUE()             HAL_GPIO_ReadPin(KEY_MODE_GPIO_Port,KEY_MODE_Pin)


#define KEY_DEC_VALUE()              HAL_GPIO_ReadPin(KEY_DEC_GPIO_Port,KEY_DEC_Pin)
#define KEY_ADD_VALUE()              HAL_GPIO_ReadPin(KEY_ADD_GPIO_Port,KEY_ADD_Pin)



#define KEY_DOWN_LEVEL               0  /* ¸ù¾ÝÔ­ÀíÍ¼Éè¼Æ£¬KEY2°´ÏÂÊ±Òý½ÅÎªµÍµçÆ½£¬ËùÒÔÕâÀïÉèÖÃÎª0 */


#define		_KEY_ALL_OFF				 0XFF

//normal times be pressed key
#define		_KEY_POWER     		0x01  //key power
#define		_KEY_MODE     		0x02  //key mode
#define		_KEY_DEC     			0x04  //key dec
#define		_KEY_ADD     			0x08  //key add



//long times be pressed
#define		_KEY_CONT_1_POWER     	     0x81    //??????--???
#define		_KEY_CONT_2_MODE   		     0x82    //??????--???
#define		_KEY_CONT_3_DEC    	         0x84    //?????? ?????
#define		_KEY_CONT_4_ADD     	     0x88    //??? --?????

#define KEY0_PRES 	0x01
#define KEY1_PRES	0x02
#define KEY2_PRES	0x04
#define WKUP_PRES   0x08

extern uint32_t  K1;



//#define HIGH_LEVEL                   1  /* 根据原理图设计，KEY1按下时引脚为低电平，所以这里设置为0 */

typedef enum
{
  KEY_DOWN = 0,
  KEY_UP   = 1,
  

}KEYState_TypeDef;


typedef enum{

  set_temperature,
  set_timer_timing
  
}key_state_en;

typedef enum{

   power_key_id =0x01,
   mode_key_id,
   add_key_id,
   dec_key_id
   

}key_input_state;

typedef enum{

    run_update_data =0x01,
	power_off_fan_pro
	

}process_state;

typedef enum{

 link_net =0x01,
 ask_link_state,


}wifi_cmd;






typedef enum{

	
	POWER_ON_ITEM=0xe0,
	LINK_WIFI_ITEM=0x81,
	MODE_KEY_ITEM=0x02,
	MODE_KEY_LONG_TIME_KEY=0x82,
	ADD_KEY_ITEM=0x08,
	DEC_KEY_ITEM=0x04,
	POWER_OFF_ITEM=0x01,
	KEY_NULL=0xff,
}key_state;

typedef enum{

 
  POWER_ID_FLAG =0x02,
  POWER_NULL


}power_state;






typedef enum _key_value{

  KEY_POWER= 0X80,KEY_MODE=0X40,KEY_DEC=0X20,KEY_ADD=0X10,KEY_LONG_POWER=0X81

}key_value;


typedef  struct  _state_
{
 unsigned char         read;
 unsigned char         buffer;
 unsigned long          value;
 unsigned char        off_time;
 unsigned long        on_time;

 enum{
  start  = 0,
  first  = 1,
  second = 2,
  cont   = 3,
  end    = 4,
  finish = 5,
 }state;
}key_types;


void Key_Init(void);





void SplitDispose_Key(uint8_t value);

uint8_t KEY_Scan(void);


uint8_t  KEY_GPIO_Scan(uint8_t mode);

void WaitingPowerOn_Fun(void);
void Wifi_Detected_KeyScan(void);

void Key_TheSecond_Scan(void);

void Key_Set_Timer_Handler(uint8_t(*key_timer_handler)(void));

void Power_Handler(uint8_t(*power_on_handler)(void));

uint8_t ReadKey(void);


//uint8_t  POWER_KEY_StateRead(void);
//KEYState_TypeDef MODE_KEY_StateRead(void);
//KEYState_TypeDef ADD_KEY_StateRead(void);
//KEYState_TypeDef DEC_KEY_StateRead(void);



#endif 



