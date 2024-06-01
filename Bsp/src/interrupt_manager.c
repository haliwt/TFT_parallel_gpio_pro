#include "interrupt_manager.h"
#include "bsp.h"



uint8_t voice_inputBuf[1];


/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void USART_Cmd_Error_Handler(void)
{
 
	   uint32_t temp;
       if(gctl_t.gTimer_ctl_usart1_error > 6 ){ //9
			
			gctl_t.gTimer_ctl_usart1_error=0;
			  __HAL_UART_CLEAR_OREFLAG(&huart1);
	        
	          temp = USART1->RDR;
	     
	         HAL_UART_Receive_IT(&huart1,voice_inputBuf,1);//UART receive data interrupt 1 byte
			 // UART_Start_Receive_IT(&huart1,inputBuf,1);
			
	  }

	 if(gctl_t.gTimer_ctl_usart2_error >8){
	  	gctl_t.gTimer_ctl_usart2_error=0;

           __HAL_UART_CLEAR_OREFLAG(&huart2);
        

          temp = USART2->RDR;
		 
		    HAL_UART_Receive_IT(&huart2,wifi_t.usart2_dataBuf,1);
		
     }

	 
}

  

/*******************************************************************************
	*
	*Function Name:void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
	*Function : timer timing 1ms 
	*           sys_clk = 64MHz
	*
*******************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  static uint16_t tm0;
  static uint8_t tm1,tm2;
 

    
   if(htim->Instance==TIM17){
    
    tm0++;  //1ms
    tm1++;
   
    gpro_t.gTimer_pro_wifi_fast_led++;

	if(tm1 > 9){
		tm1=0;
		gctl_t.gTimer_ctl_select_led++;
     }  
	
	
	if(tm0>999){ //1000 *1ms = 1000ms = 1s
		tm0=0;
		tm2++;


    

	 //main process timer
	
	    //be used to timer 
	   gpro_t.gTimer_pro_wifi_dht11_temp_hum++;
       gpro_t.gTimer_pro_update_dht11_data ++;
       gpro_t.gTimer_pro_set_tem_value_blink++;
       gpro_t.gTimer_pro_mode_key_be_select++;
       gpro_t.gTimer_pro_wifi_led++;
       gpro_t.gTimer_pro_time_split_symbol++;  //diplay timer ":"
       gpro_t.gTimer_pro_fan++;  //fan continuce counter 60s
       gpro_t.gTimer_pro_feed_dog++;
       gpro_t.gTimer_pro_temp_delay++;
   
        //cotrol timer
    
	   gctl_t.gTimer_ctl_set_timer_time_senconds++;
	   gctl_t.gTimer_ctl_ptc_adc_times++;
	   gctl_t.gTimer_ctl_fan_adc_times ++;
	   gctl_t.gTimer_ctl_disp_works_time_second++;
	   gctl_t.gTimer_ctl_warning_time++;
	   gctl_t.gTimer_ctl_usart1_error++; 
	   gctl_t.gTimer_ctl_usart2_error++; 
	  
	
	  //wifi counter 
	 
	  wifi_t.gTimer_login_tencent_times++;
	  wifi_t.gTimer_publish_dht11++;
	  wifi_t.gTimer_auto_detected_net_state_times++;
	  wifi_t.gTimer_get_beijing_time++;
	  wifi_t.gTimer_read_beijing_time++;
	
	  
	 
	  wifi_t.gTimer_linking_tencent_duration++;
	  wifi_t.gTimer_power_first_link_tencent ++;
	  wifi_t.gTimer_main_pro_times++;
     

	  //voice sound 
	  v_t.gTimer_voice_time_counter_start++;
	 
	  	

	  

	  if(tm2 > 59){//1 minute
          tm2 =0;
        
		   gpro_t.gTimer_run_total++;
           gpro_t.gTimer_run_time_out++;
	  }
	 
	  
	 
   }
  }
}

/********************************************************************************
**
*Function Name:void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
*Function :UART callback function  for UART interrupt for transmit data
*Input Ref: structure UART_HandleTypeDef pointer
*Return Ref:NO
*
*******************************************************************************/
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if(huart==&huart1) //voice  sound send 
	{
		v_t.transOngoingFlag=0; //UART Transmit interrupt flag =0 ,RUN
	}

//	if(huart== &huart2){
//
//       usart2_transOngoingFlag =0;
//
//	}

}







