#include "bsp_key.h"
#include "bsp.h"


#define KEY_POWER_LONG_DOWN     2
#define KEY_MODE_LONG_DOWN      2



key_types key_t;
uint8_t power_key_detected;
uint16_t key_mode_counter;




uint32_t  K1=0;
uint32_t  K2=0;

uint16_t  K3=0;
uint16_t  K4=0;


uint16_t cnt;
uint8_t value1 = 0;
uint8_t value2 = 0;
uint8_t value3 = 0;
uint8_t value4 = 0;

/**************************************************************
	*
	*Function Name:uint8_t KEY_Scan(void)
	*
	*
	*
	*
**************************************************************/
#if NORMAL_KEY_3
uint8_t KEY_Scan(void)
{

   uint8_t  reval = 0;
 
  key_t.read = _KEY_ALL_OFF; //0xFF 
  
  
    if(POWER_KEY_VALUE() ==KEY_DOWN )// high level
    {
        key_t.read &= ~0x01; // 0xff & 0xfe =  0xFE
    }
//    if(DEC_KEY_VALUE()==KEY_DOWN )
//	{
//		  key_t.read &= ~0x04; // 0xFf & 0xfB =  0xFB
//	}
//    else if(ADD_KEY_VALUE() ==KEY_DOWN )
//	{
//		  key_t.read &= ~0x08; // 0x1f & 0xf7 =  0xF7
//	 }
    else if(MODE_KEY_VALUE() ==KEY_DOWN )
	{
		key_t.read &= ~0x02; // 0xFf & 0xfd =  0xFD
	}
    
   

    switch(key_t.state )
	{
		case start:
		{
			if(key_t.read != _KEY_ALL_OFF)
			{
				key_t.buffer   = key_t.read; //??:key.buffer = 0xFE  POWER KEY 
				key_t.state    = first;
				key_t.on_time  = 0;
				key_t.off_time = 0;
                
			}
			break;
		}
		case first:
		{
			if(key_t.read == key_t.buffer) // adjust key be down ->continunce be pressed key
			{

			 if(++key_t.on_time>59 ){

					key_t.value = key_t.buffer^_KEY_ALL_OFF; // key.value = 0xFE ^ 0xFF = 0x01
					key_t.on_time = 0;                        //key .value = 0xEF ^ 0XFF = 0X10
                   
					key_t.state   = second;


				 }
			}
			else
			{
				key_t.on_time = 0; 
				key_t.state   = start;
			}
			break;
		}
		case second:
		{
			if(key_t.read == key_t.buffer) //again adjust key if be pressed down 
			{
				if(++key_t.on_time> 1000 && power_on_state() ==power_on)// 500 long key be down
				{
					
					key_t.value = key_t.value|0x80; //key.value(power_on) = 0x01 | 0x80  =0x81  
					key_t.on_time = 0;               //key.vaule(model_key) = 0x01 | 0x80 = 0x82
					key_t.state   = finish; 
                   
				}
			}
			else if(key_t.read == _KEY_ALL_OFF)  // loose hand 
				{
					if(++key_t.off_time>1) //8 //30 don't holding key dithering
					{
						key_t.value = key_t.buffer^_KEY_ALL_OFF; // key.value = 0x1E ^ 0x1f = 0x01
						
						key_t.state   = finish; // loose hand
					}
					
				}
		   
			break;
		}
		case finish:
		{
			
			reval = key_t.value; // is short time  TIMER_KEY = 0x01  2. long times TIMER_KEY = 0X81
			key_t.state   = end;
         
			break;
		}
		case end:
		{
			if(key_t.read == _KEY_ALL_OFF)
			{
				if(++key_t.off_time>0)//50 //100
				{
					key_t.state   = start;
                   
                  
				}
				
			}
			break;
		}
		default:
		{
			key_t.state   = start;
            
			break;
		}
	}
	return  reval;


}


#endif 
/********************************************************************
*
*Function Name:
*Function: interrupt of GPIO of call back function
*
*
*
*********************************************************************/
#if INTERRUPT_KEY
void HAL_GPIO_EXTI_Falling_Callback(uint16_t GPIO_Pin)
{
 
    
   if(!gpro_t.gTimer_pro_detect_key_ms) return ;

	gpro_t.gTimer_pro_detect_key_ms=0;
	switch(GPIO_Pin){

	case KEY_POWER_Pin:
         if(POWER_KEY_VALUE()==KEY_DOWN ){
		     gpro_t.key_power_be_pressed_flag =1;
			

         }
	

    break;

	case KEY_MODE_Pin:

      if(gpro_t.gPower_On == power_on && ptc_error_state()==0 && fan_error_state()==0){   

  	  gpro_t.mode_key_pressed_flag =1;
    

	//counter starts after 4 seconds ,cancel this function
     }

	
	break;

	case KEY_DEC_Pin:
	if(gpro_t.gPower_On == power_on && ptc_error_state()==0 && fan_error_state()==0 && DEC_KEY_VALUE()==KEY_DOWN){  
	
     gpro_t.gKey_value = dec_key_id;
	//DEC_Key_Fun();
	}

	break;


	case KEY_ADD_Pin:
	if(gpro_t.gPower_On == power_on && ptc_error_state()==0 && fan_error_state()==0 && ADD_KEY_VALUE()==KEY_DOWN){  
	
      gpro_t.gKey_value = add_key_id;
	//ADD_Key_Fun();
	 }


	break;
	
}

}
#endif 

/***********************************************************
 *  *
    *Function Name: uint8_t ReadKey(void)
    *Function: 
    *Input Ref: NO
    *Return Ref: 
    * 
***********************************************************/
#if NORMAL_KEY 
uint8_t ReadKey(void)
{

    uint8_t value0;
	if(!gpro_t.gTimer_pro_detect_key_ms) return value0;

	 gpro_t.gTimer_pro_detect_key_ms=0;


	if(HAL_GPIO_ReadPin(KEY_POWER_GPIO_Port,KEY_POWER_Pin) == KEY_DOWN && gpro_t.long_key_flag ==0){ //KEY1 =POWER_KEY ,KEY2 = MODES
			cnt = 0;
			gpro_t.long_key_flag =0;
			K1++;
			if(K1 > 1990000 && gpro_t.gPower_On ==power_on){
	               K1= 0;
			
				  gpro_t.long_key_flag =1;
				 // gctl_t.gKey_value = wifi_fun_on;
	              return   KEY_POWER_LONG_DOWN;
	              
			}
	}
	else if(HAL_GPIO_ReadPin(KEY_MODE_GPIO_Port,KEY_MODE_Pin)==KEY_DOWN  && gpro_t.long_key_flag ==0 && power_on_state() == power_on){
	  		cnt = 0;
			K2++;   //Confirm_key press
		
			gpro_t.long_key_flag =0;
			if(K2 > 1990000 && power_on_state() ==power_on){
	              K2=0;
				  cnt = 0;
				 
				  gpro_t.long_key_flag =1;
				  
				  return KEY_MODE_LONG_DOWN;
	            }


	 }
	 else if(HAL_GPIO_ReadPin(KEY_DEC_GPIO_Port,KEY_DEC_Pin)== KEY_DOWN && power_on_state() == power_on){
	       cnt =0;
		   K3++;
	       
	}
	else if(HAL_GPIO_ReadPin(KEY_ADD_GPIO_Port,KEY_ADD_Pin) == KEY_DOWN && power_on_state() == power_on){
		cnt =0;
		K4++;
	}
	//be detected of key release .
    else if(POWER_KEY_VALUE()==KEY_UP && MODE_KEY_VALUE()==KEY_UP \

      && DEC_KEY_VALUE()==KEY_UP && ADD_KEY_VALUE()==KEY_UP && gpro_t.long_key_flag ==0){ //oneself key 
		cnt++;
		if(cnt<50){ //按键松开消抖,一定要大于短按键次数 > 20
		    return 0; 

		}
//		
		cnt = 0;//

       if(K1>600){


          value1 = power_key_id;
	   }
	   else{

          value1 =0;
	   }
	
		if(K2>600 ){//short time modes press 
            value2 = mode_key_id;
			

		}
		else{ 
			value2 = 0;
		}

		//DEC_CONFIRM 
		if(K3>600 ){//short time modes press 
            value3 = dec_key_id;
			

		}
		else{ 
			value3 = 0;
		}

		
		//ADD_KEY
		if(K4>600){//short time modes press 
			value4 = add_key_id;
			
		
		}
		else{ 
		  value4 = 0;
		}

		
		
		 	
		
		K1 = 0;
		K2 = 0;
		K3 = 0 ;
		K4 = 0;

		return (value1+value2+value3+value4);
	}

   
   //judge key combination
    #if 0
	if((K1==500) && (K2<50)) //combination key 
		value1 = 0x03;
	else if(K2==500 && K1<50)
		value2 = 0x30;
	else if(K1==100 && K2>100)
		value1 = 0x44;
	else if(K1>100 && K2==100)
		value1 = 0x44;
	

  return (value1+value2);
  #endif 
	
 //  return 0;
}
#endif 

#if NORMAL_KEY_2
/**
  * 函数功能: 读取按键KEY1的状态
  * 输入参数：无
  * 返 回 值: KEY_DOWN：按键被按下；
  *           KEY_UP  ：按键没被按下
  * 说    明：无。
  */
uint8_t  POWER_KEY_StateRead(void)
{
  /* 读取此时按键值并判断是否是被按下状态，如果是被按下状态进入函数内 */
  if(HAL_GPIO_ReadPin(KEY_POWER_GPIO_Port,KEY_POWER_Pin)==KEY_DOWN_LEVEL)
  {
    /* 延时一小段时间，消除抖动 */
    HAL_Delay(20);
    /* 延时时间后再来判断按键状态，如果还是按下状态说明按键确实被按下 */
    if(HAL_GPIO_ReadPin(KEY_POWER_GPIO_Port,KEY_POWER_Pin)==KEY_DOWN_LEVEL)
    {
      /* 等待按键弹开才退出按键扫描函数 */
      while(HAL_GPIO_ReadPin(KEY_POWER_GPIO_Port,KEY_POWER_Pin)==KEY_DOWN_LEVEL){

         K1++;
		 iwdg_feed();
		 if(power_on_state() == power_on){

		    if(K1 > 900000){
                K1=0;
				
				
	
				gpro_t.wifi_led_fast_blink_flag=0;
				 gpro_t.gTimer_pro_wifi_led =0;
			
				return KEY_POWER_LONG_DOWN;
			}
		}


	  };      
       /* 按键扫描完毕，确定按键被按下，返回按键被按下状态 */
	  K1=0;
      return 1;

	  }
    }
  
  /* 按键没被按下，返回没被按下状态 */
   return 0;
}

/**
  * 函数功能: 读取按键KEY1的状态
  * 输入参数：无
  * 返 回 值: KEY_DOWN：按键被按下；
  *           KEY_UP  ：按键没被按下
  * 说    明：无。
  */

uint8_t  MODE_KEY_StateRead(void)
{
  /* 读取此时按键值并判断是否是被按下状态，如果是被按下状态进入函数内 */
  if(HAL_GPIO_ReadPin(KEY_MODE_GPIO_Port,KEY_MODE_Pin)==KEY_DOWN_LEVEL)
  {
    /* 延时一小段时间，消除抖动 */
    HAL_Delay(20);
    /* 延时时间后再来判断按键状态，如果还是按下状态说明按键确实被按下 */
    if(HAL_GPIO_ReadPin(KEY_MODE_GPIO_Port,KEY_MODE_Pin)==KEY_DOWN_LEVEL)
    {
      /* 等待按键弹开才退出按键扫描函数 */
      while(HAL_GPIO_ReadPin(KEY_MODE_GPIO_Port,KEY_MODE_Pin)==KEY_DOWN_LEVEL){
		 K2++;
		 if(power_on_state() == power_on){

		    if(K2 > 900000){
                K2=0;
				
			    
				return KEY_MODE_LONG_DOWN;

			}

		}

		};      
       /* 按键扫描完毕，确定按键被按下，返回按键被按下状态 */
	  K2=0;
      return KEY_DOWN;
    }
  }
  /* 按键没被按下，返回没被按下状态 */
  return KEY_UP;
}

/**
  * 函数功能: 读取按键KEY1的状态
  * 输入参数：无
  * 返 回 值: KEY_DOWN：按键被按下；
  *           KEY_UP  ：按键没被按下
  * 说    明：无。
  */

KEYState_TypeDef ADD_KEY_StateRead(void)
{
  /* 读取此时按键值并判断是否是被按下状态，如果是被按下状态进入函数内 */
  if(HAL_GPIO_ReadPin(KEY_ADD_GPIO_Port,KEY_ADD_Pin)==KEY_DOWN_LEVEL)
  {
    /* 延时一小段时间，消除抖动 */
    HAL_Delay(20);
    /* 延时时间后再来判断按键状态，如果还是按下状态说明按键确实被按下 */
    if(HAL_GPIO_ReadPin(KEY_ADD_GPIO_Port,KEY_ADD_Pin)==KEY_DOWN_LEVEL)
    {
      /* 等待按键弹开才退出按键扫描函数 */
      while(HAL_GPIO_ReadPin(KEY_ADD_GPIO_Port,KEY_ADD_Pin)==KEY_DOWN_LEVEL);      
       /* 按键扫描完毕，确定按键被按下，返回按键被按下状态 */
      return KEY_DOWN;
    }
  }
  /* 按键没被按下，返回没被按下状态 */
  return KEY_UP;
}

/**
  * 函数功能: 读取按键KEY1的状态
  * 输入参数：无
  * 返 回 值: KEY_DOWN：按键被按下；
  *           KEY_UP  ：按键没被按下
  * 说    明：无。
  */

KEYState_TypeDef DEC_KEY_StateRead(void)
{
  /* 读取此时按键值并判断是否是被按下状态，如果是被按下状态进入函数内 */
  if(HAL_GPIO_ReadPin(KEY_DEC_GPIO_Port,KEY_DEC_Pin)==KEY_DOWN_LEVEL)
  {
    /* 延时一小段时间，消除抖动 */
    HAL_Delay(20);
    /* 延时时间后再来判断按键状态，如果还是按下状态说明按键确实被按下 */
    if(HAL_GPIO_ReadPin(KEY_DEC_GPIO_Port,KEY_DEC_Pin)==KEY_DOWN_LEVEL)
    {
      /* 等待按键弹开才退出按键扫描函数 */
      while(HAL_GPIO_ReadPin(KEY_DEC_GPIO_Port,KEY_DEC_Pin)==KEY_DOWN_LEVEL);      
       /* 按键扫描完毕，确定按键被按下，返回按键被按下状态 */
      return KEY_DOWN;
    }
  }
  /* 按键没被按下，返回没被按下状态 */
  return KEY_UP;
}



#endif 


