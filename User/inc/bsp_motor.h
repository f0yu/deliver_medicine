
#ifndef __BSP_MOTOR_H__
#define __BSP_MOTOR_H__

#include "stm32f1xx_hal.h"

#define RIGHT_AIN1_GPIO_PORT                  GPIOB
#define RIGHT_AIN1_GPIO_PIN                   GPIO_PIN_14
#define RIGHT_AIN1_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

#define RIGHT_AIN2_GPIO_PORT                  GPIOB
#define RIGHT_AIN2_GPIO_PIN                   GPIO_PIN_15
#define RIGHT_AIN2_CLK_ENABLE()               do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

#define LEFT_AIN1_GPIO_PORT                  GPIOB
#define LEFT_AIN1_GPIO_PIN                   GPIO_PIN_12
#define LEFT_AIN1_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */

#define LEFT_AIN2_GPIO_PORT                  GPIOB
#define LEFT_AIN2_GPIO_PIN                   GPIO_PIN_13
#define LEFT_AIN2_CLK_ENABLE()               do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA��ʱ��ʹ�� */


//void right_motor_init(void);

void right_motor_go(void);

void right_motor_return(void);
void right_motor_stop(void);


//void left_motor_init(void);

void left_motor_go(void);

void left_motor_return(void);
void left_motor_stop(void);
void motor(int16_t Speed,TIM_HandleTypeDef *Motor_TIM_Handle,uint32_t TIM_CHANNEL_x);
/******************************************************************************************/
/* �߼���ʱ�� ���� */

 /* TIMX ���ָ������PWM ���� 
 * ���������PWMͨ��PC6(TIM8_CH1)���, �����öŰ�������PC6��PE5, Ȼ���ڳ������潫PE5���óɸ�������
 * �Ϳ��� ����TIM8_CH1����LED1(GREEN)������, ����һ�α�ʾһ��PWM��
 * Ĭ��ʹ�õ���TIM8_CH1.
 * ע��: ͨ���޸��⼸���궨��, ����֧��TIM1/TIM8��ʱ��, ����һ��IO�����ָ��������PWM
 */
 
//#define MOTOR_TIMER_PWM_CHY_GPIO_PORT            GPIOA
//#define MOTOR_TIMER_PWM_CHY_GPIO_PIN             GPIO_PIN_8|GPIO_PIN_11
//#define MOTOR_TIMER_PWM_CHY_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PC��ʱ��ʹ�� */

//#define MOTOR_TIMER_PWM                          TIM1
//#define MOTOR_TIMER_PWM_IRQn                     TIM1_UP_IRQn
//#define MOTOR_TIMER_PWM_IRQHandler               TIM1_UP_IRQHandler
//#define MOTOR_TIMER_PWM_CHY                      TIM_CHANNEL_1                           /* ͨ��Y,  1<= Y <=4 */
//#define MOTOR_TIMER_PWM_CHY_CCRX                 TIM1->CCR1                  /* ͨ��Y������ȽϼĴ��� */
//#define MOTOR_TIMER_PWM_CHY_CLK_ENABLE()         do{ __HAL_RCC_TIM1_CLK_ENABLE(); }while(0)  /* TIM8 ʱ��ʹ�� */


/******************************************************************************************/
void motor_pwm_init(void);


#endif /*__ GPIO_H__ */

