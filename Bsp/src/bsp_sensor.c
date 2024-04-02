#include "bsp_sensor.h"
#include "bsp.h"


#define Bit_RESET 0
#define Bit_SET   1


static void DHT11_Mode_IPU(void);
static void DHT11_Mode_Out_PP(void);
static uint8_t DHT11_ReadByte(void);
static void  Dht11_Read_TempHumidity_Handler(DHT11_Data_TypeDef * pdth11);

DHT11_Data_TypeDef DHT11;




/**
  * 锟斤拷锟斤拷锟斤拷锟斤拷: 使DHT11-DATA锟斤拷锟脚憋拷为锟斤拷锟斤拷锟斤拷锟斤拷模式
  * 锟斤拷锟斤拷锟斤拷锟ￄ1�7: 锟斤拄1�7
  * 锟斤拄1�7 锟斤拄1�7 倄1�7: 锟斤拄1�7
  * 评1�7    锟斤拷锟斤拷锟斤拄1�7
  */
static void DHT11_Mode_IPU(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DHT11_Dout_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT11_Dout_PORT, &GPIO_InitStruct);
}

/**
  * 锟斤拷锟斤拷锟斤拷锟斤拷: 使DHT11-DATA锟斤拷锟脚憋拷为锟斤拷锟斤拷锟斤拷锟侥Ｊￄ1�7
  * 锟斤拷锟斤拷锟斤拷锟ￄ1�7: 锟斤拄1�7
  * 锟斤拄1�7 锟斤拄1�7 倄1�7: 锟斤拄1�7
  * 评1�7    锟斤拷锟斤拷锟斤拄1�7
  */
static void DHT11_Mode_Out_PP(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DHT11_Dout_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(DHT11_Dout_PORT, &GPIO_InitStruct);
}

/**
  * 锟斤拷锟斤拷锟斤拷锟斤拷: 锟斤拷DHT11锟斤拷取丢�锟斤拷锟街节ｏ拷MSB锟斤拷锟斤拷
  * 锟斤拷锟斤拷锟斤拷锟ￄ1�7: 锟斤拄1�7
  * 锟斤拄1�7 锟斤拄1�7 倄1�7: 锟斤拄1�7
  * 评1�7    锟斤拷锟斤拷锟斤拄1�7
  */
static uint8_t DHT11_ReadByte ( void )
{
	uint8_t i, temp=0;
	
	for(i=0;i<8;i++)    
	{	 
		/*每bit锟斤拄1�750us锟酵碉拷平锟斤拷锟矫匡拷始锟斤拷锟斤拷询直锟斤拷锟接伙拷锟斤拷锟斤拷 锟斤拄1�750us 锟酵碉拷幄1�7 锟斤拷锟斤拷*/  
		while(DHT11_Data_IN()==Bit_RESET);

		/*DHT11 锟斤拄1�726~28us锟侥高碉拷平锟斤拷示锟斤拄1�70锟斤拷锟斤拷锟斤拄1�770us锟竭碉拷平锟斤拷示锟斤拷1锟斤拷锟斤拷
		 *通锟斤拷锟斤拷锟ￄ1�7 x us锟斤拷牡锟狡斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟阶ￄ1�7 锟斤拷x 锟斤拷锟斤拷锟斤拷锟斤拷锟绞ￄ1�7 
		 */
		delay_us(40); //锟斤拷时x us 锟斤拷锟斤拷锟绞憋拷锟揭拷锟斤拷锟斤拷锟斤拷锟ￄ1�70锟斤拷锟斤拷锟斤拷时锟戒即锟斤拷	   	  

		if(DHT11_Data_IN()==Bit_SET)/* x us锟斤拷锟斤拷为锟竭碉拷平锟斤拷示锟斤拷锟捷★拄1�71锟斤拄1�7 */
		{
			/* 锟饺达拷锟斤拷锟斤拷1锟侥高碉拷平锟斤拷锟斤拷 */
			while(DHT11_Data_IN()==Bit_SET);

			temp|=(uint8_t)(0x01<<(7-i));  //锟窖碉拷7-i位锟斤拷1锟斤拷MSB锟斤拷锟斤拷 
		}
		else	 // x us锟斤拷为锟酵碉拷平锟斤拷示锟斤拷锟捷★拷0锟斤拄1�7
		{			   
			temp&=(uint8_t)~(0x01<<(7-i)); //锟窖碉拷7-i位锟斤拷0锟斤拷MSB锟斤拷锟斤拷
		}
	}
	return temp;
}

/**
  * 锟斤拷锟斤拷锟斤拷锟斤拷: 丢�锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷锟捷达拷锟斤拷丄1�740bit锟斤拷锟斤拷位锟饺筹拄1�7
  * 锟斤拷锟斤拷锟斤拷锟ￄ1�7: DHT11_Data:DHT11锟斤拷锟斤拷锟斤拷锟斤拷
  * 锟斤拄1�7 锟斤拄1�7 倄1�7: ERROR锟斤拄1�7  锟斤拷取锟斤拷锟斤拷
  *           SUCCESS锟斤拷锟斤拷取锟缴癸拄1�7
  * 评1�7    锟斤拷锟斤拷8bit 湿锟斤拷锟斤拷锟斤拷 + 8bit 湿锟斤拷小锟斤拷 + 8bit 锟铰讹拷锟斤拷锟斤拷 + 8bit 锟铰讹拷小锟斤拷 + 8bit 校锟斤拷锟� 
  */
uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef *DHT11_Data)
{  
  uint8_t temp;
  uint16_t humi_temp;
  
	/*锟斤拷锟侥Ｊￄ1�7*/
	DHT11_Mode_Out_PP();
	/*锟斤拷锟斤拷锟斤拷锟斤拷*/
	DHT11_Dout_LOW();
	/*锟斤拷时18ms*/
	HAL_Delay(20);

	/*锟斤拷锟斤拷锟斤拷锟斤拷 锟斤拷锟斤拷锟斤拷时30us*/
	DHT11_Dout_HIGH(); 

	delay_us(30);   //锟斤拷时30us

	/*锟斤拷锟斤拷锟斤拷为锟斤拷锟斤拷 锟叫断从伙拷锟斤拷应锟脚猴拷*/ 
	DHT11_Mode_IPU();
  delay_us(40);   //锟斤拷时30us
	/*锟叫断从伙拷锟角凤拷锟叫低碉拷平锟斤拷应锟脚猴拷 锟界不锟斤拷应锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷应锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷*/   
	if(DHT11_Data_IN()==Bit_RESET)     
	{
    /*锟斤拷询直锟斤拷锟接伙拷锟斤拷锟斤拷 锟斤拄1�780us 锟酵碉拷幄1�7 锟斤拷应锟脚号斤拷锟斤拷*/  
    while(DHT11_Data_IN()==Bit_RESET);

    /*锟斤拷询直锟斤拷锟接伙拷锟斤拷锟斤拷锟斤拄1�7 80us 锟竭碉拷幄1�7 锟斤拷锟斤拷锟脚号斤拷锟斤拷*/
    while(DHT11_Data_IN()==Bit_SET);

    /*锟斤拷始锟斤拷锟斤拷锟斤拷锟斤拷*/   
    DHT11_Data->humi_high8bit= DHT11_ReadByte();
    DHT11_Data->humi_low8bit = DHT11_ReadByte();
    DHT11_Data->temp_high8bit= DHT11_ReadByte();
    DHT11_Data->temp_low8bit = DHT11_ReadByte();
    DHT11_Data->check_sum    = DHT11_ReadByte();

    /*锟斤拷取锟斤拷锟斤拷锟斤拷锟斤拷锟脚革拷为锟斤拷锟侥Ｊ�*/
    DHT11_Mode_Out_PP();
    /*锟斤拷锟斤拷锟斤拷锟斤拷*/
    DHT11_Dout_HIGH();
    
    /* 锟斤拷锟斤拷锟捷斤拷锟叫达拷锟斤拄1�7 */
    humi_temp=DHT11_Data->humi_high8bit*100+DHT11_Data->humi_low8bit;
    DHT11_Data->humidity =(float)humi_temp/100;
    
    humi_temp=DHT11_Data->temp_high8bit*100+DHT11_Data->temp_low8bit;
    DHT11_Data->temperature=(float)humi_temp/100;    
    
    /*锟斤拷锟斤拷取锟斤拷锟斤拷锟斤拷锟角凤拷锟斤拷确*/
    temp = DHT11_Data->humi_high8bit + DHT11_Data->humi_low8bit +
           DHT11_Data->temp_high8bit+ DHT11_Data->temp_low8bit;
    if(DHT11_Data->check_sum==temp)
    { 
      return SUCCESS;
    }
    else 
      return ERROR;
	}	
	else
		return ERROR;
}


static void  Dht11_Read_TempHumidity_Handler(DHT11_Data_TypeDef * pdth11)
{
	if(DHT11_Read_TempAndHumidity(pdth11) == 0){
		   
	//gProcess_t.gdht11_temperature = pdth11->temp_high8bit;

      //humidity data
		  gctl_t.dht11_hum_value = pdth11->humi_high8bit;

      //temperature data
     
      gctl_t.dht11_temp_value = pdth11->temp_high8bit  ;
		   
	   
	 }

}

void Update_DHT11_Value(void)
{
    Dht11_Read_TempHumidity_Handler(&DHT11);

}




void Update_Dht11_Totencent_Value(void)
{


	Dht11_Read_TempHumidity_Handler(&DHT11);

	MqttData_Publis_ReadTempHum(gctl_t.dht11_temp_value,gctl_t.dht11_hum_value);

}









