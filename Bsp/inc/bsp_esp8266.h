#ifndef __BSP_ESP8266_H_
#define __BSP_ESP8266_H_
#include "main.h"


#define SSID                    "UYIKIA"
#define PASSWD                  "20329263"


#define TOPIC                  "open"

#define TOPIC_VALUE            1
//#define SMARTCONFIG      			0


#define PRODUCT_ID              "EHQB1P53IH" //production ID 
//#define DEVUICE_NAME            "UYIJIA01-afcc8"      //device Name:
#define DEVICE_SECRET           "5xdQMgx7ZHznYyQK6wvdmolc"//"5xdQMgx7ZHznYyQK6wvdmolc"  //Secret key 
#define PRODUCT_REGION          "ap-guangzhou" //filed name 

#define WIFI_IC_ENABLE()         HAL_GPIO_WritePin(WIFI_EN_GPIO_Port,WIFI_EN_Pin,GPIO_PIN_SET)
#define WIFI_IC_DISABLE()        HAL_GPIO_WritePin(WIFI_EN_GPIO_Port,WIFI_EN_Pin,GPIO_PIN_RESET)


typedef enum _esp8266_para{

	wifi_set_restor=0x01,
	wifi_set_cwmode,
	wifi_set_softap,
	wifi_set_tcdevreg,
	wifi_set_tcsap,
	wifi_login_tencent,
	wifi_link_smartphone


}esp8266_para;


typedef enum{

  driver_esp8266_rest=0x01,
  driver_esp8266_step_2,
  driver_esp8266_step_3,
  driver_esp8266_step_4,
  driver_esp8266_step_5,
  driver_esp8266_step_6,
  driver_esp8266_step_7,
  driver_esp8266_step_8,
  driver_esp8266_step_9


}wifi_link_step;


void Auto_InitWifiModule_Hardware(void);
void Auto_SmartPhone_TryToLink_TencentCloud(void);



void InitWifiModule_Hardware(void);
void SmartPhone_LinkTencent_Cloud(void);

uint8_t at_send_data(uint8_t *pdata, uint16_t len);
void Wifi_Rx_InputInfo_Handler(void);

void Wifi_SoftAP_Config_Handler(void);
void SmartPhone_TryToLink_TencentCloud(void);
void wifi_Disconnect_Fun(void);
void Get_BeiJing_Time_Cmd(void);
void Get_Beijing_Time(void);
void PowerOn_Self_Auto_Link_Tencent_Cloud(void);

void Wifi_Link_Tencent_Net_State(void);

void Wifi_Reconnect_Tencent_Net(void);

void AutoConnect_SmartPhone_TencentCloud(void);
void ReConnect_Wifi_Net_ATReset_Hardware(void);


#endif 
