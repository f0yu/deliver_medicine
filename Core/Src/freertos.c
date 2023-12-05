/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include  <math.h>    //Keil library 
#include "timers.h"
#include "queue.h"
#include "usart.h"
#include "vl53l0x_api.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
void pid_control(void *params);

void lcd_test_task(void *params);
void motor_time_isq(void *params);
void logic_control(void *param);

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
//static  char pcWriteBuffer[200];


TimerHandle_t g_motor_timer;

QueueSetHandle_t g_xQueueSetInput;
QueueHandle_t g_speed_data_quene;
QueueHandle_t g_key_data_quene;
QueueHandle_t g_speed_aim_quene;

QueueHandle_t g_distance_data_quene;
QueueHandle_t g_angle_data_quene;
QueueHandle_t g_uart_data_quene;

TaskHandle_t g_read_hmc_task;
TaskHandle_t g_measure_distance;


/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* Hook prototypes */
void vApplicationIdleHook(void);

/* USER CODE BEGIN 2 */
void vApplicationIdleHook( void )
{
   /* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
   to 1 in FreeRTOSConfig.h. It will be called on each iteration of the idle
   task. It is essential that code added to this hook function never attempts
   to block in any way (for example, call xQueueReceive() with a block time
   specified, or call vTaskDelay()). If the application makes use of the
   vTaskDelete() API function (as this demo application does) then it is also
   important that vApplicationIdleHook() is permitted to return to its calling
   function, because it is the responsibility of the idle task to clean up
   memory allocated by the kernel to any task that has since been deleted. */
//	int i;
//	vTaskList((char *)pcWriteBuffer);
////	for (i = 0; i < 16; i++)
////		printf("-");
//	printf("\n\r");
//	printf("%s\n\r", pcWriteBuffer);
	
}
/* USER CODE END 2 */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
//	OLED_Init();
//	OLED_Clear();
//	Init_HMC5883();

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
	g_motor_timer = xTimerCreate("motor_timer",10,pdTRUE,NULL,motor_time_isq);
	
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
	g_speed_data_quene = xQueueCreate(1,sizeof(Speed_Data_Struct));
	g_key_data_quene = xQueueCreate(1,sizeof(Key_Data));
	g_speed_aim_quene = xQueueCreate(1,sizeof(Speed_AIM_Data_Struct));
	g_distance_data_quene = xQueueCreate(1,sizeof(Distance_Data_Struct));
	g_angle_data_quene = xQueueCreate(1,sizeof(Angle_Data_Struct));
	g_uart_data_quene = xQueueCreate(1,sizeof(void*));
	
	
	g_xQueueSetInput = xQueueCreateSet(4);
	xQueueAddToSet(g_distance_data_quene,g_xQueueSetInput);
	xQueueAddToSet(g_angle_data_quene,g_xQueueSetInput);
	xQueueAddToSet(g_uart_data_quene,g_xQueueSetInput);
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
//  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
///// 	xTaskCreate(lcd_test_task, "lcd_test", 256, NULL, osPriorityNormal, NULL);
	xTaskCreate(key_task, "key_task", 128, NULL, osPriorityNormal, NULL);
	xTaskCreate(read_hmc_task, "hmc_task", 256, NULL, osPriorityNormal,&g_read_hmc_task);
//	xTaskCreate(measure_distance, "dis_task", 128, NULL, osPriorityNormal, &g_measure_distance);
	xTaskCreate(logic_control, "logic_control", 256, NULL, osPriorityNormal+1, NULL);
	OLED_Init();
	Menu_Init();
//	HAL_Delay(100);
	
//	OLED_Clear();
	Menu_Task_Create();
	xTaskCreate(pid_control, "pid_control", 256, NULL, osPriorityNormal+2, NULL);
//	xTaskCreate(fire_pid_task, "fire_pid", 500, NULL, osPriorityNormal, NULL);


	xTimerStart(g_motor_timer,0);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
	vTaskDelete(NULL);
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void motor_time_isq(void *params)
{
	BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	Speed_Data_Struct car_speed;
	car_speed.left_angel =   Get_Angle(&htim2);
	car_speed.right_angel =   Get_Angle(&htim4);
	car_speed.left_speed =   -Get_Speed(&htim2);
	car_speed.right_speed =   Get_Speed(&htim4);

	xQueueOverwriteFromISR(g_speed_data_quene,&car_speed,&xHigherPriorityTaskWoken);
//	printf("car_speed: %f,%f\r\n",car_speed.right_speed,car_speed.left_speed);
}
void pid_control(void *params)
{
	Speed_Data_Struct car_speed ={0,0,0,0};
	Speed_AIM_Data_Struct speed_aim_data = {0,0};
	int16_t pid_data_right = 0;
	int16_t pid_data_left = 0;
//	TIM1->CCR1 = 0;
//	TIM1->CCR4 = 0;
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13|GPIO_PIN_15, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14|GPIO_PIN_12, GPIO_PIN_RESET);
	while(1)
	{
		xQueuePeek(g_speed_aim_quene,&speed_aim_data,0);
		g_motor_right_pid.target_val = speed_aim_data.right_speed_aim;
		g_motor_left_pid.target_val = speed_aim_data.left_speed_aim;
		xQueueReceive(g_speed_data_quene,&car_speed,portMAX_DELAY);
//对pid进行控制
		g_motor_right_pid.actual_val = car_speed.right_speed;
		g_motor_left_pid.actual_val = car_speed.left_speed;
		
		pid_data_right = PID_Increment(&g_motor_right_pid);
		pid_data_left = PID_Increment(&g_motor_left_pid);
		if (pid_data_right > 8000)
		{
			pid_data_right = 8000;
            motor(0,&htim1,TIM_CHANNEL_4);
		}
        else if (pid_data_right < -8000)
		{
            pid_data_right = -8000;
			motor(0,&htim1,TIM_CHANNEL_4);
		}
		else 
		{
			motor(pid_data_right,&htim1,TIM_CHANNEL_4);
		}
		
		if (pid_data_left > 8000)
		{
			pid_data_left = 8000;
            motor(0,&htim1,TIM_CHANNEL_1);
		}
        else if (pid_data_left < -8000)
		{
            pid_data_left = -8000;
			motor(0,&htim1,TIM_CHANNEL_1);
		}
		else 
		{
			motor(pid_data_left,&htim1,TIM_CHANNEL_1);
		}
//		printf("car_speed: %d,%d\r\n",pid_data_left,(int16_t)car_speed.left_speed);
	}
}




extern u8g2_t u8g2; 
/*   读取各个传感器   将计算结果发到pid中去*/
void logic_control(void *param)
{
	Speed_AIM_Data_Struct speed_aim_data= {0,0};
	QueueSetMemberHandle_t xQueueHandle;
//	double init_angle = 0;
	double current_angle_error = 0;
	double aim_angle = 0;
	Angle_Data_Struct angle_data;
	static float speed_error = 0;
	static int16_t speed_rate = 0;
	void* usart_point;
	Uart_Car_Data uart_data ;
	while(1)
	{
		/*读取队列集中队列*/
		xQueueHandle = xQueueSelectFromSet(g_xQueueSetInput, portMAX_DELAY);
		if(xQueueHandle)
		{
			if(xQueueHandle == g_angle_data_quene)
			{
				/*接收角度数据并进行处理  结果传入到速度目标值中去*/
				xQueueReceive(g_angle_data_quene,&angle_data,0);
//				if(init_angle == 0)
//				{
//					init_angle = angle_data.car_angle;
//					aim_angle = init_angle;
//				}
				
				if(aim_angle - angle_data.car_angle <1 &&
					aim_angle - angle_data.car_angle >-1)
				{
					/*到达旋转的目标角度*/
					speed_error = 0;
				}else
				{
					current_angle_error = calculateAngleDifference(angle_data.car_angle 
					,aim_angle);
					/*进行角度环pid计算*/
					g_angle_pid.actual_val = current_angle_error;
					speed_error = PID_Increment(&g_angle_pid);
//					speed_error = current_angle_error*0.3;
					if(speed_error>70)
					{
						speed_error = 70;
					}else if(speed_error<-70)
					{
						speed_error = -70;
					}
				}
				
				printf("5883data:%f,%f\r\n",speed_error,current_angle_error);
			}else
			if(xQueueHandle == g_uart_data_quene)
			{
//				xQueueOverwriteFromISR(g_uart_data_quene, &c_main_buffer_data, &xHigherPriorityTaskWoken);

				xQueueReceive(g_uart_data_quene,&usart_point ,0);
				
//				for(uint8_t i = 0;i<10;i++)
//				{
//					printf("%d\r\n",((uint8_t *)usart_point)[i]);
//				}
				uart_data.head_frame = ((uint8_t *)usart_point)[0];
				uart_data.logic_data = ((uint8_t *)usart_point)[2]<<8 |((uint8_t *)usart_point)[1];
				uart_data.x_data = ((uint8_t *)usart_point)[4]<<8 |((uint8_t *)usart_point)[3];
				uart_data.y_data = ((uint8_t *)usart_point)[6]<<8 |((uint8_t *)usart_point)[5];
				uart_data.speed_data = ((uint8_t *)usart_point)[8]<<8 |((uint8_t *)usart_point)[7];
				uart_data.sum_data = ((uint8_t *)usart_point)[8];
				uart_data.end_frame = ((uint8_t *)usart_point)[10];
				double angle_cal = 0;
				angle_cal = atan2((double)uart_data.y_data,(double)uart_data.x_data)*(180/3.14159265);
				aim_angle = angle_cal;
				speed_rate = uart_data.speed_data;
//				printf("%f\r\n",angle_cal);
			}
			
		} 
		
		speed_aim_data.left_speed_aim = speed_rate+speed_error;
		speed_aim_data.right_speed_aim = speed_rate-speed_error;
		xQueueOverwrite(g_speed_aim_quene,&speed_aim_data);
	}
}



void lcd_test_task(void *params)
{
//		TIM1->CCR1 = 1250;
//		TIM1->CCR4 = 1250;
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13|GPIO_PIN_15, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14|GPIO_PIN_12, GPIO_PIN_RESET);
//	uint8_t BUF[6];
	
//	double angle_xz;
//	double angle_yz;
//	printf("helloworld\r\n");
//	VL53L0X_Init();
//	
//	u8g2Init(&u8g2);
//	testDrawFrame(&u8g2);
//	testDrawPixelToFillScreen(&u8g2);
	while(1)
	{
		
//		printf("d=%d\r\n",VL53L0X_GetValue());
//	Multiple_Read_HMC5883(BUF);
//	printf("5883data:%f\r\n",HMC5883_anglexy(BUF));
//	printf("counter :%d\r\n",__HAL_TIM_GetCounter(&htim4));
//	__HAL_TIM_SetCounter(&htim4, 0);
//		
	
//		OLED_Clear();
//		testDrawFrame(&u8g2);
//		testDrawRBox(&u8g2);
//		testDrawMulti(&u8g2);
//		osDelay(100);

	}
	
}
//				if(speed_rate == 0)
//				{
//				//	speed_rate = uart_data.speed_data;
//					speed_rate = 0; 
//				}
//				printf("%d\r\n" ,uart_data.speed_data);
//				printf("\r\n%p\r\n",usart_point);
//				printf("%d,%d,%d,%d,%d \r\n",uart_data.head_frame,
//				uart_data.logic_data,
//				uart_data.x_data,
//				uart_data.y_data,
//				uart_data.speed_data
//				);
//				uart_data.logic_data = usart_point[2]<<8|usart_point[1];
//				uart_data.x_data = usart_point[4]<<8|usart_point[3];
//				uart_data.y_data = usart_point[6]<<8|usart_point[5];
//				uart_data.speed_data = usart_point[8]<<8|usart_point[7];
//				printf("usart:%d,%d,%d,%d\r\n",uart_data.logic_data
//				,uart_data.x_data
//				,uart_data.y_data
//				,uart_data.speed_data
//				);
/* USER CODE END Application */

