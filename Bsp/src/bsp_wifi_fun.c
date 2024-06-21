#include "bsp_wifi_fun.h"
#include "bsp.h"



WIFI_FUN   wifi_t;


uint8_t (*wifi_link_net_state)(void); //函数指针


static uint8_t wifi_link_default_fun(void);



/****************************************************************
     * 
     * Function Name:void Wifi_Init(void)
     * Function: 
     * Input Ref: NO
     * Return Ref:NO
     * 
****************************************************************/
void Wifi_Init(void)
{
  Wifi_Link_Net_Handler(wifi_link_default_fun);


}

/***************************************************************************************
     * 
     * Function Name:void Wifi_Link_Net_Handler(uint8_t (*wifi_link_handler)(void))
     * Function: 
     * Input Ref: NO
     * Return Ref:NO
     * 
***************************************************************************************/
void Wifi_Link_Net_Handler(uint8_t (*wifi_link_handler)(void))
{
    wifi_link_net_state = wifi_link_handler;
}


static uint8_t wifi_link_default_fun(void)
{

    if(wifi_t.esp8266_login_cloud_success ==1)return 1;
	else return 0;


}




