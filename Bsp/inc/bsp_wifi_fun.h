#ifndef __BSP_WIFI_FUN_
#define __BSP_WIFI_FUN_
#include "main.h"


typedef enum{
  
   wifi_AI=0x08 ,wifi_notAI=0x18,wifi_kill=0x04,wifi_notkill=0x14,
   wifi_heat= 0x02,wifi_notheat=0x12


}wifi_mode_t;

typedef enum _wifi_state_t{  
	
    
	wifi_link_tencent_cloud=0x01, //1
	wifi_tencent_publish_init_data, //2
	wifi_subscriber_form_tencent_data,//3
	wifi_publish_dht11_delay,//4
	wifi_subscriber_form_tencent_cloud_cmd,//5
	wifi_publish_update_tencent_cloud_data,//6
	wifi_auto_to_link_cloud,//7
	wifi_atuo_link_cloud_ref,//8
	wifi_auto_repeat_link_cloud,//9
	wifi_tencent_publish_dht11_data,//10
	wifi_get_beijing_time,//11
	wifi_disconnect,//12
	wifi_null//13


}wifi_state_t;



/* USER CODE BEGIN Prototypes */
typedef struct usart
{
    unsigned char UART_Data[150]; 
    unsigned char UART_Flag;
    unsigned int wifi_uart_counter;
    unsigned char wifi_inputBuf[1];
}USART_DataBuf;






typedef struct _WIFI_FUN{
	

    uint8_t data[512];
	uint8_t auto_det_data[150];
    uint8_t  data_size;
    uint8_t flag;
	uint8_t wifi_RunState;
	uint8_t esp8266_smartphone_flag;
	uint8_t esp8266_link_cloud_flag;
	uint8_t soft_ap_config_success;
	uint8_t esp8266_login_cloud_success;

	uint8_t subscribe_cloud_success;
	uint8_t power_off_step;
    uint8_t usart2_dataBuf[1];

    uint8_t link_tencent_thefirst_times;
	
	
	uint8_t esp8266_dynamic_reg_flag;
    uint8_t subscribe_flag;
	uint8_t subscribe_rx_flag;
    uint8_t subscribe_rxCloud_flag;
	uint8_t esp8266_data_rx_success;
	uint8_t rx_data_success ;
	uint8_t  rx_counter ;
	uint8_t  rx_data_state;
	uint8_t getCloudValue_unit ;
	uint8_t getCloudValue_decade;
	uint8_t rx_data_len;
	uint8_t rx_data_name_len;
	uint8_t linking_tencent_cloud_doing;
    uint8_t cmp_flag;
	uint8_t send_tencent_cloud_data_lable;
    uint8_t subscrible_receive_data_label;
	uint8_t link_tencent_step_counter;
	uint8_t link_beijing_times_flag;
	uint8_t auto_linknet_flag;
	uint8_t three_times_link_beijing;
    //esp8266 up ref

	uint8_t runCommand_order_lable;
	uint8_t has_been_login_flag;
    uint8_t soft_ap_config_flag;
    uint8_t get_rx_beijing_time_enable;
	uint8_t auto_link_login_tencent_cloud_flag;

	
	//wifi
	unsigned char wifi_data[150]; 
	uint8_t  wifi_temp_data[150]; 
 
	
  
    //uint8_t usart2_rx_flag;
    uint8_t first_power_on_link_net;
	
	
	uint8_t wifi_uart_counter;
	uint8_t tencent_cloud_command_power_on;
	uint8_t get_rx_auto_repeat_net_enable;
	
	//
	uint8_t response_wifi_signal_label;
	uint8_t reconnect_tencent_cloud_flag;

	
    //smart phone 
    uint8_t smartphone_app_power_on_flag;
    uint8_t power_on_linkwifi;
	uint8_t power_on_login_tencent_cloud_flag;
	uint8_t repeat_login_tencent_cloud_init_ref;
    

	//fan
	uint8_t set_wind_speed_value;

	//config
	uint8_t wifi_config_net_lable;

    //real time 
    uint8_t real_hours;
	uint8_t real_minutes;
	uint8_t real_seconds;
	uint8_t restart_link_tencent_cloud ;
	uint8_t received_data_from_tencent_cloud ;
    
	//beijing time
	
	uint8_t get_beijing_time_success ;

    uint8_t link_net_tencent_data_flag;

    uint8_t smartphone_app_power_on_timer_flag ;
    

	//auto link net
	
	
    uint8_t rx_error_data_success;

    //wifi gtimer
	uint8_t gTimer_read_beijing_time;
	uint8_t gTimer_linking_tencent_duration;
	
    uint8_t gTimer_beijing_time;
	uint8_t gTimer_reconnect_wifi;
	uint8_t gTimer_power_off;
	uint8_t gTimer_subscriber_send ;
	uint8_t gTimer_publish_dht11;
	
	uint8_t gTimer_login_tencent_times;
	uint8_t gTimer_power_first_link_tencent ;

	uint8_t gTimer_wifi_power_on_detect ;
	uint8_t gTimer_wifi_counter_link_beijing_times ;
	uint8_t gTimer_main_pro_times;


    uint8_t gTimer_counter_repeat_link_net;
    uint8_t gTimer_auto_detected_net;
	
    uint16_t gTimer_get_beijing_time;
	uint16_t gTimer_auto_detected_net_state_times;

 
    
  
	

	uint32_t randomName[1];
	
	
	
		
	
}WIFI_FUN;

extern WIFI_FUN   wifi_t;

extern uint8_t (*wifi_link_net_state)(void); //函数指针


void Wifi_Init(void);

void PowerOn_Host(void (*poweronHandler)(void));
void PowerOff_Host(void (*poweroffHandler)(void));




void wifiDisplayTemperature_Humidity(void);

void Wifi_Link_Net_Handler(uint8_t (*wifi_link_handler)(void));

void wifiUpdate_SetTimeValue(uint8_t tv);
void wifiUpdate_SetTemperatureValue(uint8_t temp);









#endif 

