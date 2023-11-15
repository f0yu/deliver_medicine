#ifndef __BSP_PID_H__
#define __BSP_PID_H__

#include "stm32f1xx_hal.h"


typedef struct __PID_Increment_Struct
{
    float Kp, Ki, Kd;  //ϵ��
    float Error_Last1; //�ϴ����
    float Error_Last2; //�ϴ����
    float Out_Last;    //�ϴ����
} PID_Increment_Struct;



void motor(int16_t Speed,TIM_HandleTypeDef *Motor_TIM_Handle,uint32_t TIM_CHANNEL_x);

float PID_Increment(PID_Increment_Struct *PID, float Current, float Target);

void pid_angle_control(void);
void pid_speed_control(void);
void pid_openmv_control(void);

#endif /*__BSP_PID_H__ */


