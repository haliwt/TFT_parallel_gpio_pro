#ifndef __BSP_LED_H
#define __BSP_LED_H
#include "main.h"


#define LED_POWER_KEY_SetHigh()            do{LED_POWER_GPIO_Port->BSRR |= LED_POWER_Pin;}while(0)//HAL_GPIO_WritePin(LED_POWER_GPIO_Port,LED_POWER_Pin,GPIO_PIN_SET)    // output high level
#define LED_POWER_KEY_SetLow()             do{LED_POWER_GPIO_Port->BSRR |=(uint32_t)LED_POWER_Pin<<16;}while(0)//HAL_GPIO_WritePin(LED_POWER_GPIO_Port,LED_POWER_Pin,GPIO_PIN_RESET)    // output low level

#define LED_MODE_KEY_SetHigh()            do{LED_KEY_TIM_GPIO_Port->BSRR |= LED_KEY_TIM_Pin;}while(0)//HAL_GPIO_WritePin(LED_KEY_TIM_GPIO_Port,LED_KEY_TIM_Pin,GPIO_PIN_SET)    // output high level
#define LED_MODE_KEY_SetLow()             do{LED_KEY_TIM_GPIO_Port->BSRR |=(uint32_t)LED_KEY_TIM_Pin<<16;}while(0)//HAL_GPIO_WritePin(LED_KEY_TIM_GPIO_Port,LED_KEY_TIM_Pin,GPIO_PIN_RESET)    // output low level

  
//WIFI ICON LED
#define LED_WIFI_ICON_ON()                  do{LED_WIFI_GPIO_Port->BSRR |= (uint32_t)LED_WIFI_Pin<<16;}while(0) //  do{LED_RAT_GPIO_Port->BSRR |= (uint32_t)LED_RAT_Pin<<16;}while(0)//HAL_GPIO_WritePin(LED_RAT_GPIO_Port,LED_RAT_Pin,GPIO_PIN_RESET)//HAL_GPIO_WritePin(LED_WIFI_GPIO_Port ,LED_WIFI_Pin,GPIO_PIN_RESET)
#define LED_WIFI_ICON_OFF()                 do{LED_WIFI_GPIO_Port->BSRR |=LED_WIFI_Pin;}while(0) //  do{LED_RAT_GPIO_Port->BSRR |= LED_RAT_Pin;}while(0)//HAL_GPIO_WritePin(LED_RAT_GPIO_Port,LED_RAT_Pin,GPIO_PIN_SET)//HAL_GPIO_WritePin(LED_WIFI_GPIO_Port ,LED_WIFI_Pin,GPIO_PIN_SET)
//PTC ICON LED
#define LED_PTC_ICON_ON()                 do{LED_KILL_GPIO_Port->BSRR |= (uint32_t)LED_KILL_Pin<<16;}while(0)//HAL_GPIO_WritePin(LED_KILL_GPIO_Port,LED_KILL_Pin,GPIO_PIN_RESET)//HAL_GPIO_WritePin(LED_PTC_GPIO_Port,LED_PTC_Pin,GPIO_PIN_RESET)
#define LED_PTC_ICON_OFF()                do{LED_KILL_GPIO_Port->BSRR |= LED_KILL_Pin;}while(0)//HAL_GPIO_WritePin(LED_KILL_GPIO_Port,LED_KILL_Pin,GPIO_PIN_SET)//HAL_GPIO_WritePin(LED_PTC_GPIO_Port,LED_PTC_Pin,GPIO_PIN_SET)

//KILL ICON LED
#define LED_KILL_ICON_ON()               do{LED_PTC_GPIO_Port->BSRR |=(uint32_t)LED_PTC_Pin <<16;}while(0)////HAL_GPIO_WritePin(LED_PTC_GPIO_Port,LED_PTC_Pin,GPIO_PIN_RESET)// HAL_GPIO_WritePin(LED_KILL_GPIO_Port,LED_KILL_Pin,GPIO_PIN_RESET)
#define LED_KILL_ICON_OFF()              do{LED_PTC_GPIO_Port->BSRR |= LED_PTC_Pin;}while(0)//HAL_GPIO_WritePin(LED_PTC_GPIO_Port,LED_PTC_Pin,GPIO_PIN_SET)//  HAL_GPIO_WritePin(LED_KILL_GPIO_Port,LED_KILL_Pin,GPIO_PIN_SET)

//ULTRSONIC ICO LED
#define LED_RAT_ICON_ON()                 do{LED_RAT_GPIO_Port->BSRR |= (uint32_t)LED_RAT_Pin<<16;}while(0) //do{LED_WIFI_GPIO_Port->BSRR |=(uint32_t)LED_WIFI_Pin << 16;}while(0)//HAL_GPIO_WritePin(LED_WIFI_GPIO_Port ,LED_WIFI_Pin,GPIO_PIN_RESET) //HAL_GPIO_WritePin(LED_RAT_GPIO_Port,LED_RAT_Pin,GPIO_PIN_RESET)
#define LED_RAT_ICON_OFF()               do{LED_RAT_GPIO_Port->BSRR |= LED_RAT_Pin;}while(0)// do{LED_WIFI_GPIO_Port->BSRR |=LED_WIFI_Pin;}while(0)//HAL_GPIO_WritePin(LED_WIFI_GPIO_Port ,LED_WIFI_Pin,GPIO_PIN_SET)//HAL_GPIO_WritePin(LED_RAT_GPIO_Port,LED_RAT_Pin,GPIO_PIN_SET)










void LED_Mode_Key_On(void);
void LED_Mode_Key_Off(void);

void LED_Power_Key_On(void);
void LED_Power_Key_Off(void);

void LED_Net_On(void);
void LED_Net_Off(void);


void power_on_action_led_init(void);


void Power_Off_Led(void);


void Breath_Led(void);


#endif 

