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
#include "timers.h"
#include "queue.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include  <math.h>    //Keil library 
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
void lcd_test(void *params);
void motor_time_isq(void *params);

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim4;



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
//  	xTaskCreate(lcd_test, "lcd_test", 100, NULL, osPriorityNormal, NULL);
	xTaskCreate(pid_control, "pid_control", 100, NULL, osPriorityNormal+1, NULL);
	
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
	car_speed.left_speed =   Get_Speed(&htim2);
	car_speed.right_speed =   Get_Speed(&htim4);
	car_speed.left_angel =   Get_Angle(&htim2);
	car_speed.right_angel =   Get_Angle(&htim4);
	xQueueOverwriteFromISR(g_speed_data_quene,&car_speed,&xHigherPriorityTaskWoken);
	printf("car_speed: %f,%f\r\n",car_speed.right_speed,car_speed.left_speed);
}
void pid_control(void *params)
{
	Speed_Data_Struct car_speed ={0,0,0,0};
	TIM1->CCR1 = 2500;
	TIM1->CCR4 = 2500;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13|GPIO_PIN_15, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14|GPIO_PIN_12, GPIO_PIN_RESET);
	while(1)
	{
		xQueueReceive(g_speed_data_quene,&car_speed,portMAX_DELAY);
//		printf("car_speed: %f\r\n",car_speed.left_speed);
	}
	
}
void lcd_test(void *params)
{
		TIM1->CCR1 = 1250;
		TIM1->CCR4 = 1250;
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13|GPIO_PIN_15, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14|GPIO_PIN_12, GPIO_PIN_RESET);
//	uint8_t BUF[6];
//	double angle_xz;
//	double angle_yz;

	while(1)
	{
//	Multiple_Read_HMC5883(BUF);
//	OLED_PrintSignedVal(0,0,(short)HMC5883_anglexy(BUF));
//	OLED_PrintSignedVal(0,2,(short)(BUF[0] << 8 | BUF[1]));
//	printf("5883data:%f\r\n",HMC5883_anglexy(BUF));
//	printf("counter :%d\r\n",__HAL_TIM_GetCounter(&htim4));
//    __HAL_TIM_SetCounter(&htim4, 0);
//		
//	printf("elloworld\r\n");
//		OLED_Clear();
		
		
		osDelay(10);

	}
	
}
/* USER CODE END Application */

