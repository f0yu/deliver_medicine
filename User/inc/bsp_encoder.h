#ifndef __BSP_ENCODER_H__
#define __BSP_ENCODER_H__

#include "stm32f1xx_hal.h"
typedef struct __Speed_Data_Struct
{
    float left_speed,right_speed,left_angel,right_angel;
}Speed_Data_Struct;

void Encoder_Init(void);
float Get_Speed(TIM_HandleTypeDef *Encoder_TIM_Handle);
float Get_Angle(TIM_HandleTypeDef *Encoder_TIM_Handle);

#endif /*__BSP_ENCODER_H__ */

