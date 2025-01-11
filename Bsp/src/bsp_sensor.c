#include "bsp_sensor.h"
#include "bsp.h"




//static void gxhtc3_write_one_byte(uint8_t addr, uint8_t data);
//static void gxhtc3_write_data(uint8_t addr, uint8_t *pbuf, uint8_t datalen);
//static void gxhtc3_read(uint8_t addr, uint8_t *pbuf, uint8_t datalen);

//static uint8_t gxhtc3_read_one_byte(uint8_t addr);

//static uint8_t gxhtc3_read_continuous_byte(uint8_t addr,uint8_t *pbuf,uint8_t n);

static void  aht30_read_sensor_temperature_value(void);

static void  aht30_read_sensor_humidity_value(void);


//static unsigned int hexToDecimal(uint32_t *hexStr);



uint8_t i2c_read_data[6]={0,0,0,0,0,0};


uint8_t i2c_flag_1;

uint32_t hum_value;
uint32_t temp_value;



static void  aht30_read_sensor_humidity_value(void)
{
    
  //  uint32_t hum_value;

  //  uint32_t hum_value_temp;

    double RH;

  //  uint8_t value_rh;

    hum_value = (i2c_read_data[0] <<   12 ) | (i2c_read_data[1] <<4) | (i2c_read_data[2] >> 4);

    //hum_value  =  hexToDecimal(&hum_value_temp) ;

     RH = (double)(hum_value) / (1<<20);
    
     osDelay(100);

    // value_rh = (uint8_t )(RH *100);

    gctl_t.dht11_hum_value = (uint8_t )(RH *100);

    

}


static void  aht30_read_sensor_temperature_value(void)
{
    
  //  uint32_t temp_value;

  //  uint32_t SR;

    double SR;
        
  //  int8_t SR_TEMP;

    temp_value = ((i2c_read_data[3] <<8 ) | i2c_read_data[4] | ((i2c_read_data[2] & 0x00FF) << 16));

     temp_value =  temp_value & 0xFFFFF;

     SR =(double)(temp_value ) /(1<<20);
     osDelay(100);

    
     gctl_t.dht11_temp_value = (uint8_t)(200* SR-50);

    

}


void get_aht30_sensor_humidity_temperature_value(uint8_t *buf)
{
    aht30_read_ee_i2c_data(buf);
    aht30_read_sensor_humidity_value();
    aht30_read_sensor_temperature_value();

}



void Update_DHT11_Value(void)
{

     // dht11_read_data(&gctl_t.dht11_temp_value, &gctl_t.dht11_hum_value);
     //aht3_read_sensor_humidity_value();
     //aht3_read_sensor_temperature_value();
     get_aht30_sensor_humidity_temperature_value(i2c_read_data);
       

}



void Update_Dht11_Totencent_Value(void)
{

  //  dht11_read_data(&gctl_t.dht11_temp_value, &gctl_t.dht11_hum_value);

    get_aht30_sensor_humidity_temperature_value(i2c_read_data);


	MqttData_Publis_ReadTempHum(gctl_t.dht11_temp_value,gctl_t.dht11_hum_value);
    osDelay(100);//HAL_Delay(100);

}


//static unsigned int hexToDecimal(uint32_t *hexStr) 
//{ 
//    unsigned int decimalValue = 0; 
//    while (*hexStr) 
//    { 
//        char hexChar = *hexStr++; 
//        if (hexChar >= '0' && hexChar <= '9') 
//        { 
//           decimalValue = (decimalValue << 4) | (hexChar - '0'); 
//        } 
//        else if (hexChar >= 'A' && hexChar <= 'F') 
//        { 
//             decimalValue = (decimalValue << 4) | (hexChar - 'A' + 10); 
//        } 
//        else if (hexChar >= 'a' && hexChar <= 'f') 
//        { 
//           decimalValue = (decimalValue << 4) | (hexChar - 'a' + 10); 
//        } 
//    } 
//    return decimalValue; 

//}


