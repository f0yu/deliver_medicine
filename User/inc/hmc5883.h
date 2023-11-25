#ifndef __HMC5883_H
#define __HMC5883_H			  	 

#include "main.h"
#include "stdlib.h"
#include "stm32f1xx_hal.h"


#define I2C_Group_SCL GPIOB // I2C的时钟GPIO组号
#define I2C_SCL 		GPIO_PIN_3  // I2C时钟的GPIO端口号

#define I2C_Group_SDA 	GPIOB // I2C的数据GPIO组号
#define I2C_SDA 		GPIO_PIN_4  // I2C数据的GPIO端口号

#define I2C_Write_SCL(x) HAL_GPIO_WritePin(I2C_Group_SCL, I2C_SCL, x)
#define I2C_Write_SDA(x) HAL_GPIO_WritePin(I2C_Group_SDA, I2C_SDA, x)

#define I2C_Read_SCL() HAL_GPIO_ReadPin(I2C_Group_SCL, I2C_SCL)
#define I2C_Read_SDA() HAL_GPIO_ReadPin(I2C_Group_SDA, I2C_SDA)

void Init_HMC5883(void );
void Multiple_Read_HMC5883(unsigned char *BUF);
double HMC5883_anglexy(unsigned char *BUF);
void read_hmc_task(void * parms);
#endif  //__HMC5883_H


