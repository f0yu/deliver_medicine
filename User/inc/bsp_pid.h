#ifndef __BSP_PID_H__
#define __BSP_PID_H__

#include "stm32f1xx_hal.h"


typedef struct __PID_Increment_Struct
{
    float Kp, Ki, Kd;  //ϵ��
	float target_val;  //Ŀ��ֵ
    float Error_Last1; //�ϴ����
    float Error_Last2; //�ϴ����
    float Out_Last;    //�ϴ����
    float actual_val;  //ʵ��ֵ
}PID_Increment_Struct;



float PID_Increment(PID_Increment_Struct *PID);

void pid_angle_control(void);
void pid_speed_control(void);
void pid_openmv_control(void);



extern PID_Increment_Struct g_motor_right_pid;
extern PID_Increment_Struct g_motor_left_pid;
#endif /*__BSP_PID_H__ */


