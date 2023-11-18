#ifndef __BSP_PID_H__
#define __BSP_PID_H__

#include "stm32f1xx_hal.h"


typedef struct __PID_Increment_Struct
{
    float Kp, Ki, Kd;  //系数
	float target_val;  //目标值
    float Error_Last1; //上次误差
    float Error_Last2; //上次误差
    float Out_Last;    //上次输出
    float actual_val;  //实际值
}PID_Increment_Struct;



float PID_Increment(PID_Increment_Struct *PID);

void pid_angle_control(void);
void pid_speed_control(void);
void pid_openmv_control(void);



extern PID_Increment_Struct g_motor_right_pid;
extern PID_Increment_Struct g_motor_left_pid;
#endif /*__BSP_PID_H__ */


