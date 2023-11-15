
#include "bsp_motor.h"
#include "tim.h"
//¶¨Ê±Æ÷x¾ä±ú
extern  TIM_HandleTypeDef htim4 ;

/**
 * @brief  £ºÓÒÂÖ³õÊ¼»¯
 * */
void right_motor_init(void)
{
	
}
/**
 * @brief  ÓÒÂÖÏòÇ°
 * */
void right_motor_go()
{
	HAL_GPIO_WritePin(RIGHT_AIN1_GPIO_PORT,RIGHT_AIN1_GPIO_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RIGHT_AIN2_GPIO_PORT,RIGHT_AIN2_GPIO_PIN,GPIO_PIN_SET);
	
}
/**
 * @brief  ÓÒÂÖºóÍË
 * */
void right_motor_return()
{
	HAL_GPIO_WritePin(RIGHT_AIN1_GPIO_PORT,RIGHT_AIN1_GPIO_PIN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(RIGHT_AIN2_GPIO_PORT,RIGHT_AIN2_GPIO_PIN,GPIO_PIN_RESET);
	
}
/**
 * @brief  £ºÓÒÂÖÍ£Ö¹
 * */
void right_motor_stop()
{
	HAL_GPIO_WritePin(RIGHT_AIN1_GPIO_PORT,RIGHT_AIN1_GPIO_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(RIGHT_AIN2_GPIO_PORT,RIGHT_AIN2_GPIO_PIN,GPIO_PIN_RESET);
	
}


/**
 * @brief  £º×óÂÖ³õÊ¼»¯__HAL_TIM_SET_COMPARE()
 * */
void left_motor_init(void)
{

	
}
/**
 * @brief  £º×óÂÖÇ°½ø
 * */
void left_motor_go()
{
	HAL_GPIO_WritePin(LEFT_AIN1_GPIO_PORT,LEFT_AIN1_GPIO_PIN,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LEFT_AIN2_GPIO_PORT,LEFT_AIN2_GPIO_PIN,GPIO_PIN_RESET);
	
}

/**
 * @brief  £º×óÂÖºóÍË
 * */
void left_motor_return()
{
	HAL_GPIO_WritePin(LEFT_AIN1_GPIO_PORT,LEFT_AIN1_GPIO_PIN,GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LEFT_AIN2_GPIO_PORT,LEFT_AIN2_GPIO_PIN,GPIO_PIN_SET);
	
}
/**
 * @brief  £º×óÂÖÍ£Ö¹
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


