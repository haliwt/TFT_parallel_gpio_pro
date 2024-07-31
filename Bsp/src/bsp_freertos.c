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
static void vTaskRunPro(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);


/*
**********************************************************************************************************
											变量声明
**********************************************************************************************************
*/
//static TaskHandle_t xHandleTaskUserIF = NULL;
static TaskHandle_t xHandleTaskRunPro = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;


uint8_t add_key_counter,dec_key_counter;

uint32_t mode_key_long_conter;
uint32_t power_key_long_conter;

uint32_t add_dec_combin_counter;

uint8_t key_power_sound_flag;





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
*	函 数 名: vTaskRunPro
*	功能说明: 使用函数xTaskNotifyWait接收任务vTaskTaskUserIF发送的事件标志位设置
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 1  
*********************************************************************************************************
*/
static void vTaskRunPro(void *pvParameters)
{

   static uint8_t power_sound_flag;

   while(1)
   {


     if(power_sound_flag==0){
       power_sound_flag++;
      
       buzzer_sound();
       HAL_Delay(100);
       VOICE_OUTPUT_SOUND_ENABLE();
     }

     
      if(gpro_t.gPower_On == power_on && (key_power_sound_flag !=3)){

      
         PowerOn_Process_Handler();
         Temperature_Ptc_Pro_Handler();
        
         Wifi_Fast_Led_Blink();
       
         detection_net_link_state_handler();
          //update data to tencent cloud.

          disp_all_led_on_off_state();

          smart_phone_power_on_to_tencent_data();
        
          WIFI_Process_Handler();

          
         //update data to tencent cloud.
          if(gpro_t.gTimer_pro_update_dht11_data > 11  && wifi_link_net_state() ==1){
              gpro_t.gTimer_pro_update_dht11_data=0;
      
               Update_Dht11_Totencent_Value();
      
          }
       
      }
      bsp_run_Idle();
      
      MainBoard_Self_Inspection_PowerOn_Fun();
    
      USART_Cmd_Error_Handler();
      clear_rx_copy_data();
     
      vTaskDelay(30);////100// 40
  }
	
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
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(50); /* 设置最大等待时间为50ms */
	uint32_t ulValue;
    static uint8_t key_add_sound_flag,key_dec_sound_flag,key_mode_short_sound_flag;
    static uint8_t key_mode_long_sound_flag,key_power_long_sound_flag;
    static uint8_t add_dec_combin,key_power_off_sound_flag ;
	
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
		
		xResult = xTaskNotifyWait(0x00000000,      
						          0xFFFFFFFF,      
						          &ulValue,        /* 保存ulNotifiedValue到变量ulValue中 */
						          xMaxBlockTime);  /* 最大允许延迟时间 */
		
		if( xResult == pdPASS )
		{
			/* 接收到消息，检测那个位被按下 */
             
			if((ulValue & POWER_KEY_0) != 0){
			
                if(key_power_long_sound_flag !=3){
			   	  key_power_sound_flag =1;//gpro_t.key_power_be_pressed_flag =1;
                 }
                 power_key_long_conter =0;
                 mode_key_long_conter = 0;
                 add_dec_combin_counter=0;

            }
            else if((ulValue & POWER_LONG_KEY_11) != 0){

              if(gpro_t.gPower_On==power_on){

                key_power_long_sound_flag =1;

              }
               add_dec_combin_counter=0;
               mode_key_long_conter = 0;
            }
            else if((ulValue &  POWER_ON_5) != 0){

               key_power_sound_flag =1;//gpro_t.key_power_be_pressed_flag =1;
                            
                power_key_long_conter =0;
                mode_key_long_conter = 0;
                add_dec_combin_counter=0;
                
            }
            else if((ulValue &  POWER_OFF_4) != 0){

               
                  key_power_off_sound_flag =1;           
                  power_key_long_conter =0;
                  mode_key_long_conter = 0;
                  add_dec_combin_counter=0;
                  gpro_t.gPower_On = power_off;


            }
            else if((ulValue & MODE_KEY_1) != 0){


                 if(gpro_t.gPower_On==power_on){
                
                    if(key_mode_long_sound_flag != 3){
                       key_mode_short_sound_flag =1;

                    }
                    
                      mode_key_long_conter=0;
                      power_key_long_conter =0;
                      add_dec_combin_counter=0;
                 }
               
            }
            else if((ulValue & MODE_LONG_KEY_10) != 0){
                
                if(gpro_t.gPower_On==power_on){
                           
                    key_mode_long_sound_flag = 1;
                    
                 }
           }
            else if((ulValue & DEC_KEY_2) != 0){

                 if(gpro_t.gPower_On==power_on){
                      power_key_long_conter=0;
                      mode_key_long_conter = 0;
                    
                       key_dec_sound_flag=1;

                   }

               
            }
            else if((ulValue & ADD_KEY_3) != 0){
                if(gpro_t.gPower_On==power_on){
                      power_key_long_conter=0;
                      mode_key_long_conter = 0;
                    
                       key_add_sound_flag=1;

                    }           
                
            }
           else if((ulValue & VOICE_BIT_8) != 0){
              

                v_t.sound_rx_data_success_flag = 1;
               
            }
            else if((ulValue & ADD_DEC_COMBIN_KEY_12) != 0){

              //   buzzer_sound();
                add_dec_combin=1;
                gpro_t.gTimer_exit_mode_long_key=0;
                gctl_t.disp_ntc_res_switch_normal_ptc_counter ++;
                if(gctl_t.disp_ntc_res_switch_normal_ptc_counter > 1){
                   gctl_t.disp_ntc_res_switch_normal_ptc_counter =0;

                }
                gctl_t.gTimer_ctl_ptc_adc_times =20;
                power_key_long_conter=0;
                mode_key_long_conter = 0;
                     
                
               
            }
           
       }
	   else{
		         
       
        
       if(key_dec_sound_flag ==1 || key_add_sound_flag ==1 || key_mode_short_sound_flag == 1 || key_mode_long_sound_flag ==1 \
           || key_power_sound_flag ==1 || key_power_long_sound_flag==1 || key_power_off_sound_flag ==1){


             if(key_power_sound_flag == 1){

               key_power_sound_flag ++;
                DISABLE_INT();
                LCD_Clear(BLACK);
                ENABLE_INT();

                buzzer_sound();

              }
              else if(key_mode_short_sound_flag== 1){

                key_mode_short_sound_flag  ++;
                buzzer_sound();
    

              }
              else if(key_mode_long_sound_flag == 1){

                   key_mode_long_sound_flag++;
                   buzzer_sound();


              }
              else if(key_power_off_sound_flag ==1){
                  key_power_off_sound_flag ++;
                   DISABLE_INT();
                   LCD_Clear(BLACK);
                   ENABLE_INT();
                  buzzer_sound();

                  power_off_init_set_ref();
                  power_off_handler();


              }
              else if(key_dec_sound_flag == 1){
                 key_dec_sound_flag++;
                 buzzer_sound();

              }
              else if(key_add_sound_flag ==1){
                
                  key_add_sound_flag++;
                  buzzer_sound();


              }
              else if(key_power_long_sound_flag ==1){

                  key_power_long_sound_flag ++;
                  buzzer_sound();

              }
          
          }

           
          if(key_power_sound_flag==2){//run_main_board_process();
           

              if(wifi_t.smartphone_app_power_on_flag==1){
                    wifi_t.smartphone_app_power_on_flag++;
                    gpro_t.power_on_or_off_flag = power_on;
                    gpro_t.run_process_step=0;
               
               }
              else{
               PowerOnOff_Init_Ref_Fun();

              }
             key_power_sound_flag++;
            
         }


         if(key_power_sound_flag==3){
            key_power_sound_flag++;

            if(gpro_t.power_on_or_off_flag == power_on){
               gpro_t.gPower_On = power_on;
               power_on_init_set_ref();
               if(wifi_t.smartphone_app_power_on_flag==2){
                   wifi_t.smartphone_app_power_on_flag++;
                   Device_Action_No_Wifi_Handler(); //smart phone app is power on .

               }
            

            }
            else{

                gpro_t.gPower_On = power_off;


            }

            if(wifi_t.smartphone_app_power_on_flag==0 && gpro_t.gPower_On == power_on){
                power_on_action_led_init();
            }
                

          }
          else if(gpro_t.gTimer_exit_mode_long_key > 1 && (key_power_long_sound_flag  == 3 ||  key_mode_long_sound_flag==3 ||  add_dec_combin==1)){

                  if(key_power_long_sound_flag ==3){
                      power_key_long_conter =0; //clear power key loong flag .
                     key_power_long_sound_flag  = 0;


                  }
                   
                 if(key_mode_long_sound_flag==3){
                      key_mode_long_sound_flag =0;
                     mode_key_long_conter =0;

                   }

                  if(add_dec_combin==1){
                     add_dec_combin =0;
                     buzzer_sound();
                    add_dec_combin_counter =0;
                     if(gctl_t.disp_ntc_res_switch_normal_ptc_counter ==0){
                        donot_display_ntc_temp_value();

                     }


                  }
           }

            


             if(gpro_t.gPower_On==power_on){
                 
              
                if(key_mode_short_sound_flag==2){
                    key_mode_short_sound_flag++;

                    Mode_Key_Selection_Func() ;

                }
                else if(key_mode_long_sound_flag==2){
                    key_mode_long_sound_flag ++;

                    Mode_Key_Long_Fun();


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
                else if(key_power_long_sound_flag == 2){
                     key_power_long_sound_flag ++;

                      PowerOn_LongKey_Fun();

                }

              Mode_Key_Config_Fun_Handler();

              
              WIFI_LED_Blink();
           
              Wifi_Fast_Led_Blink();

              TFT_Disp_Timer_Split_Symbol();

              TimeTimer_Pro_Handler();

              SetPtc_TempComare_Value();

            //  WIFI_Process_Handler();
            
   

          }
         else if(gpro_t.gPower_On == power_off){
            mode_key_long_conter  =0;
            power_key_long_conter = 0xff;
            key_power_sound_flag=0;
            Power_Off_Process_Handler();
         }

         if(v_t.sound_rx_data_success_flag == 1 ){
               v_t.sound_rx_data_success_flag=0;
         
               Voice_Decoder_Handler();
         
          }

          wifi_get_beijint_time_handler();
          
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
    if(KEY_POWER_VALUE() == KEY_DOWN){
        while(KEY_POWER_VALUE() == KEY_DOWN && power_key_long_conter < 2965500){

               power_key_long_conter++;
               if(power_key_long_conter > 2950000){
                   power_key_long_conter = 2965900;

                xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
                            POWER_LONG_KEY_11,            /* 设置目标任务事件标志位bit0  */
                            eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

                }

         }


         if(power_key_long_conter < 2950000 ){

             
               xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
					 POWER_KEY_0,            /* 设置目标任务事件标志位bit0  */
					 eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/


         }
				                                    


     }
     else if(KEY_MODE_VALUE() == KEY_DOWN){
      
       while(KEY_MODE_VALUE() == KEY_DOWN && mode_key_long_conter < 2965500){

               mode_key_long_conter++;
               if(mode_key_long_conter > 2950000){
                   mode_key_long_conter = 2965900;
               
               xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
                         MODE_LONG_KEY_10,            /* 设置目标任务事件标志位bit0  */
                         eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

                }


         }
         
         if(mode_key_long_conter < 2950000 ){
            
           xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
                         MODE_KEY_1,            /* 设置目标任务事件标志位bit0  */
                         eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

          }


     }
     else if(KEY_ADD_VALUE() == KEY_DOWN && KEY_DEC_VALUE() == KEY_UP){
        
          
          xTaskNotify(xHandleTaskMsgPro, /* 目标任务 */
                         ADD_KEY_3,            /* 设置目标任务事件标志位bit0  */
                         eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/


     }
     else if(KEY_DEC_VALUE() == KEY_DOWN && KEY_ADD_VALUE() == KEY_UP){
          
       
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

	xTaskCreate( vTaskRunPro,     		/* 任务函数  */
                 "vTaskRunPro",   		/* 任务名    */
                 128,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 1,               		/* 任务优先级最低*/
                 &xHandleTaskRunPro);  /* 任务句柄  */



    xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
                 "vTaskMsgPro",   		/* 任务名    */
                 128,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 2,               		/* 任务优先级次子*/
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务名    */
                 128,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 3,              		/* 任务优先级最高*/
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
       // taskENTER_CRITICAL();

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




