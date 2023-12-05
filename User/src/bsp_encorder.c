#include "bsp_encoder.h"
#include "tim.h"
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;

void Encoder_Init(void)
{
	
	MX_TIM2_Init();
	MX_TIM4_Init();
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_1);   // ¿ªÆô±àÂëÆ÷A
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_2);   // ¿ªÆô±àÂëÆ÷B	
	
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_1);   // ¿ªÆô±àÂëÆ÷A
	HAL_TIM_Encoder_Start(&htim2, TIM_CHANNEL_2);   // ¿ªÆô±àÂëÆ÷B	
	
}
float LowPassFilter(float x)
{
	static float y_prev = 0;
//    float alpha = 10/(10 + 10);
    float y = 0.5f*y_prev + (1.0f - 0.5f)*x;
    y_prev = y;
    return y;
}

float Get_Speed(TIM_HandleTypeDef *Encoder_TIM_Handle)
{
    int16_t zj;
    float Speed = 0;
    zj = __HAL_TIM_GetCounter(Encoder_TIM_Handle);
    __HAL_TIM_SetCounter(Encoder_TIM_Handle, 0);
//	printf("car_speed: %d,\r\n",zj);
    Speed = (float)(zj * 100 * 60) / (4 * 448 * 30);
//	Speed = LowPassFilter(Speed);
    return Speed;
}

float Get_Angle(TIM_HandleTypeDef *Encoder_TIM_Handle)
{
    int16_t zj;
    float angle = 0;
    zj = __HAL_TIM_GetCounter(Encoder_TIM_Handle);
    angle = (float)zj* 360 / (4 * 448 * 30) ;
    return angle;
}

