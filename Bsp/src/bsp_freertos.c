#include "bsp.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"



/*
**********************************************************************************************************
											宏定义
**********************************************************************************************************
*/
#define POWER_KEY_0	        (1 << 0)
#define MODE_KEY_1	        (1 << 1)
#define DEC_KEY_2           (1 << 2)
#define ADD_KEY_3           (1 << 3)

#define POWER_OFF_4         (1 << 4)
#define POWER_ON_5          (1 << 5)
#define POWER_ON_APP_6        (1<< 6)
#define VOICE_BIT_8         (1 << 8)

//long key define 
#define MODE_LONG_KEY_10             (1 << 10)
#define POWER_LONG_KEY_11            (1 << 11)
#define ADD_DEC_COMBIN_KEY_12        (1 << 12)    


/*
**********************************************************************************************************
											函数声明
**********************************************************************************************************
*/
//static void vTaskTaskUserIF(void *pvParameters);
//static void vTaskRunPro(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);


/*
**********************************************************************************************************
											变量声明
**********************************************************************************************************
*/
//static TaskHandle_t xHandleTaskUserIF = NULL;
//static TaskHandle_t xHandleTaskRunPro = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;


uint8_t add_key_counter,dec_key_counter;


uint32_t start_counter_power_key_long_pressed;

uint32_t add_dec_combin_counter;

uint8_t key_power_sound_flag,gwifi_key_flag;

uint8_t gpower_onoff_key_flag,gmode_key_flag  ;

volatile uint8_t buzzer_sound_flag,key_power_long_sound_flag;


/**********************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口。
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
void freeRTOS_Handler(void)
{
	/* 创建任务 */
	AppTaskCreate();

	/* 创建任务通信机制 */
//	AppObjCreate();
	
    /* 启动调度，开始执行任务 */
    vTaskStartScheduler();
}

/*
*********************************************************************************************************
*	函 数 名: vTaskMsgPro
*	功能说明: 使用函数xTaskNotifyWait接收任务vTaskTaskUserIF发送的事件标志位设置
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 2  
*********************************************************************************************************
*/
static void vTaskMsgPro(void *pvParameters)
{
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(30); /* 设置最大等待时间为50ms */
	uint32_t ulValue;
    static uint8_t key_add_sound_flag,key_dec_sound_flag;
    static uint8_t add_dec_combin,key_power_off_sound_flag ;
	static uint8_t power_sound_flag;
    while(1)
    {
		/*
			第一个参数 ulBitsToClearOnEntry的作用（函数执行前）：
		          ulNotifiedValue &= ~ulBitsToClearOnEntry
		          简单的说就是参数ulBitsToClearOnEntry那个位是1，那么notification value
		          的那个位就会被清零。

		          这里ulBitsToClearOnEntry = 0x00000000就是函数执行前保留所有位。
		
		    第二个参数 ulBitsToClearOnExit的作用（函数退出前）：			
				  ulNotifiedValue &= ~ulBitsToClearOnExit
		          简单的说就是参数ulBitsToClearOnEntry那个位是1，那么notification value
		          的那个位就会被清零。

				  这里ulBitsToClearOnExi = 0xFFFFFFFF就是函数退出前清楚所有位。
		
		    注：ulNotifiedValue表示任务vTaskMsgPro的任务控制块里面的变量。		
		*/

     if(power_sound_flag==0){
       power_sound_flag++;
      
       buzzer_sound();
      
       Update_DHT11_Value();
          
      TFT_Disp_Temp_Value(0,gctl_t.dht11_temp_value);
      TFT_Disp_Humidity_Value(gctl_t.dht11_hum_value);
      TFT_Display_Handler();
      HAL_Delay(100);
      VOICE_OUTPUT_SOUND_ENABLE();
     }
		
		xResult = xTaskNotifyWait(0x00000000,      
						          0xFFFFFFFF,      
						          &ulValue,        /* 保存ulNotifiedValue到变量ulValue中 */
						          xMaxBlockTime);  /* 最大允许延迟时间 */
		
		if( xResult == pdPASS )
		{
			/* 接收到消息，检测那个位被按下 */
             
		  if((ulValue & POWER_LONG_KEY_11) != 0){

              if(gpro_t.gPower_On==power_on){

                key_power_long_sound_flag =1;
                gpower_onoff_key_flag=1;
                gwifi_key_flag =1;
                buzzer_sound(); //WT.EDIT 2024.08.15
              
                

              }
               add_dec_combin_counter=0;
           
          
              gpro_t.gTimer_exit_mode_long_key =0;
            }
            else if((ulValue &  POWER_ON_5) != 0){

              // key_power_sound_flag =1;//gpro_t.key_power_be_pressed_flag =1;
                            
                gpower_onoff_key_flag =1;
            
                add_dec_combin_counter=0;
                 
                
            }
            else if((ulValue &  POWER_OFF_4) != 0){

               
                  key_power_off_sound_flag =1;           
                 // start_counter_power_key_long_pressed =0;
                
                  add_dec_combin_counter=0;
                  gpro_t.gPower_On = power_off;


            }
            else if((ulValue & MODE_KEY_1) != 0){


                 if(gpro_t.gPower_On==power_on){
                
                    gpro_t.key_mode_be_pressed_flag=1;

                  }
                     
                   start_counter_power_key_long_pressed =0;
                   add_dec_combin_counter=0;
                  
                    
            }
            else if((ulValue & DEC_KEY_2) != 0){

                 if(gpro_t.gPower_On==power_on){
                      start_counter_power_key_long_pressed=0;
                      key_dec_sound_flag=1;
                 }

               
            }
            else if((ulValue & ADD_KEY_3) != 0){
                if(gpro_t.gPower_On==power_on){
                      start_counter_power_key_long_pressed=0;
                   
                    
                       key_add_sound_flag=1;
                     

                    }           
                
            }
           else if((ulValue & VOICE_BIT_8) != 0){
              

                v_t.sound_rx_data_success_flag = 1;
               
            }
            else if((ulValue & ADD_DEC_COMBIN_KEY_12) != 0){

              //   buzzer_sound();
                add_dec_combin=1;

                gctl_t.disp_ntc_res_switch_normal_ptc_counter ++;
                if(gctl_t.disp_ntc_res_switch_normal_ptc_counter > 1){
                   gctl_t.disp_ntc_res_switch_normal_ptc_counter =0;

                }
                gctl_t.gTimer_ctl_ptc_adc_times =20;
                start_counter_power_key_long_pressed=0;
               
                gpro_t.gTimer_exit_mode_long_key =0;
                
               
            }
           
       }
	   else{  //over times run this below .


         if(gpower_onoff_key_flag ==1){

             if(KEY_POWER_VALUE() == KEY_UP){

                 gpower_onoff_key_flag++;
                 if(gwifi_key_flag==1){
                     gwifi_key_flag++;

                     PowerOn_LongKey_Fun();
                               
                     direct_wifi_led_fast_blink_handler();
                     Wifi_Fast_Led_Blink();

                }
                else{
                   gpower_onoff_key_flag++;
                    start_counter_power_key_long_pressed=0; //WT.EIDT .2024.12.27
                   buzzer_sound_flag =1;
                   LCD_Clear(BLACK);
                   if(wifi_t.smartphone_app_power_on_flag==1){
                    wifi_t.smartphone_app_power_on_flag++;
                    gpro_t.gPower_On = power_on;//gpro_t.power_on_or_off_flag = power_on;
                    gpro_t.run_process_step=0;
                    gpro_t.fan_run_the_first_flag++; //WT.EDIT .2024.12.23
                    wifi_app_power_on_handler();
               
                   }
                   else{
                    PowerOnOff_Init_Ref_Fun();

                   }
                   Buzzer_KeySound();
                  // buzzer_sound_flag =1;


                 }

             }
            
           }
           else if(gmode_key_flag == 1){ //WT.EDIT 2024.08.13

               if(KEY_MODE_VALUE() == KEY_UP){
                gmode_key_flag ++ ;

                if( gpro_t.key_long_mode_flag ==1){
                     gpro_t.key_short_mode_flag  ++;
                     gpro_t.long_key_mode_counter=0;
                   
                     Mode_Key_Long_Fun();
                       
                }
                else{
                    gpro_t.long_key_mode_counter=0;
                   // buzzer_sound_flag = 1;
                  
                    gpro_t.key_mode_be_pressed_flag =1;
                   // mode_key_adjust_fun();
                   Buzzer_KeySound();

                   gpro_t.key_short_mode_flag = 1;


                }
                

             }
      
         }

           
        

		         
         if(key_dec_sound_flag ==1 || key_add_sound_flag ==1  || key_power_off_sound_flag ==1){


           if(key_power_off_sound_flag ==1){
                  key_power_off_sound_flag ++;
                  DISABLE_INT(); 
                 
                   LCD_Clear(BLACK);
                   buzzer_sound();//WT.EDIT 2024.08.17 
                  ENABLE_INT();
              
                 // power_off_init_set_ref();
                  power_off_handler();
                 // buzzer_sound_flag = 1;//WT.EDIT 2024.08.17 
                 


              }
              else if(key_dec_sound_flag == 1){
                 key_dec_sound_flag++;
                 Buzzer_KeySound();
                 

              }
              else if(key_add_sound_flag ==1){
                
                  key_add_sound_flag++;
                  Buzzer_KeySound();
                  


              }
             }

          
          

            if(gpro_t.gTimer_exit_mode_long_key > 2 && (add_dec_combin==1)){

                  if(add_dec_combin==1){
                     add_dec_combin =0;
                     buzzer_sound();
                    add_dec_combin_counter =0;
                     if(gctl_t.disp_ntc_res_switch_normal_ptc_counter ==0){
                        donot_display_ntc_temp_value();

                     }
                  }
           }

          
           /**********************run power on*****************************/
           if(gpro_t.gPower_On==power_on){
         
                if(gpro_t.key_short_mode_flag==1){
               
                    gpro_t.key_short_mode_flag++;

                    Mode_Key_Selection_Func() ;

                }
                else if(key_dec_sound_flag==2 || key_add_sound_flag==2){
                     if(key_dec_sound_flag==2){
                         key_dec_sound_flag++;
                         DEC_Key_Fun();

                      }

                    if(key_add_sound_flag==2){

                       key_add_sound_flag++;

                        ADD_Key_Fun();


                     }

                    if(gpro_t.disp_key_set_temp_value ==1){ //add and dec of key input number on TFT screen
                		 gpro_t.disp_key_set_temp_value =0;
                        TFT_Disp_Temp_Value(0,gctl_t.gSet_temperature_value);

                    }
                	else if(gpro_t.disp_key_set_timer_timing  ==1){
                		      gpro_t.disp_key_set_timer_timing =0;
                	  TFT_Disp_Set_TimerTime(0);
                    }
                }
              Mode_Key_Config_Fun_Handler();
              
              wifi_detected_signal_handler(gpro_t.wifi_led_fast_blink_flag);
              SetPtc_TempComare_Value();
              disp_all_led_on_off_state();
              tft_disp_time_colon_symbol();
              Wifi_Fast_Led_Blink();
         
        }
        else if(gpro_t.gPower_On == power_off){
     
            start_counter_power_key_long_pressed = 0;
            key_power_sound_flag=0;
            Power_Off_Process_Handler();
         }

        if(v_t.sound_rx_data_success_flag == 1 ){
             v_t.sound_rx_data_success_flag=0;
         
             Voice_Decoder_Handler();
         
         }
         Wifi_Fast_Led_Blink();
         if(gpro_t.wifi_led_fast_blink_flag==0){
             wifi_get_beijint_time_handler();
             MainBoard_Self_Inspection_PowerOn_Fun();
         }
         TFT_DonnotDisp_Works_Time();
         USART_Cmd_Error_Handler();
         Wifi_Fast_Led_Blink();
         clear_rx_copy_data();
          
        }
             
    }
      
}

/**********************************************************************************************************
*	函 数 名: vTaskStart
*	功能说明: 启动任务，也就是最高优先级任务，这里用作按键扫描。
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 3  
**********************************************************************************************************/
static void vTaskStart(void *pvParameters)
{
   //BaseType_t xResult;
   ///const TickType_t xMaxBlockTime = pdMS_TO_TICKS(50); /* 设置最大等待时间为500ms */

 
    while(1)
    {
		/* 按键扫描 */
		//bsp_KeyScan();


    if(KEY_POWER_VALUE() == KEY_DOWN ){
        gpro_t.long_key_mode_counter=0;

        start_counter_power_key_long_pressed++;

        if(start_counter_power_key_long_pressed > 60 && gpro_t.gPower_On==power_on){
          start_counter_power_key_long_pressed =0;
          gwifi_key_flag =1;
           buzzer_sound();


       }

       gpower_onoff_key_flag =1;


     }
     else if(KEY_MODE_VALUE() == KEY_DOWN   && KEY_POWER_VALUE() == KEY_UP){
         
         
          start_counter_power_key_long_pressed=0;

         if(gpro_t.gPower_On == power_on){
      
             gpro_t.long_key_mode_counter ++ ;

          if(gpro_t.long_key_mode_counter > 70  ){
             gpro_t.long_key_mode_counter=0;   
               gpro_t.key_long_mode_flag =1;
               gpro_t.gTimer_pro_set_long_key_tims=0;
            
                Buzzer_KeySound();
                
          }
             
    
//            xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
//                         MODE_KEY_1,            /* 设置目标任务事件标志位bit0  */
//                         eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/
   
       gmode_key_flag = 1;


      }
        

    }
    else if(KEY_ADD_VALUE() == KEY_DOWN && KEY_DEC_VALUE() == KEY_UP){
        gpro_t.long_key_mode_counter=0;
        start_counter_power_key_long_pressed=0;

       
          xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
                         ADD_KEY_3,            /* 设置目标任务事件标志位bit0  */
                         eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

         


     }
     else if(KEY_DEC_VALUE() == KEY_DOWN && KEY_ADD_VALUE() == KEY_UP){
            gpro_t.long_key_mode_counter=0;
            start_counter_power_key_long_pressed=0;
            
              xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
                              DEC_KEY_2,            /* 设置目标任务事件标志位bit0  */
                              eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

            


     }
     else if(KEY_ADD_VALUE() == KEY_DOWN && KEY_DEC_VALUE() == KEY_DOWN && add_dec_combin_counter <220){

         
            add_dec_combin_counter++;
             if(add_dec_combin_counter > 117){
                   add_dec_combin_counter = 230;

              xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
                            ADD_DEC_COMBIN_KEY_12,          /* 设置目标任务事件标志位bit0  */
                            eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

             }
            

     }

    

    vTaskDelay(20);
  }
}
/**********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：无
*	返 回 值: 无
**********************************************************************************************************/
static void AppTaskCreate (void)
{

//	xTaskCreate( vTaskRunPro,     		/* 任务函数  */
//                 "vTaskRunPro",   		/* 任务名    */
//                 128,             		/* 任务栈大小，单位word，也就是4字节 */
//                 NULL,           		/* 任务参数  */
//                 1,               		/* 任务优先级最低*/
//                 &xHandleTaskRunPro);  /* 任务句柄  */



    xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
                 "vTaskMsgPro",   		/* 任务名    */
                 256,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 1,               		/* 任务优先级次子*/
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务名    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 2,              		/* 任务优先级最高*/
                 &xHandleTaskStart );   /* 任务句柄  */
}


/********************************************************************************
	**
	*Function Name:void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
	*Function :UART callback function  for UART interrupt for receive data
	*Input Ref: structure UART_HandleTypeDef pointer
	*Return Ref:NO
	*
*******************************************************************************/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    static uint8_t state_uart1 ;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    //wifi usart2
    if(huart->Instance==USART2)
    {
         DISABLE_INT();
        // taskENTER_CRITICAL();
        if(wifi_t.linking_tencent_cloud_doing  ==1){ //link tencent netware of URL

			wifi_t.wifi_data[wifi_t.wifi_uart_counter] = wifi_t.usart2_dataBuf[0];
			wifi_t.wifi_uart_counter++;

			if(*wifi_t.usart2_dataBuf==0X0A) // 0x0A = "\n"
			{
				//wifi_t.usart2_rx_flag = 1;
				Wifi_Rx_Link_Net_InputInfo_Handler();
				wifi_t.wifi_uart_counter=0;
			}

	      } 
		  else{

		      if(wifi_t.get_rx_beijing_time_enable==1){
					wifi_t.wifi_data[wifi_t.wifi_uart_counter] = wifi_t.usart2_dataBuf[0];
					wifi_t.wifi_uart_counter++;
				}
				else{
					Subscribe_Rx_Interrupt_Handler();

				}
	      }
	  
	 ENABLE_INT();
	//  taskEXIT_CRITICAL();
	//__HAL_UART_CLEAR_NEFLAG(&huart2);
	//__HAL_UART_CLEAR_FEFLAG(&huart2);
	__HAL_UART_CLEAR_OREFLAG(&huart2);
	//__HAL_UART_CLEAR_TXFECF(&huart2);

      HAL_UART_Receive_IT(&huart2,wifi_t.usart2_dataBuf,1);
	}

 //voice sound by USART1
  if(huart->Instance==USART1){

       DISABLE_INT();
       

    	switch(state_uart1)
		{
		case 0:  //#0
		  
		    v_t.voice_rxBuf[0]=voice_inputBuf[0];
			if(v_t.voice_rxBuf[0]==0xA5){  //hex :4D - "M" -fixed mainboard
				state_uart1=1; //=1
              
			}
			
			break;
		case 1: //#1
		     v_t.voice_rxBuf[1]=voice_inputBuf[0];
			if(v_t.voice_rxBuf[1]==0xFA) //hex : 41 -'A'  -fixed master
			{
				state_uart1=2; 
			}
			else
				state_uart1=0; 
			break;

	   case 2:
           v_t.voice_rxBuf[2]=voice_inputBuf[0];
	      if(v_t.voice_rxBuf[2]==0) //hex : 41 -'A'	-fixed master
		   {
			   state_uart1=3; 
		   }
		   else{
			  state_uart1=0; 
		   }


	   break;

	   case 3:

           v_t.voice_rxBuf[3]=voice_inputBuf[0];
	      if(v_t.voice_rxBuf[3]==0x81) //hex : 41 -'A'	-fixed master
		   {
			  
			   state_uart1=4; 
		   }
		   else{
			  state_uart1=0; 
			 }


	   break;

	   case 4:

        v_t.voice_rxBuf[4]=voice_inputBuf[0];
      
	    if(v_t.voice_rxBuf[4]==0x01){
		 	 
			 v_t.voice_wakeword_enable=1;
			 v_t.gTimer_voice_time_counter_start =0;
             v_t.voice_data[0]=  v_t.voice_rxBuf[4];
		      state_uart1=5;
		    
		 }
		 else if(v_t.voice_wakeword_enable==1){
            if(  v_t.voice_rxBuf[4] >0 &&   v_t.voice_rxBuf[4] < 0x40) //hex : 41 -'A'	-fixed master
		   {

               v_t.voice_data[0]=  v_t.voice_rxBuf[4]; //voice data4 + data6
			   state_uart1=5;
		   }
		   else
			  state_uart1=0; 
		  
    	    }
		  else{

		   state_uart1=0; 
		
         }
		 

	  break;

	   case 5:
	     v_t.voice_rxBuf[5]=voice_inputBuf[0];
	   if(v_t.voice_rxBuf[5]==0x00) //hex : 41 -'A' -fixed master
		{
		  state_uart1=6; 
		}
		else
			state_uart1=0; 
	   

	   break;

	   case 6:
         v_t.voice_rxBuf[6]=voice_inputBuf[0];
         v_t.voice_data[1]=v_t.voice_rxBuf[6];
		 state_uart1=7; 
	   break;

	   case 7:
	   	 v_t.voice_rxBuf[7]=voice_inputBuf[0];
	     if(v_t.voice_rxBuf[7]==0xFB){ //hex : 41 -'A'	-fixed master
		  
           state_uart1=0; 
            
           xTaskNotifyFromISR(xHandleTaskMsgPro,  /* 目标任务 */
                   VOICE_BIT_8,      /* 设置目标任务事件标志位bit0  */
                   eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                   &xHigherPriorityTaskWoken);
            
                   /* Èç¹ûxHigherPriorityTaskWoken = pdTRUE£¬ÄÇÃ´ÍË³öÖÐ¶ÏºóÇÐµ½µ±Ç°×î¸ßÓÅÏÈ¼¶ÈÎÎñÖ´ÐÐ */
             portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            
			

		  }
		  else{

			  state_uart1=0; 

		  }
		  
	   
		 
     break;

	  
	}
	 
	ENABLE_INT(); 
	 //taskEXIT_CRITICAL();
    //  __HAL_UART_CLEAR_NEFLAG(&huart1);
    //  __HAL_UART_CLEAR_FEFLAG(&huart1);
     __HAL_UART_CLEAR_OREFLAG(&huart1);
    //  __HAL_UART_CLEAR_TXFECF(&huart1);


    HAL_UART_Receive_IT(&huart1,voice_inputBuf,1);//UART receive data interrupt 1 byte

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
       //DISABLE_INT();

      // taskENTER_CRITICAL_FROM_ISR();
       v_t.transOngoingFlag=0; //UART Transmit interrupt flag =0 ,RUN
      // //ENABLE_INT();
      // taskEXIT_CRITICAL_FROM_ISR(0);
	}



}
/*****************************************************************************
 * 
 * Function Name: void App_PowerOff_Handler(void)
 * Function:
 * Input Ref: NO
 * Return Ref: NO
 * 
*****************************************************************************/
void App_PowerOff_Handler(void)
{
     
     xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
	 POWER_OFF_4 ,            /* 设置目标任务事件标志位bit0  */
	 eSetBits);             /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/
     

}


void App_PowerOn_Handler(void)
{
    
     xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
	 POWER_ON_5 ,            /* 设置目标任务事件标志位bit0  */
	 eSetBits);             /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/
     

}




