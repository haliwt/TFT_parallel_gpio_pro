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

#define RUN_POWER_4         (1 << 4)
#define RUN_MODE_5          (1 << 5)
#define RUN_DEC_6           (1 << 6)
#define RUN_ADD_7           (1 << 7)
#define VOICE_BIT_8         (1 << 8)
#define RUN_VOICE_9          (1<<9)

/*
**********************************************************************************************************
											函数声明
**********************************************************************************************************
*/
//static void vTaskTaskUserIF(void *pvParameters);
//static void vTaskLED(void *pvParameters);
static void vTaskMsgPro(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);


/*
**********************************************************************************************************
											变量声明
**********************************************************************************************************
*/
//static TaskHandle_t xHandleTaskUserIF = NULL;
//static TaskHandle_t xHandleTaskLED = NULL;
static TaskHandle_t xHandleTaskMsgPro = NULL;
static TaskHandle_t xHandleTaskStart = NULL;

//static void power_long_short_key_fun(void);

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
*   优 先 级: 3  
*********************************************************************************************************
*/
static void vTaskMsgPro(void *pvParameters)
{
   // MSG_T *ptMsg;
    BaseType_t xResult;
	const TickType_t xMaxBlockTime = pdMS_TO_TICKS(50); /* 设置最大等待时间为500ms */
	uint32_t ulValue;
   
   
	
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
             
			if((ulValue & POWER_KEY_0) != 0)
			{
   
                     
                 xTaskNotify(xHandleTaskStart, /* 目标任务 */
							RUN_POWER_4 ,            /* 设置目标任务事件标志位bit0  */
							eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/
				                                    
			}
            else if((ulValue & MODE_KEY_1) != 0){

               //switch timer timing and works timing 

                xTaskNotify(xHandleTaskStart, /* 目标任务 */
							RUN_MODE_5 ,            /* 设置目标任务事件标志位bit0  */
							eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/
               
            }   
            else if((ulValue & DEC_KEY_2) != 0){


                xTaskNotify(xHandleTaskStart, /* 目标任务 */
							RUN_DEC_6 ,            /* 设置目标任务事件标志位bit0  */
							eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

               
            }
            else if((ulValue & ADD_KEY_3) != 0){

                  xTaskNotify(xHandleTaskStart, /* 目标任务 */
							RUN_ADD_7 ,            /* 设置目标任务事件标志位bit0  */
							eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/

              
                
            }
           else if((ulValue & VOICE_BIT_8) != 0){
              
             //  xTaskNotify(xHandleTaskStart, /* 目标任务 */
                             ///          RUN_VOICE_9 ,            /* 设置目标任务事件标志位bit0  */
                              ///         eSetBits);          /* 将目标任务的事件标志位与BIT_0进行或操作，  将结果赋值给事件标志位。*/
                  v_t.sound_rx_data_success_flag = 1;

               
              
            }

      

		
		}
		else
		{
			/* 超时 */

       
        //MainBoard_Self_Inspection_PowerOn_Fun();
        if(v_t.sound_rx_data_success_flag == 1 ){
                 v_t.sound_rx_data_success_flag=0;
                  Voice_Decoder_Handler();
              
           }
                
           
            WIFI_Process_Handler();
	        USART_Cmd_Error_Handler();
             

             
         }
      
        
        
			
		}
}


/*
*********************************************************************************************************
*	函 数 名: vTaskStart
*	功能说明: 启动任务，也就是最高优先级任务，这里用作按键扫描。
*	形    参: pvParameters 是在创建该任务时传递的形参
*	返 回 值: 无
*   优 先 级: 4  
*********************************************************************************************************
*/
static void vTaskStart(void *pvParameters)
{
   BaseType_t xResult;
   const TickType_t xMaxBlockTime = pdMS_TO_TICKS(50); /* 设置最大等待时间为500ms */
   static uint8_t sound_flag,power_on_first;
   uint32_t ulValue;
   static uint8_t add_flag,dec_flag,power_sound_flag;

    while(1)
    {
		/* 按键扫描 */
		//bsp_KeyScan();

       xResult = xTaskNotifyWait(0x00000000,      
						           0xFFFFFFFF,      
						          &ulValue,        /* 保存ulNotifiedValue到变量ulValue中 */
						          xMaxBlockTime);  /* 最大允许延迟时间 */
        if( xResult == pdPASS )
		{
			/* 接收到消息，检测那个位被按下 */
			if((ulValue & RUN_POWER_4 ) != 0)
			{
				//printf("接收到K2按键按下消息, ulNotifiedValue = 0x%08x\r\n", ulValue);
				//printf("receive notice key1_bit0 \n");
				 gpro_t.key_power_be_pressed_flag =1;
            }
            else if((ulValue & RUN_MODE_5 ) != 0)   /* 接收到消息，检测那个位被按下 */
			{

                 if(gpro_t.gPower_On==power_on){
                

                  gpro_t.mode_key_pressed_flag =1;

               
              
                 }
				
            }
            else if((ulValue & RUN_DEC_6 ) != 0)   /* 接收到消息，检测那个位被按下 */
			{
                dec_flag =1;
               // Dec_Key_Fun(gkey_t.key_add_dec_mode);
               DEC_Key_Fun();

                 if(dec_flag ==1){
                     add_flag ++;
                    buzzer_sound();

                  }
                 
            }
            else if((ulValue & RUN_ADD_7 ) != 0)   /* 接收到消息，检测那个位被按下 */
			{
                   add_flag =1;
                  // Add_Key_Fun(gkey_t.key_add_dec_mode);
                  ADD_Key_Fun();

                  if(add_flag ==1){
                     add_flag ++;
                     buzzer_sound();

                  }
				
            }
            else if((ulValue & RUN_VOICE_9 ) != 0)   /* 接收到消息，检测那个位被按下 */
			{
			    
                  
            }
            

            
        }
        else {
            
              
        if(power_sound_flag==0){
          power_sound_flag++;
          VOICE_OUTPUT_SOUND_ENABLE();
          buzzer_sound();

        }

         //// power_long_short_key_fun();
         Key_Speical_Power_Fun_Handler();
              
         if(gpro_t.gPower_On==power_on){
                 bsp_Idle();
                // mode_long_short_key_fun();
                Key_Speical_Mode_Fun_Handler();

                 

         }
              
         TFT_Process_Handler();
         
         }

      }
  }

				
/*
*********************************************************************************************************
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{

	
	xTaskCreate( vTaskMsgPro,     		/* 任务函数  */
                 "vTaskMsgPro",   		/* 任务名    */
                 256,             		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 1,               		/* 任务优先级*/
                 &xHandleTaskMsgPro );  /* 任务句柄  */
	
	
	xTaskCreate( vTaskStart,     		/* 任务函数  */
                 "vTaskStart",   		/* 任务名    */
                 256,            		/* 任务栈大小，单位word，也就是4字节 */
                 NULL,           		/* 任务参数  */
                 2,              		/* 任务优先级*/
                 &xHandleTaskStart );   /* 任务句柄  */
}


void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{

 
   BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    __HAL_GPIO_EXTI_CLEAR_RISING_IT(GPIO_Pin);
 
   switch(GPIO_Pin){

   case KEY_POWER_Pin:

    if(KEY_POWER_VALUE()==KEY_DOWN){

        xTaskNotifyFromISR(xHandleTaskMsgPro,  /* 目标任务 */
        POWER_KEY_0,      /* 设置目标任务事件标志位bit0  */
        eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
        &xHigherPriorityTaskWoken);

        /* Èç¹ûxHigherPriorityTaskWoken = pdTRUE£¬ÄÇÃ´ÍË³öÖÐ¶ÏºóÇÐµ½µ±Ç°×î¸ßÓÅÏÈ¼¶ÈÎÎñÖ´ÐÐ */
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);


    }
            
   
   break;

   case KEY_MODE_Pin:
      if(KEY_MODE_VALUE() == KEY_DOWN){
        xTaskNotifyFromISR(xHandleTaskMsgPro,  /* 目标任务 */
               MODE_KEY_1,     /* 设置目标任务事件标志位bit0  */
               eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
               &xHigherPriorityTaskWoken);

        /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);


       }
   
   break;


   case KEY_ADD_Pin:
    
         xTaskNotifyFromISR(xHandleTaskMsgPro,  /* 目标任务 */
                ADD_KEY_3,     /* 设置目标任务事件标志位bit0  */
                eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                &xHigherPriorityTaskWoken);
   
         /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
         portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
       
   break;

   case KEY_DEC_Pin:

       
        xTaskNotifyFromISR(xHandleTaskMsgPro,  /* 目标任务 */
                DEC_KEY_2,     /* 设置目标任务事件标志位bit0  */
                eSetBits,  /* 将目标任务的事件标志位与BIT_0进行或操作， 将结果赋值给事件标志位 */
                &xHigherPriorityTaskWoken);
   
         /* 如果xHigherPriorityTaskWoken = pdTRUE，那么退出中断后切到当前最高优先级任务执行 */
         portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

   break;


   }



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
    static uint8_t state=0,state_uart1,voice_cmd_time = 0xff,voice_wakewor_int=0xff;
    uint32_t temp ;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    //wifi usart2
    if(huart->Instance==USART2)
    {
           
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
				else if(wifi_t.get_rx_auto_repeat_net_enable ==1){

					wifi_t.wifi_data[wifi_t.wifi_uart_counter] = wifi_t.usart2_dataBuf[0];
					wifi_t.wifi_uart_counter++;

					if(*wifi_t.usart2_dataBuf==0X0A) // 0x0A = "\n"
					{
						
						Wifi_Rx_Auto_Link_Net_Handler();
						wifi_t.wifi_uart_counter=0;
					}


				}
				else{
					Subscribe_Rx_Interrupt_Handler();

				}
	      }
     
	      
	  HAL_UART_Receive_IT(&huart2,wifi_t.usart2_dataBuf,1);
	  
//	__HAL_UART_CLEAR_NEFLAG(&huart2);
//	__HAL_UART_CLEAR_FEFLAG(&huart2);
//	__HAL_UART_CLEAR_OREFLAG(&huart2);
//	__HAL_UART_CLEAR_TXFECF(&huart2);
     
	}

 //voice sound by USART1
  if(huart->Instance==USART1){

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
	 
	  
    //  __HAL_UART_CLEAR_NEFLAG(&huart1);
    //  __HAL_UART_CLEAR_FEFLAG(&huart1);
      __HAL_UART_CLEAR_OREFLAG(&huart1);
    //  __HAL_UART_CLEAR_TXFECF(&huart1);


    HAL_UART_Receive_IT(&huart1,voice_inputBuf,1);//UART receive data interrupt 1 byte

	}
	
}



