
#ifndef __BSP_MOTOR_H__
#define __BSP_MOTOR_H__

#include "stm32f1xx_hal.h"

#define RIGHT_AIN1_GPIO_PORT                  GPIOB
#define RIGHT_AIN1_GPIO_PIN                   GPIO_PIN_14
#define RIGHT_AIN1_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define RIGHT_AIN2_GPIO_PORT                  GPIOB
#define RIGHT_AIN2_GPIO_PIN                   GPIO_PIN_15
#define RIGHT_AIN2_CLK_ENABLE()               do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define LEFT_AIN1_GPIO_PORT                  GPIOB
#define LEFT_AIN1_GPIO_PIN                   GPIO_PIN_12
#define LEFT_AIN1_GPIO_CLK_ENABLE()          do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */

#define LEFT_AIN2_GPIO_PORT                  GPIOB
#define LEFT_AIN2_GPIO_PIN                   GPIO_PIN_13
#define LEFT_AIN2_CLK_ENABLE()               do{ __HAL_RCC_GPIOB_CLK_ENABLE(); }while(0)   /* PA口时钟使能 */


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
/* 高级定时器 定义 */

 /* TIMX 输出指定个数PWM 定义 
 * 这里输出的PWM通过PC6(TIM8_CH1)输出, 我们用杜邦线连接PC6和PE5, 然后在程序里面将PE5设置成浮空输入
 * 就可以 看到TIM8_CH1控制LED1(GREEN)的亮灭, 亮灭一次表示一个PWM波
 * 默认使用的是TIM8_CH1.
 * 注意: 通过修改这几个宏定义, 可以支持TIM1/TIM8定时器, 任意一个IO口输出指定个数的PWM
 */
 
//#define MOTOR_TIMER_PWM_CHY_GPIO_PORT            GPIOA
//#define MOTOR_TIMER_PWM_CHY_GPIO_PIN             GPIO_PIN_8|GPIO_PIN_11
//#define MOTOR_TIMER_PWM_CHY_GPIO_CLK_ENABLE()    do{  __HAL_RCC_GPIOA_CLK_ENABLE(); }while(0)   /* PC口时钟使能 */

//#define MOTOR_TIMER_PWM                          TIM1
//#define MOTOR_TIMER_PWM_IRQn                     TIM1_UP_IRQn
//#define MOTOR_TIMER_PWM_IRQHandler               TIM1_UP_IRQHandler
//#define MOTOR_TIMER_PWM_CHY                      TIM_CHANNEL_1                           /* 通道Y,  1<= Y <=4 */
//#define MOTOR_TIMER_PWM_CHY_CCRX                 TIM1->CCR1                  /* 通道Y的输出比较寄存器 */
//#define MOTOR_TIMER_PWM_CHY_CLK_ENABLE()         do{ __HAL_RCC_TIM1_CLK_ENABLE(); }while(0)  /* TIM8 时钟使能 */


/******************************************************************************************/
void motor_pwm_init(void);


#endif /*__ GPIO_H__ */

