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
   
    pro_t.gTimer_pro_wifi_fast_led++;

	if(tm1 > 9){
		tm1=0;
		gctl_t.gTimer_ctl_select_led++;
	    pro_t.gTimer_pro_detect_key_ms =1;
	}
	
	
	if(tm0>999){ //1000 *1ms = 1000ms = 1s
		tm0=0;
		tm2++;


    

	 //main process timer
	 pro_t.gTimer_pro_temp_delay++;
	

	 pro_t.gTimer_pro_feed_dog++;
	
     pro_t.gTimer_pro_display_dht11_value++;
	
	 pro_t.gTimer_pro_fan++;  //fan continuce counter 60s
	
	  pro_t.gTimer_pro_time_split_symbol++;
	  pro_t.gTimer_pro_wifi_led++;

	  pro_t.gTimer_pro_timer_mode_times++;
	  pro_t.gTimer_pro_mode_key_be_select++;
	  pro_t.gTimer_pro_set_tem_value_blink++;
	 

	  pro_t.gTimer_pro_mode_key_adjust ++;
	  pro_t.gTimer_pro_power_key_adjust++;
	  pro_t.gTimer_pro_ptc_delay_time++;
	   pro_t.gTimer_pro_confir_delay++;
	
	  pro_t.gTimer_pro_mode_long_key++;
	  pro_t.gTimer_pro_action_publis++;

	    pro_t.gTimer_pro_update_dht11_data ++;
		pro_t.gTimer_pro_display_dht11_temp ++; //at once display dht11 value
		pro_t.gTimer_pro_display_dht11_hum ++;
	    pro_t.gTimer_pro_wifi_dht11_temp_hum++;

	  
	   //cotrol timer
    
	   gctl_t.gTimer_ctl_set_timer_time_senconds++;
	   gctl_t.gTimer_ctl_ptc_adc_times++;
	   gctl_t.gTimer_ctl_fan_adc_times ++;
	   gctl_t.gTimer_ctl_disp_works_time_second++;
	   gctl_t.gTimer_ctl_warning_time++;
	   gctl_t.gTimer_ctl_usart1_error++; 
	   gctl_t.gTimer_ctl_usart2_error++; 
	   gctl_t.gTimer_ctl_dma_state++ ;
	
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
		  gctl_t.gTimer_ctl_total_continue_time++;
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







