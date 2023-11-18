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
void fire_pid(void *params);
void lcd_test(void *params);
void motor_time_isq(void *params);

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;
//static  char pcWriteBuffer[200];


TimerHandle_t g_motor_timer;
QueueHandle_t g_speed_data_quene;
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
//	for (i = 0; i < 16; i++)
//		printf("-");
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
  
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
 	xTaskCreate(lcd_test, "lcd_test", 50, NULL, osPriorityNormal, NULL);
	xTaskCreate(key_task, "key_task", 100, NULL, osPriorityNormal+1, NULL);
	Menu_Init();
	Menu_Task_Create();
//	xTaskCreate(pid_control, "pid_control", 100, NULL, osPriorityNormal+2, NULL);
//	xTaskCreate(fire_pid, "fire_pid", 100, NULL, osPriorityNormal, NULL);
//	TIM1->CCR1 = 2500;
//	TIM1->CCR4 = 2500;
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13|GPIO_PIN_15, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14|GPIO_PIN_12, GPIO_PIN_RESET);
//	xTimerStart(g_motor_timer,0);
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
	int16_t pid_data_right = 0;
	int16_t pid_data_left = 0;
//	TIM1->CCR1 = 0;
//	TIM1->CCR4 = 0;
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13|GPIO_PIN_15, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14|GPIO_PIN_12, GPIO_PIN_RESET);
	while(1)
	{
		
		xQueueReceive(g_speed_data_quene,&car_speed,portMAX_DELAY);
//对pid进行控制
		g_motor_right_pid.actual_val = car_speed.right_speed;
		g_motor_left_pid.actual_val = car_speed.left_speed;
		pid_data_right = -PID_Increment(&g_motor_right_pid);
		pid_data_left = -PID_Increment(&g_motor_right_pid);
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

uint32_t a = 100;
void fire_pid(void *params)
{
	protocol_init();


	while(1)
	{
		receiving_process();
//set_computer_value(SEND_STOP_CMD,CURVES_CH1,&a,1);
//		a++;
//		set_computer_value(SEND_STOP_CMD, CURVES_CH1, NULL, 0);    // 同步上位机的启动按钮状态
//set_computer_value(SEND_FACT_CMD, CURVES_CH1, &a, 1);     // 给通道 1 发送目标值
		
		osDelay(10);
	}
	
}

extern u8g2_t u8g2; 
void lcd_test(void *params)
{
//		TIM1->CCR1 = 1250;
//		TIM1->CCR4 = 1250;
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13|GPIO_PIN_15, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14|GPIO_PIN_12, GPIO_PIN_RESET);
//	uint8_t BUF[6];
	
//	double angle_xz;
//	double angle_yz;
	printf("helloworld\r\n");
//	
//	u8g2Init(&u8g2);
//	testDrawFrame(&u8g2);
//	testDrawPixelToFillScreen(&u8g2);
	while(1)
	{
//	Multiple_Read_HMC5883(BUF);
//	OLED_PrintSignedVal(0,0,(short)HMC5883_anglexy(BUF));
//	OLED_PrintSignedVal(0,2,(short)(BUF[0] << 8 | BUF[1]));
//	printf("5883data:%f\r\n",HMC5883_anglexy(BUF));
//	printf("counter :%d\r\n",__HAL_TIM_GetCounter(&htim4));
//	__HAL_TIM_SetCounter(&htim4, 0);
//		
	
//		OLED_Clear();
//		testDrawFrame(&u8g2);
//		testDrawRBox(&u8g2);
//		testDrawMulti(&u8g2);
		osDelay(100);

	}
	
}
/* USER CODE END Application */

