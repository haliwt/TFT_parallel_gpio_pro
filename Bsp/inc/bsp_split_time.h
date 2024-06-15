#ifndef __BSP_SPLIT_TIME_H
#define __BSP_SPLIT_TIME_H
#include "main.h"


typedef enum TIMING_T{

   works_time=0x01,
   timer_time,
   timer_set_time,
   timing_success ,
   fun_selection,
   timer_countdown
   
}timing_t;




void TimerTiming_Init(void);

void TimeTimer_Pro_Handler(void);

void Display_Precise_Works_Time(void);

void Power_Off_Retain_Beijing_Time(void);


void Key_Works_Timer_Timing_Handler(uint8_t(*worktiming_handler)(void));


void Dis_pWorks_Timer_Timing_Handler(uint8_t(*disp_worktimer_handler)(void));



#endif 







