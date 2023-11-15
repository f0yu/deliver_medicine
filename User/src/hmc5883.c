#include "hmc5883.h"
#include <math.h>
//���iic

/**
 * @brief һ���ӳ�
 * @param ��
 * @return ��
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
 * @brief ����I2C��ʼ�ź�
 * @param ��
 * @return ��
 * @author HZ12138
 * @date 2022-07-27 08:54:48
 */
void I2C_Start(void)
{
	I2C_Write_SDA(GPIO_PIN_SET);   //����SCL֮ǰ�趨
	I2C_Write_SCL(GPIO_PIN_SET);   // SCL->��
	I2C_Delay();				   //��ʱ
	I2C_Write_SDA(GPIO_PIN_RESET); // SDA��1->0,������ʼ�ź�
	I2C_Delay();				   //��ʱ
	I2C_Write_SCL(GPIO_PIN_RESET); // SCL->��
}
/**
 * @brief ����I2C�����ź�
 * @param ��
 * @return ��
 * @author HZ12138
 * @date 2022-07-27 08:57:03
 */
void I2C_End(void)
{
	I2C_Write_SDA(GPIO_PIN_RESET); //��SCL֮ǰ����
	I2C_Write_SCL(GPIO_PIN_SET);   // SCL->��
	I2C_Delay();				   //��ʱ
	I2C_Write_SDA(GPIO_PIN_SET);   // SDA��0->1,���������ź�
	I2C_Delay();				   //��ʱ
}
/**
 * @brief ����Ӧ����
 * @param ack:0 Ӧ�� 1 ��Ӧ��
 * @return ��
 * @author HZ12138
 * @date 2022-07-27 09:03:38
 */
void IIC_Send_ACK(uint8_t ack)
{
	if (ack == 1)
		I2C_Write_SDA(GPIO_PIN_SET); //����Ӧ���ƽ
	else
		I2C_Write_SDA(GPIO_PIN_RESET);
	I2C_Delay();
	I2C_Write_SCL(GPIO_PIN_SET);   //����Ӧ���ź�
	I2C_Delay();				   //��ʱ����4us
	I2C_Write_SCL(GPIO_PIN_RESET); //�����ڼ䱣��Ӧ���ź�
}
/**
 * @brief ����Ӧ����
 * @param ��
 * @return Ӧ���� 0 Ӧ�� 1 ��Ӧ��
 * @author HZ12138
 * @date 2022-07-27 09:04:28
 */
uint8_t IIC_Get_ACK(void)
{
	uint8_t ret;				 //�������շ���ֵ
	I2C_Write_SDA(GPIO_PIN_SET); //��������,�����
	I2C_Delay();
	I2C_Write_SCL(GPIO_PIN_SET); //����Ӧ����
	I2C_Delay();				 //������ʱ4us
	ret = I2C_Read_SDA();		 //����Ӧ���ź�
	I2C_Write_SCL(GPIO_PIN_RESET);
	return ret;
}
/**
 * @brief I2Cд1Byte
 * @param dat:1Byte����
 * @return Ӧ���� 0 Ӧ�� 1 ��Ӧ��
 * @author HZ12138
 * @date 2022-07-27 09:05:14
 */
uint8_t I2C_SendByte(uint8_t dat)
{
	uint8_t ack;
	for (int i = 0; i < 8; i++)
	{
		// ����ǰ���ں�
		if (dat & 0x80)
			I2C_Write_SDA(GPIO_PIN_SET);
		else
			I2C_Write_SDA(GPIO_PIN_RESET);
		I2C_Delay();
		I2C_Write_SCL(GPIO_PIN_SET);
		I2C_Delay(); //��ʱ����4us
		I2C_Write_SCL(GPIO_PIN_RESET);
		dat <<= 1; //��λ���λ�ƶ�
	}

	ack = IIC_Get_ACK();

	return ack;
}
/**
 * @brief I2C��ȡ1Byte����
 * @param ack:Ӧ�� 0 Ӧ�� 1 ��Ӧ��
 * @return ���ܵ�������
 * @author HZ12138
 * @date 2022-07-27 09:06:13
 */
uint8_t I2C_ReadByte(uint8_t ack)
{
	uint8_t ret = 0;
	// OLED_Read_SDA() �������뷽��
	I2C_Write_SDA(GPIO_PIN_SET);
	for (int i = 0; i < 8; i++)
	{
		ret <<= 1;
		I2C_Write_SCL(GPIO_PIN_SET);
		I2C_Delay();
		// ����ǰ���ں�
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
#define	SlaveAddress   0x3C	  //����������IIC�����еĴӵ�ַ
void Single_Write_HMC5883(unsigned char REG_Address,unsigned char REG_data)
{
    I2C_Start();                  //��ʼ�ź�
    I2C_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    I2C_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ����ο�����pdf 
    I2C_SendByte(REG_data);       //�ڲ��Ĵ������ݣ���ο�����pdf
    I2C_End();                   //����ֹͣ�ź�
}

void Multiple_Read_HMC5883(unsigned char *BUF)
{   unsigned char i;
    I2C_Start();                          //��ʼ�ź�
    I2C_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
    I2C_SendByte(0x03);                   //���ʹ洢��Ԫ��ַ����0x3��ʼ	
    I2C_Start();                          //��ʼ�ź�
    I2C_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
	 for (i=0; i<6; i++)                      //������ȡ6����ַ���ݣ��洢��BUF
    {
        
        if (i == 5)
        {
           BUF[i] = I2C_ReadByte(1);          //BUF[0]�洢����
        }
        else
        {
          BUF[i] = I2C_ReadByte(0);          //BUF[0]�洢����
       }
   }
    I2C_End();                          //ֹͣ�ź�
  //  Delay5ms();
}

void Init_HMC5883()
{
	//gpio�Ѿ���mx��ʹ��
	Single_Write_HMC5883(0x00,(3<<5)|(1<<4)|(0<<0)); 
	Single_Write_HMC5883(0x01,(1<<5)); 
	Single_Write_HMC5883(0x02,0x00); 
}
double HMC5883_anglexy(unsigned char *BUF)
{
	int16_t x,y;
	double angle_xy;
	Multiple_Read_HMC5883(BUF);
	x=BUF[0] << 8 | BUF[1]; //Combine MSB and LSB of X Data output register  �����Чλ
	
	y=BUF[4] << 8 | BUF[5]; //Combine MSB and LSB of Y Data output register
	angle_xy = atan2((double)y,(double)x)*(180/3.14159265)+180;
	return angle_xy;
}
