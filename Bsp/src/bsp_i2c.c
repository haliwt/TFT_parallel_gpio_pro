#include "bsp.h"


#include "main.h"

//#define	EE_ADDR 0xE0//0xa0//EEPROM地址，地址管脚全接地，为0xA0
#define EE_SCL_PIN             GPIO_PIN_6   //模拟IIC的SCL信号  1.修改引脚即可修改IIC接口
#define EE_SDA_PIN             GPIO_PIN_7   //模拟IIC的SDA信号

#define EE_I2C_GPIO_Port              GPIOC

typedef enum {
    LOW = 0,
    HIGH = 1
} EEPinState;

// 设置 SCL 引脚为高电平




#define EE_IIC_SCL(val)         HAL_GPIO_WritePin(GPIOC, EE_SCL_PIN ,val)                    //SCL 输出高或者低     2.修改引脚即可修改IIC接口        
#define EE_IIC_SDA(val)         HAL_GPIO_WritePin(GPIOC, EE_SDA_PIN,val)                    //SDA 输出高或者低 

static uint8_t  i2c_WriteOneByte(uint8_t data);

uint8_t i2c_connect_flag,ack_flag,i2c_state;


void EE_SDA_IN(void) 	//PB11配置成输入  
{  
    __HAL_RCC_GPIOC_CLK_ENABLE();//GPIO时钟使能
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = EE_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM  ;//LOW;
  HAL_GPIO_Init(EE_I2C_GPIO_Port, &GPIO_InitStruct);
} 
void EE_SDA_OUT(void)//PB11配置成开漏输出
{
	__HAL_RCC_GPIOC_CLK_ENABLE();//GPIO时钟使能
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = EE_SDA_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;//GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM  ;//LOW;
  HAL_GPIO_Init(EE_I2C_GPIO_Port, &GPIO_InitStruct);
	
}

//void EE_SCK_OUT(void) //PB10配置成开漏输出
//{
//	__HAL_RCC_GPIOC_CLK_ENABLE();//GPIO时钟使能
//	GPIO_InitTypeDef GPIO_InitStruct = {0};
//	GPIO_InitStruct.Pin = EE_SCL_PIN;
//  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
//  GPIO_InitStruct.Pull = GPIO_NOPULL;
//  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//  HAL_GPIO_Init(EE_I2C_GPIO_Port, &GPIO_InitStruct);
//}

//读DATA引脚状态
unsigned char EE_READ_SDA(void)
{
return HAL_GPIO_ReadPin(EE_I2C_GPIO_Port,EE_SDA_PIN);
}	



/******************************************************************************
*函  数：void EE_IIC_Delay(void)
*功　能：IIC延时
*参  数：无
*返回值：无
*备  注: 移植时只需要将EE_IIC_Delay()换成自己的延时即可,目前是在16M主频下运行，大约等待10us
*******************************************************************************/	
void EE_IIC_Delay(uint16_t us)
{
	uint16_t j;
	for(j=0;j<us;j++)
	{
        for(int i = 0; i < 20; i++)    //for(int i = 0; i < 20; i++)    
        {
            __asm("NOP");//等待1个指令周期，系统主频16M
        }
	}

  // delay_us(us);
    
}
/******************************************************************************
*函  数：void IIC_Init(void)
*功　能：IIC初始化
*参  数：无
*返回值：无
*备  注：无
*******************************************************************************/

void EE_IIC_Init(void)
{			
   // EE_SCK_OUT();//CLK引脚配置成输出
    EE_SDA_OUT();//DATA引脚配置成输出
    EE_IIC_SCL(GPIO_PIN_SET);//CLK引脚输出高
    EE_IIC_SDA(GPIO_PIN_SET);//DATA引脚输出高   
}
//开始	
void EE_IIC_Start(void)
{
	EE_SDA_OUT(); //DATA引脚配置成输出
	EE_IIC_SDA(GPIO_PIN_SET);//DATA引脚输出高	
	EE_IIC_SCL(GPIO_PIN_SET);//CLK引脚输出高
	EE_IIC_Delay(4);//等待大约40us
 	EE_IIC_SDA(GPIO_PIN_RESET); //DATA引脚输出低
	EE_IIC_Delay(4);//等待大约40us
	EE_IIC_SCL(GPIO_PIN_RESET); //CLK引脚输出低，钳住I2C总线，准备发送或接收数据 
}

//停止	  
void EE_IIC_Stop(void)
{
	EE_SDA_OUT(); //DATA引脚配置成输出
	EE_IIC_SCL(GPIO_PIN_RESET);//CLK引脚输出低
	EE_IIC_SDA(GPIO_PIN_RESET); //DATA引脚输出低
  EE_IIC_Delay(4);//等待大约40us
	EE_IIC_SCL(GPIO_PIN_SET); //CLK引脚输出高
	EE_IIC_SDA(GPIO_PIN_SET); //DATA引脚输出高，发送I2C总线结束信号
  EE_IIC_Delay(4);//等待大约40us							   	
}

//等待应答
uint8_t EE_IIC_WaitAck(void)
{
	uint8_t ucErrTime=0;
	EE_SDA_IN(); //DATA引脚配置成输入  （从机给一个低电平做为应答） 
	EE_IIC_SDA(GPIO_PIN_SET);
	EE_IIC_Delay(1);	   
	EE_IIC_SCL(GPIO_PIN_SET);
	EE_IIC_Delay(1);//等待约10us 
	while(EE_READ_SDA())//一直读，直到读取到低电平应答
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			EE_IIC_Stop();
			return 1;
		}
	}
	EE_IIC_SCL(GPIO_PIN_RESET); //时钟输出0 	   
	return 0;  
} 


//发送应答
void EE_IIC_Ack(void)
{
	EE_IIC_SCL(GPIO_PIN_RESET); //low level
	EE_SDA_OUT();
	EE_IIC_SDA(GPIO_PIN_RESET);//EE_IIC_SDA(0);
	EE_IIC_Delay(1); 
	EE_IIC_SCL(GPIO_PIN_SET);//EE_IIC_SCL(1);
	EE_IIC_Delay(2);
	EE_IIC_SCL(GPIO_PIN_RESET);//EE_IIC_SCL(0);
}

//发送非应答
void EE_IIC_NAck(void)
{
	EE_IIC_SCL(GPIO_PIN_RESET);  //low level
	EE_SDA_OUT();
	EE_IIC_SDA(GPIO_PIN_SET);   //high level
	EE_IIC_Delay(1);
	EE_IIC_SCL(GPIO_PIN_SET);  //high level
	EE_IIC_Delay(1);
	EE_IIC_SCL(GPIO_PIN_RESET); //low level
}					 				     

//发送一个字节	  
void EE_IIC_SendByte(uint8_t data)
{                        
    uint8_t t;   
    EE_SDA_OUT(); 	    
    EE_IIC_SCL(GPIO_PIN_RESET); //拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
	    EE_IIC_SDA((data&0x80)>>7);//发送数据
        EE_IIC_Delay(1);			
        EE_IIC_SCL(GPIO_PIN_SET);  //high level
        data<<=1;
        EE_IIC_Delay(1);
        EE_IIC_SCL(GPIO_PIN_RESET); //low level	   
    }
    EE_IIC_Delay(1);
} 	

static uint8_t i2c_WriteOneByte(uint8_t data)
{                        
    uint8_t t;   
    EE_SDA_OUT(); 	    
    EE_IIC_SCL(GPIO_PIN_RESET); //拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
	    EE_IIC_SDA((data&0x80)>>7);//发送数据
        EE_IIC_Delay(1);			
        EE_IIC_SCL(GPIO_PIN_SET);    //high level
        data<<=1;
        EE_IIC_Delay(1);
        EE_IIC_SCL(GPIO_PIN_RESET);	  //low level  
    }
    EE_IIC_Delay(1);

    if(EE_IIC_WaitAck()){

        EE_IIC_Stop();
        return 1; //从机地址写入失败


    }
    
} 


//读取1字节	
uint8_t EE_IIC_ReadByte(uint8_t ack)
{
	uint8_t i,receive=0;
	EE_SDA_IN(); //SDA设置为输入模式 等待接收从机返回数据
    for(i=0;i<8;i++ )
	{
        EE_IIC_SCL(GPIO_PIN_RESET); //low level
        EE_IIC_Delay(1);
        EE_IIC_SCL(GPIO_PIN_SET); //high level
        receive<<=1;
        if(EE_READ_SDA())receive++; //读取从机发送的电平，如果是高，就记录高
        EE_IIC_Delay(1); 
    }					 
    if(ack)
        EE_IIC_Ack(); //发送ACK 
    else
        EE_IIC_NAck(); //发送nACK  
    return receive;
}

#if 0
//从EE指定地址读取一个字节
uint8_t I2C_ReadByteFromSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t *buf)
{
	
    uint8_t buf_t;
    EE_IIC_Start();	
    #if 0
	EE_IIC_SendByte(I2C_Addr);	 //发送从机地址
	if(EE_IIC_WaitAck()) //如果从机未应答则数据发送失败
	{
		EE_IIC_Stop();
		return 1;
	}
	EE_IIC_SendByte(reg); //发送寄存器地址
	EE_IIC_WaitAck();	  
	
	EE_IIC_Start();
    #endif 
	EE_IIC_SendByte(I2C_Addr+1); //进入接收模式			   
	EE_IIC_WaitAck();
	
   
	buf[0]=EE_IIC_ReadByte(1);
    
    buf[1]=EE_IIC_ReadByte(1);
 
    buf[2]=EE_IIC_ReadByte(1);
 
    buf[3]=EE_IIC_ReadByte(1);
 
    buf[4]=EE_IIC_ReadByte(1);

    buf_t=EE_IIC_ReadByte(0);

    
    
     //EE_IIC_Stop(); //产生一个停止条件
	return 0;
}
#endif 
/*********************************************************************************
*
*Function Name:
*Function: form aht3 sensor read  humidity and temperature value 
*Input Ref: acknowlege single
*Return Ref: 
*
*********************************************************************************/
uint8_t I2C_ReadByteFromSlave(uint8_t *buf)
{
	
  
	buf[0]=EE_IIC_ReadByte(1);
    
    buf[1]=EE_IIC_ReadByte(1);
 
    buf[2]=EE_IIC_ReadByte(1);
 
    buf[3]=EE_IIC_ReadByte(1);
 
    buf[4]=EE_IIC_ReadByte(1);

    buf[5]=EE_IIC_ReadByte(0);  // CRC data 

    
    EE_IIC_Stop(); //产生一个停止条件
 }


#if 0
//发送一个字节内容到EE指定地址
uint8_t I2C_SendByteToSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t data)
{
	EE_IIC_Start();
	EE_IIC_SendByte(I2C_Addr); //发送从机地址
	if(EE_IIC_WaitAck())
	{
		EE_IIC_Stop();
		return 1; //从机地址写入失败
	}
	EE_IIC_SendByte(reg); //发送寄存器地址
        EE_IIC_WaitAck();	  
	EE_IIC_SendByte(data); 
	if(EE_IIC_WaitAck())
	{
		EE_IIC_Stop(); 
		return 1; //数据写入失败
	}
	EE_IIC_Stop(); //产生一个停止条件
    
	return 0;
}
#endif 
/*********************************************************************************
*
*Function Name:uint8_t I2C_SendByteToSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t data,uin8_t data1)
*Function: write four command to slave device
*Input Ref: command 1~4
*Return Ref: 
*
*********************************************************************************/
uint8_t I2C_SendByteToSlave(uint8_t I2C_Addr,uint8_t reg,uint8_t data,uint8_t data1)
{
	EE_IIC_Start();
	EE_IIC_SendByte(I2C_Addr); //发送从机地址
	if(EE_IIC_WaitAck())
	{
		EE_IIC_Stop();
		return 1; //从机地址写入失败
	}
    
	EE_IIC_SendByte(reg); //发送寄存器地址

    if(EE_IIC_WaitAck()){

        EE_IIC_Stop();
        return 1; //从机地址写入失败


    }
    
	EE_IIC_SendByte(data); 
    
	if(EE_IIC_WaitAck())
	{
		EE_IIC_Stop(); 
		return 1; //数据写入失败
	}

    
     EE_IIC_SendByte(data1); 
        
      if(EE_IIC_WaitAck())
       {
            EE_IIC_Stop(); 
            return 1; //数据写入失败
       }


    
	EE_IIC_Stop(); //产生一个停止条件
    
	return 0;
}




void aht30_read_ee_i2c_data(uint8_t *buf)
{
   // uint8_t read_data;

    i2c_connect_flag = I2C_SendByteToSlave(0x70,0xAC,0x33,0X00);//AHT30 sensor
	
    HAL_Delay(100);//osDelay(80);
    
    EE_IIC_Start();
    ack_flag = i2c_WriteOneByte(0x71);

    if(ack_flag ==0){

        i2c_state = EE_IIC_ReadByte(1); //state of value

    }

    if(i2c_state >> 8){


    }
    else{
       I2C_ReadByteFromSlave(buf);//从0地址读取1字节内容到EEDATA变量
    }

}

