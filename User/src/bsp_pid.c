#include "bsp_pid.h"
#include "bsp_motor.h"

PID_Increment_Struct g_motor_left_pid = {10,1,0,50};



/**
 * @brief 初始化pid参数  方便调参
 */
void PID_param_init()
{
	
	
}
/**
  * @brief  设置目标值
  * @param  val		目标值
	*	@note 	无
  * @retval 无
  */
void set_pid_target(PID_Increment_Struct *pid, float temp_val)
{
  pid->target_val = temp_val;    // 设置当前的目标值
}
/**
  * @brief  获取目标值
  * @param  无
	*	@note 	无
  * @retval 目标值
  */
float get_pid_target(PID_Increment_Struct *pid)
{
  return pid->target_val;    // 设置当前的目标值
}
void set_p_i_d(PID_Increment_Struct *pid, float p, float i, float d)
{
	pid->Kp = p;    // 设置比例系数 P
	pid->Ki = i;    // 设置积分系数 I
	pid->Kd = d;    // 设置微分系数 D
}

//PID_Increment_Struct PID_openmv_control = {1, 0};&
float PID_Increment(PID_Increment_Struct *PID)
{
    float err,                                                                                                       //误差
        out,                                                                                                         //输出
        proportion,                                                                                                  //比例
        differential;                                                                                                //微分
    err = (float)PID->target_val - (float)PID->actual_val;                                                                            //计算误差
    proportion = (float)err - (float)PID->Error_Last1;                                                               //计算比例项
    differential = (float)err - 2 * (float)PID->Error_Last1 + (float)PID->Error_Last2;                               //计算微分项
    out = (float)PID->Out_Last + (float)PID->Kp * proportion + (float)PID->Ki * err + (float)PID->Kd * differential; //计算PID
    PID->Error_Last2 = PID->Error_Last1;                                                                             //更新上上次误差
    PID->Error_Last1 = err;                                                                                          //更新误差
    PID->Out_Last = out;                                                                                             //更新上此输出
    return out;
}



//void pid_speed_control()
//{

//	//获取速度值
//	left_speed = -Get_Speed(&);//左
//	right_speed = Get_Speed(&g_tim4_encoder_right_handle);
//	//用pid计算需要设置的速度值
//	//aim_speed_left，aim_speed_right   目标值
//	
//	set_speed_left = PID_Increment(&PID_Speed_left, left_speed, aim_speed_left);
//	//printf("%f,%f\n",right_speed,aim_speed_right);
//	set_speed_right = PID_Increment(&PID_Speed_right, right_speed, aim_speed_right);
//	
//	//输出限幅
//	if (set_speed_right > 7000)
//            set_speed_right = 7000;
//        else if (set_speed_right < -7000)
//            set_speed_right = -7000;
//		
//		if (set_speed_left > 7000)
//            set_speed_left = 7000;
//        else if (set_speed_left < -7000)
//            set_speed_left = -7000;
//	//将数值载入到pwm中，已经控制方向
//	motor(set_speed_left,&g_tim1_npwm_chy_handle,TIM_CHANNEL_4);
//	motor(set_speed_right,&g_tim1_npwm_chy_handle,TIM_CHANNEL_1);
//}

//void pid_angle_control()
//{
//	
//	//获取角度
//	left_angle += (-Get_Angle(&g_tim2_encoder_left_handle));
//	right_angle += Get_Angle(&g_tim4_encoder_right_handle);
//		
//		//获取速度
//		left_speed = -Get_Speed(&g_tim2_encoder_left_handle);//左
//		
//		right_speed = Get_Speed(&g_tim4_encoder_right_handle);
//		
//		//进行目标角度计算   给出来的是速度的目标值
//		aim_speed_left  = PID_Increment(&PID_Angle_left,left_angle,aim_angle);
//		aim_speed_right = PID_Increment(&PID_Angle_right,right_angle,aim_angle);
//		
//		//输出目标速度的限幅
//		if (PID_Angle_left.Error_Last1 > 360)
//		{
//            aim_speed_left = 180;
//		}
//        else if (PID_Angle_left.Error_Last1 < -360)
//        {
//            aim_speed_left = -180;
//		}
//		
//		
//		if (PID_Angle_right.Error_Last1 > 360)
//		{
//			aim_speed_right = 180; 
//		}
//        else if (PID_Angle_right.Error_Last1 < -360)
//            {
//			aim_speed_right = -180; 
//		}
//		
//		
//		//进行速度环控制
//		set_speed_left = PID_Increment(&PID_Speed_left, left_speed, aim_speed_left);
//		//printf("%f,%f\n",right_speed,aim_speed_right);
//		set_speed_right = PID_Increment(&PID_Speed_right, right_speed, aim_speed_right);
//		
//		
//		if (set_speed_right > 6000)
//            set_speed_right = 6000;
//        else if (set_speed_right < -6000)
//            set_speed_right = -6000;
//		
//		if (set_speed_left > 6000)
//            set_speed_left = 6000;
//        else if (set_speed_left < -6000)
//            set_speed_left = -6000;
//		
//		motor(set_speed_left,&g_tim1_npwm_chy_handle,TIM_CHANNEL_4);
//		motor(set_speed_right,&g_tim1_npwm_chy_handle,TIM_CHANNEL_1);
//	
//	
////}
//void pid_openmv_control()
//{
//	if(g_usart_rx_sta_3 >= 0x8000)
//		{
//			//openmv_dat_flag_success = 0;
//			
//			openmv_dat_analysis();
//			if((openmv_number.line_flag == 1)&& openmv_number.line_error>=0)
//			{
//				openmv_number.line_error  = -openmv_number.line_error;
//			}else
//			{
//				
//			}
//			
//			if(openmv_number.line_error>30 || openmv_number.line_error<-30)
//			{
//				openmv_number.line_error =0;
//			}
//			
//			
//			//openmv_number.line_error = openmv_number.line_error-5;
//			
//			
//			
//			if((openmv_number.angle_flag == 1)&& openmv_number.angle_error >=0)
//			{
//				openmv_number.angle_error  = -openmv_number.angle_error;
//			}
//			
//			if(openmv_number.angle_error>80 || openmv_number.angle_error < -80)
//			{
//				openmv_number.angle_error =0;
//			}
//			//printf("%d\n",openmv_number.line_error);
//			
//			//aim_speed = PID_Increment(&PID_openmv_control,(float)openmv_number.line_error,0);
//			//80	1.3
//			//50	0.9
////			aim_speed_left = 	100+openmv_number.line_error*1.4;
////			aim_speed_right = 	100-openmv_number.line_error*1.4;

//			//数据稳定性好像有问题；
////			aim_speed_left = 	150+openmv_number.line_error*1.7;
////			aim_speed_right = 	150-openmv_number.line_error*1.8;
//			
//			if(openmv_number.angle_flag != 3)
//			{
//				
//			aim_speed = PID_Increment(&PID_openmv_control,(float)openmv_number.line_error,0);
//			aim_speed_angle  = PID_Increment(&PID_openmv_angle_control,(float)openmv_number.angle_error,0);
//			aim_speed_left  = 	150+aim_speed+aim_speed_angle-1;
//			aim_speed_right = 	150-aim_speed-aim_speed_angle+1;
//				
//				if(aim_speed_left<0 ||aim_speed_right <0)
//				{
//					aim_speed_left = 150;
//					aim_speed_right = 150;
//				}
//				
//				if(aim_speed_left>230 ||aim_speed_right >230)
//				{
//					aim_speed_left = 150;
//					aim_speed_right = 150;
//				}
//				
//				if(turn_flag )
//				{
//					turn_flag =0;
//					turn_cnt ++;
//				}
//				
//			}else
//			{
//				if(turn_cnt < ring_determine)
//				{
//					turn_flag = 1;
//					aim_speed_left = 150;
//					aim_speed_right = 150;
//				}else
//				{
//					aim_speed_left = 150;
//					aim_speed_right = 150;
//					
//				}
//			}
//			
//			aim_speed_left *= car_speed_param;
//			aim_speed_right *= car_speed_param;


//			HAL_UART_Transmit_IT(&huart2, (uint8_t *)CO2AskBuffer, sizeof(CO2AskBuffer));
////			aim_speed_left = aim_speed_left*0.5;
////			aim_speed_right = aim_speed_right*0.5;
////			aim_speed_left = aim_speed_left*0.8;
////			aim_speed_right = aim_speed_right*0.8;
//			
//			
////			aim_speed_left = aim_speed_left*1.4;
////			aim_speed_right = aim_speed_right*1.4;
//			
//			
//			//printf("%d\n",openmv)_number.angle_flag);
//			//printf("%f,%f,%d\n",aim_speed_left,aim_speed_right,(int)aim_speed);
//			
////			aim_speed_left  = 	100+aim_speed+aim_speed_angle;
////			aim_speed_right = 	100-aim_speed-aim_speed_angle;

//			
//			//printf("%f,%f,%d\n",aim_speed_left,aim_speed_right,(int)aim_speed);

//		}
//}




