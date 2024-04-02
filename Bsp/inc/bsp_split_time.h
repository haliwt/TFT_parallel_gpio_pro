#ifndef __BSP_SPLIT_TIME_H
#define __BSP_SPLIT_TIME_H
#include "main.h"


typedef enum TIMING_T{

   works_time,
   timer_time,
   timer_set_time,
   timing_success ,
   fun_selection
   
}timing_t;


void TimeTimer_Pro_Handler(void);


#endif 



