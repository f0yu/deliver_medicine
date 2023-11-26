
#include "bsp_motor.h"
#include "tim.h"
//��ʱ��x���
extern  TIM_HandleTypeDef htim4 ;

/**
 * @brief  �����ֳ�ʼ��
 * */
void right_motor_init(void)
{
	
}
/**
 * @brief  ������ǰ
 * */
void right_motor_go()
{
	HAL_GPIO_WritePin(RIGHT_AIN1_GPIO_PORT,RIGHT_AIN1_GPIO_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RIGHT_AIN2_GPIO_PORT,RIGHT_AIN2_GPIO_PIN,GPIO_PIN_SET);
	
}
/**
 * @brief  ���ֺ���
 * */
void right_motor_return()
{
	HAL_GPIO_WritePin(RIGHT_AIN1_GPIO_PORT,RIGHT_AIN1_GPIO_PIN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(RIGHT_AIN2_GPIO_PORT,RIGHT_AIN2_GPIO_PIN,GPIO_PIN_RESET);
	
}
/**
 * @brief  ������ֹͣ
 * */
void right_motor_stop()
{
	HAL_GPIO_WritePin(RIGHT_AIN1_GPIO_PORT,RIGHT_AIN1_GPIO_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RIGHT_AIN2_GPIO_PORT,RIGHT_AIN2_GPIO_PIN,GPIO_PIN_RESET);
	
}


/**
 * @brief  �����ֳ�ʼ��__HAL_TIM_SET_COMPARE()
 * */
void left_motor_init(void)
{

	
}
/**
 * @brief  ������ǰ��
 * */
void left_motor_go()
{
	HAL_GPIO_WritePin(LEFT_AIN1_GPIO_PORT,LEFT_AIN1_GPIO_PIN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LEFT_AIN2_GPIO_PORT,LEFT_AIN2_GPIO_PIN,GPIO_PIN_RESET);
	
}

/**
 * @brief  �����ֺ���
 * */
void left_motor_return()
{
	HAL_GPIO_WritePin(LEFT_AIN1_GPIO_PORT,LEFT_AIN1_GPIO_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LEFT_AIN2_GPIO_PORT,LEFT_AIN2_GPIO_PIN,GPIO_PIN_SET);
	
}
/**
 * @brief  ������ֹͣ
 * */
void left_motor_stop()
{
	HAL_GPIO_WritePin(LEFT_AIN1_GPIO_PORT,LEFT_AIN1_GPIO_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LEFT_AIN2_GPIO_PORT,LEFT_AIN2_GPIO_PIN,GPIO_PIN_RESET);
	
}



void motor_pwm_init()
{
	MX_TIM1_Init();
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_1);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
//	TIM1->CCR1 = 2500;
//	TIM1->CCR4 = 2500;
}

void motor(int16_t Speed,TIM_HandleTypeDef *Motor_TIM_Handle,uint32_t TIM_CHANNEL_x)
{
	if(Speed==0)
	{
		if(TIM_CHANNEL_x == TIM_CHANNEL_4 )
		{
			right_motor_stop();
		}else if(TIM_CHANNEL_x == TIM_CHANNEL_1)
		{
			left_motor_stop();
		}
		__HAL_TIM_SET_COMPARE(Motor_TIM_Handle,TIM_CHANNEL_x,0);
	}else if(Speed>0)
	{
		if(TIM_CHANNEL_x == TIM_CHANNEL_4)
		{
			right_motor_go();
		}else if(TIM_CHANNEL_x == TIM_CHANNEL_1)
		{
			left_motor_go();
		}
		__HAL_TIM_SET_COMPARE(Motor_TIM_Handle,TIM_CHANNEL_x,Speed);
	}else if(Speed<0)
	{
		Speed = -Speed;
		if(TIM_CHANNEL_x == TIM_CHANNEL_4)
		{
			right_motor_return();
		}else if(TIM_CHANNEL_x == TIM_CHANNEL_1)
		{
			left_motor_return();
		}
		__HAL_TIM_SET_COMPARE(Motor_TIM_Handle,TIM_CHANNEL_x,Speed);
	}
}

