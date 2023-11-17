#include "bsp_pid.h"
#include "bsp_motor.h"

PID_Increment_Struct g_motor_left_pid = {10,1,0,50};



/**
 * @brief ��ʼ��pid����  �������
 */
void PID_param_init()
{
	
	
}
/**
  * @brief  ����Ŀ��ֵ
  * @param  val		Ŀ��ֵ
	*	@note 	��
  * @retval ��
  */
void set_pid_target(PID_Increment_Struct *pid, float temp_val)
{
  pid->target_val = temp_val;    // ���õ�ǰ��Ŀ��ֵ
}
/**
  * @brief  ��ȡĿ��ֵ
  * @param  ��
	*	@note 	��
  * @retval Ŀ��ֵ
  */
float get_pid_target(PID_Increment_Struct *pid)
{
  return pid->target_val;    // ���õ�ǰ��Ŀ��ֵ
}
void set_p_i_d(PID_Increment_Struct *pid, float p, float i, float d)
{
	pid->Kp = p;    // ���ñ���ϵ�� P
	pid->Ki = i;    // ���û���ϵ�� I
	pid->Kd = d;    // ����΢��ϵ�� D
}

//PID_Increment_Struct PID_openmv_control = {1, 0};&
float PID_Increment(PID_Increment_Struct *PID)
{
    float err,                                                                                                       //���
        out,                                                                                                         //���
        proportion,                                                                                                  //����
        differential;                                                                                                //΢��
    err = (float)PID->target_val - (float)PID->actual_val;                                                                            //�������
    proportion = (float)err - (float)PID->Error_Last1;                                                               //���������
    differential = (float)err - 2 * (float)PID->Error_Last1 + (float)PID->Error_Last2;                               //����΢����
    out = (float)PID->Out_Last + (float)PID->Kp * proportion + (float)PID->Ki * err + (float)PID->Kd * differential; //����PID
    PID->Error_Last2 = PID->Error_Last1;                                                                             //�������ϴ����
    PID->Error_Last1 = err;                                                                                          //�������
    PID->Out_Last = out;                                                                                             //�����ϴ����
    return out;
}



//void pid_speed_control()
//{

//	//��ȡ�ٶ�ֵ
//	left_speed = -Get_Speed(&);//��
//	right_speed = Get_Speed(&g_tim4_encoder_right_handle);
//	//��pid������Ҫ���õ��ٶ�ֵ
//	//aim_speed_left��aim_speed_right   Ŀ��ֵ
//	
//	set_speed_left = PID_Increment(&PID_Speed_left, left_speed, aim_speed_left);
//	//printf("%f,%f\n",right_speed,aim_speed_right);
//	set_speed_right = PID_Increment(&PID_Speed_right, right_speed, aim_speed_right);
//	
//	//����޷�
//	if (set_speed_right > 7000)
//            set_speed_right = 7000;
//        else if (set_speed_right < -7000)
//            set_speed_right = -7000;
//		
//		if (set_speed_left > 7000)
//            set_speed_left = 7000;
//        else if (set_speed_left < -7000)
//            set_speed_left = -7000;
//	//����ֵ���뵽pwm�У��Ѿ����Ʒ���
//	motor(set_speed_left,&g_tim1_npwm_chy_handle,TIM_CHANNEL_4);
//	motor(set_speed_right,&g_tim1_npwm_chy_handle,TIM_CHANNEL_1);
//}

//void pid_angle_control()
//{
//	
//	//��ȡ�Ƕ�
//	left_angle += (-Get_Angle(&g_tim2_encoder_left_handle));
//	right_angle += Get_Angle(&g_tim4_encoder_right_handle);
//		
//		//��ȡ�ٶ�
//		left_speed = -Get_Speed(&g_tim2_encoder_left_handle);//��
//		
//		right_speed = Get_Speed(&g_tim4_encoder_right_handle);
//		
//		//����Ŀ��Ƕȼ���   �����������ٶȵ�Ŀ��ֵ
//		aim_speed_left  = PID_Increment(&PID_Angle_left,left_angle,aim_angle);
//		aim_speed_right = PID_Increment(&PID_Angle_right,right_angle,aim_angle);
//		
//		//���Ŀ���ٶȵ��޷�
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
//		//�����ٶȻ�����
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

//			//�����ȶ��Ժ��������⣻
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




