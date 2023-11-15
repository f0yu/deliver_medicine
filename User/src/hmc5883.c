#include "hmc5883.h"
#include <math.h>
//软件iic

/**
 * @brief 一段延迟
 * @param 无
 * @return 无
 * @author HZ12138
 * @date 2022-07-27 08:53:30
 */
void I2C_Delay()
{
//	int z = 0xff;
//	while (z--)
//		;
}

/**
 * @brief 产生I2C起始信号
 * @param 无
 * @return 无
 * @author HZ12138
 * @date 2022-07-27 08:54:48
 */
void I2C_Start(void)
{
	I2C_Write_SDA(GPIO_PIN_SET);   //需在SCL之前设定
	I2C_Write_SCL(GPIO_PIN_SET);   // SCL->高
	I2C_Delay();				   //延时
	I2C_Write_SDA(GPIO_PIN_RESET); // SDA由1->0,产生开始信号
	I2C_Delay();				   //延时
	I2C_Write_SCL(GPIO_PIN_RESET); // SCL->低
}
/**
 * @brief 产生I2C结束信号
 * @param 无
 * @return 无
 * @author HZ12138
 * @date 2022-07-27 08:57:03
 */
void I2C_End(void)
{
	I2C_Write_SDA(GPIO_PIN_RESET); //在SCL之前拉低
	I2C_Write_SCL(GPIO_PIN_SET);   // SCL->高
	I2C_Delay();				   //延时
	I2C_Write_SDA(GPIO_PIN_SET);   // SDA由0->1,产生结束信号
	I2C_Delay();				   //延时
}
/**
 * @brief 发送应答码
 * @param ack:0 应答 1 不应达
 * @return 无
 * @author HZ12138
 * @date 2022-07-27 09:03:38
 */
void IIC_Send_ACK(uint8_t ack)
{
	if (ack == 1)
		I2C_Write_SDA(GPIO_PIN_SET); //产生应答电平
	else
		I2C_Write_SDA(GPIO_PIN_RESET);
	I2C_Delay();
	I2C_Write_SCL(GPIO_PIN_SET);   //发送应答信号
	I2C_Delay();				   //延时至少4us
	I2C_Write_SCL(GPIO_PIN_RESET); //整个期间保持应答信号
}
/**
 * @brief 接受应答码
 * @param 无
 * @return 应答码 0 应答 1 不应达
 * @author HZ12138
 * @date 2022-07-27 09:04:28
 */
uint8_t IIC_Get_ACK(void)
{
	uint8_t ret;				 //用来接收返回值
	I2C_Write_SDA(GPIO_PIN_SET); //电阻上拉,进入读
	I2C_Delay();
	I2C_Write_SCL(GPIO_PIN_SET); //进入应答检测
	I2C_Delay();				 //至少延时4us
	ret = I2C_Read_SDA();		 //保存应答信号
	I2C_Write_SCL(GPIO_PIN_RESET);
	return ret;
}
/**
 * @brief I2C写1Byte
 * @param dat:1Byte数据
 * @return 应答结果 0 应答 1 不应达
 * @author HZ12138
 * @date 2022-07-27 09:05:14
 */
uint8_t I2C_SendByte(uint8_t dat)
{
	uint8_t ack;
	for (int i = 0; i < 8; i++)
	{
		// 高在前低在后
		if (dat & 0x80)
			I2C_Write_SDA(GPIO_PIN_SET);
		else
			I2C_Write_SDA(GPIO_PIN_RESET);
		I2C_Delay();
		I2C_Write_SCL(GPIO_PIN_SET);
		I2C_Delay(); //延时至少4us
		I2C_Write_SCL(GPIO_PIN_RESET);
		dat <<= 1; //低位向高位移动
	}

	ack = IIC_Get_ACK();

	return ack;
}
/**
 * @brief I2C读取1Byte数据
 * @param ack:应答 0 应答 1 不应达
 * @return 接受到的数据
 * @author HZ12138
 * @date 2022-07-27 09:06:13
 */
uint8_t I2C_ReadByte(uint8_t ack)
{
	uint8_t ret = 0;
	// OLED_Read_SDA() 设置输入方向
	I2C_Write_SDA(GPIO_PIN_SET);
	for (int i = 0; i < 8; i++)
	{
		ret <<= 1;
		I2C_Write_SCL(GPIO_PIN_SET);
		I2C_Delay();
		// 高在前低在后
		if (I2C_Read_SDA())
		{
			ret++;
		}
		I2C_Write_SCL(GPIO_PIN_RESET);
		I2C_Delay();
	}

	IIC_Send_ACK(ack);

	return ret;
}
#define	SlaveAddress   0x3C	  //定义器件在IIC总线中的从地址
void Single_Write_HMC5883(unsigned char REG_Address,unsigned char REG_data)
{
    I2C_Start();                  //起始信号
    I2C_SendByte(SlaveAddress);   //发送设备地址+写信号
    I2C_SendByte(REG_Address);    //内部寄存器地址，请参考中文pdf 
    I2C_SendByte(REG_data);       //内部寄存器数据，请参考中文pdf
    I2C_End();                   //发送停止信号
}

void Multiple_Read_HMC5883(unsigned char *BUF)
{   unsigned char i;
    I2C_Start();                          //起始信号
    I2C_SendByte(SlaveAddress);           //发送设备地址+写信号
    I2C_SendByte(0x03);                   //发送存储单元地址，从0x3开始	
    I2C_Start();                          //起始信号
    I2C_SendByte(SlaveAddress+1);         //发送设备地址+读信号
	 for (i=0; i<6; i++)                      //连续读取6个地址数据，存储中BUF
    {
        
        if (i == 5)
        {
           BUF[i] = I2C_ReadByte(1);          //BUF[0]存储数据
        }
        else
        {
          BUF[i] = I2C_ReadByte(0);          //BUF[0]存储数据
       }
   }
    I2C_End();                          //停止信号
  //  Delay5ms();
}

void Init_HMC5883()
{
	//gpio已经在mx中使能
	Single_Write_HMC5883(0x00,(3<<5)|(1<<4)|(0<<0)); 
	Single_Write_HMC5883(0x01,(1<<5)); 
	Single_Write_HMC5883(0x02,0x00); 
}
double HMC5883_anglexy(unsigned char *BUF)
{
	int16_t x,y;
	double angle_xy;
	Multiple_Read_HMC5883(BUF);
	x=BUF[0] << 8 | BUF[1]; //Combine MSB and LSB of X Data output register  最高有效位
	
	y=BUF[4] << 8 | BUF[5]; //Combine MSB and LSB of Y Data output register
	angle_xy = atan2((double)y,(double)x)*(180/3.14159265)+180;
	return angle_xy;
}
