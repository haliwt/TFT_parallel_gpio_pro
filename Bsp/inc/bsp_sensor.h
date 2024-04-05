#ifndef  __BSP_SENSOR_H_
#define __BSP_SENSOR_H_
#include "main.h"

//IO��������
#define DHT11_DATA_IO_IN()      {GPIOA->MODER&=0XFFFFFFFC;GPIOD->MODER|=0<<0;}  //0x00 input mode
#define DHT11_DATA_IO_OUT()     {GPIOD->MODER&=0XFFFFFFFC;GPIOD->MODER|=1<<0;}   //0x01 output  mode 

//#define DHT11_DATA      GPIO_PIN_2
//#define DHT11_GPIO      GPIOD

#define DHT11_DATA_SetHigh()            do{TEMP_SENSOR_GPIO_Port->BSRR !=TEMP_SENSOR_Pin;}while(0)//HAL_GPIO_WritePin(TEMP_SENSOR_GPIO_Port,TEMP_SENSOR_Pin,GPIO_PIN_SET)    // output high level
#define DHT11_DATA_SetLow()             do{TEMP_SENSOR_GPIO_Port->BSRR !=(uint8_t)TEMP_SENSOR_Pin<<16;}while(0)//HAL_GPIO_WritePin(TEMP_SENSOR_GPIO_Port,TEMP_SENSOR_Pin,GPIO_PIN_RESET)    // output low level

#define DHT11_ReadData()	            HAL_GPIO_ReadPin(TEMP_SENSOR_GPIO_Port,TEMP_SENSOR_Pin)

/* �궨�� -------------------------------------------------------------------*/
/***********************   DHT11 �������Ŷ���  **************************/
#define DHT11_Dout_GPIO_CLK_ENABLE()              __HAL_RCC_GPIOD_CLK_ENABLE()//__HAL_RCC_GPIOA_CLK_ENABLE()
#define DHT11_Dout_PORT                           GPIOA
#define DHT11_Dout_PIN                            GPIO_PIN_15

/***********************   DHT11 �����궨��  ****************************/
#define DHT11_Dout_LOW()                          HAL_GPIO_WritePin(TEMP_SENSOR_GPIO_Port, TEMP_SENSOR_Pin, GPIO_PIN_RESET)
#define DHT11_Dout_HIGH()                         HAL_GPIO_WritePin(TEMP_SENSOR_GPIO_Port, TEMP_SENSOR_Pin, GPIO_PIN_SET)
#define DHT11_Data_IN()	                          HAL_GPIO_ReadPin(TEMP_SENSOR_GPIO_Port,TEMP_SENSOR_Pin)



/* ���Ͷ��� ------------------------------------------------------------------*/
/************************ DHT11 �������Ͷ���******************************/
typedef struct
{
	uint8_t  humi_high8bit;		//ԭʼ���ݣ�ʪ�ȸ�8λ
	uint8_t  humi_low8bit;	 	//ԭʼ���ݣ�ʪ�ȵ�8λ
	uint8_t  temp_high8bit;	 	//ԭʼ���ݣ��¶ȸ�8λ
	uint8_t  temp_low8bit;	 	//ԭʼ���ݣ��¶ȸ�8λ
	uint8_t  check_sum;	 	    //У���
  float    humidity;            //ʵ��ʪ��
  float    temperature;        //ʵ���¶�  
} DHT11_Data_TypeDef;
//extern DHT11_Data_TypeDef DHT11;


/* ��չ���� ------------------------------------------------------------------*/
/* �������� ------------------------------------------------------------------*/
//void DHT11_Init( void );
//uint8_t DHT11_Read_TempAndHumidity(DHT11_Data_TypeDef * DHT11_Data);
void Update_DHT11_Value(void);

void Update_Dht11_Totencent_Value(void);




#endif 

















