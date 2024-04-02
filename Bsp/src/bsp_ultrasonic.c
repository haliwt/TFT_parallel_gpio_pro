#include "bsp_ultrasonic.h"
#include "bsp.h"



void Ultrasonic_Pwm_Output(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);//ultrasnoic On

}
void Ultrasonic_Pwm_Stop(void)
{
     HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_1);//ultrasnoic Off

}
