#include "hmc5883.h"
#include <math.h>
#include "queue.h"
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
/********************************************************
* Function name ：insert_hhistory
* Description        : Insert to bd_host_history
* Parameter         ：
* @ipsql            SQL statement 
* @host_level        Risk level    
* @total            The total number of risk 
* @t_id            task id
* @t_uuid            task uuid
* @ipaddr            target ipaddr    
* @end_time        task end time
* Return          ：0 --success  ,  other -- fail
**********************************************************/
//int insert_hhistory(char* ipsql,risk host_level,int total,int t_id,char* t_uuid,char* ipaddr,long int end_time)
//{
//    /*
//    *    如果程序过于复杂，这里可以写明，具体算法和思路。
//    */
//}

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
//	static int16_t x_max = -999,x_min= 999,y_max= -999,y_min= 999;
	double angle_xy;
	Multiple_Read_HMC5883(BUF);
	x=BUF[0] << 8 | BUF[1]; //Combine MSB and LSB of X Data output register  最高有效位
	
	y=BUF[4] << 8 | BUF[5]; //Combine MSB and LSB of Y Data output register
	//磁场方向校准   需要采集xy两个轴的最大值和最小值
//	if(x>x_max)
//	{
//		x_max = x;
//	}
//	if(x<x_min)
//	{
//		x_min = x;
//	}
//	if(y>y_max)
//	{
//		y_max = y;
//	}
//	if(y<y_min)
//	{
//		y_min = y;
//	}
	x = x - ((329-370)/2);
	y = y - ((373-321)/2);
	angle_xy = atan2((double)y,(double)x)*(180/3.14159265);
//	if((x > 0)&&(y > 0)) angle_xy = atan2((double)y,(double)x)*(180/3.14159265);
//	else if((x > 0)&&(y < 0)) angle_xy = 360+atan2((double)y,(double)x)*(180/3.14159265);
//	else if((x == 0)&&(y > 0)) angle_xy = 90;
//	else if((x == 0)&&(y < 0)) angle_xy = 270;
//	else if(x < 0) angle_xy = atan2((double)y,(double)x)*(180/3.14159265)+180;

//	printf("angle:%d,%d %f\r\n",x_max,x_min,angle_xy);
//	printf("angle: %f\r\n",angle_xy);
	return angle_xy;
}
double calculateAngleDifference(double current_angle, double target_angle) {
    // 将角度限制在-180到180度范围内
    current_angle = fmod(current_angle + 180.0, 360.0) - 180.0;
    target_angle = fmod(target_angle + 180.0, 360.0) - 180.0;

    // 计算差值
    double angle_difference = target_angle - current_angle;

    // 处理角度差值的溢出情况
    if (angle_difference <= -180.0) {
        angle_difference += 360.0;
    } else if (angle_difference > 180.0) {
        angle_difference -= 360.0;
    }
    return angle_difference;
}
extern QueueHandle_t g_angle_data_quene;
void read_hmc_task(void * parms)
{
	uint8_t BUF[6];
	Angle_Data_Struct angle_data;
	double init_angle;
	Init_HMC5883();
	Multiple_Read_HMC5883(BUF);
	init_angle = HMC5883_anglexy(BUF);
	while(1)
	{
		Multiple_Read_HMC5883(BUF);
		angle_data.car_angle = HMC5883_anglexy(BUF)-init_angle;
		if (angle_data.car_angle <= -180.0) {
			angle_data.car_angle += 360.0;
		}else if (angle_data.car_angle > 180.0) {
			angle_data.car_angle -= 360.0;
		}
		xQueueSend(g_angle_data_quene,&angle_data.car_angle,portMAX_DELAY);
		vTaskDelay(20);
	}
}
