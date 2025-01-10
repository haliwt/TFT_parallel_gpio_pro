#include "bsp_sensor.h"
#include "bsp.h"




static void gxhtc3_write_one_byte(uint8_t addr, uint8_t data);
static void gxhtc3_write_data(uint8_t addr, uint8_t *pbuf, uint8_t datalen);
static void gxhtc3_read(uint8_t addr, uint8_t *pbuf, uint8_t datalen);

static uint8_t gxhtc3_read_one_byte(uint8_t addr);

static uint8_t gxhtc3_read_continuous_byte(uint8_t addr,uint8_t *pbuf,uint8_t n);

static int8_t aht30_read_sensor_temperature_value(void);





uint8_t i2c_read_data[6];



uint8_t  aht30_read_sensor_humidity_value(void)
{
    
    uint32_t hum_value;

    uint8_t RH;

    hum_value = (i2c_read_data[0] <<16 ) | (i2c_read_data[1] <<8) | (i2c_read_data[2] & 0xFF00);

     RH = hum_value >> 20;

     return RH;

}


static int8_t  aht30_read_sensor_temperature_value(void)
{
    
    uint16_t temp_value;

    uint8_t SR;
        
    int8_t SR_TEMP;

    temp_value = (i2c_read_data[3] <<16 ) | (i2c_read_data[4] << 8)|((i2c_read_data[2] & 0x00FF));

     SR = (200 * temp_value ) >> 16;

     SR_TEMP =  SR - 50;

     return SR_TEMP;

}


void get_aht30_sensor_humidity_temperature_value(uint8_t *buf)
{

    aht30_read_ee_i2c_data(buf);
    gctl_t.dht11_hum_value = aht30_read_sensor_humidity_value();
    gctl_t.dht11_temp_value = aht30_read_sensor_temperature_value();



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

