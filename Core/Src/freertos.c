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
void lcd_test(void *params);
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
	OLED_Init();
	
	OLED_Clear();
//	OLED_Display_On();
//	OLED_Clear();
	Init_HMC5883();

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  	xTaskCreate(lcd_test, "lcd_test", 300, NULL, osPriorityNormal, NULL);
	
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
void lcd_test(void *params)
{
	
//	uint8_t BUF[8];
//	int16_t x,y,z;
//	double angle_xy;
//	double angle_xz;
//	double angle_yz;
	while(1)
	{
		// 在(0, 0)打印'A'
		OLED_PutChar(0, 0, 'A');
		// 在(1, 0)打印'Y'
		OLED_PutChar(1, 0, 'Y');
		// 在第0列第2页打印一个字符串"Hello World!"
		OLED_PrintString(0, 2, "Hello World!");
		
		
		
//		 Multiple_Read_HMC5883(BUF);
//		x=(int16_t)BUF[0] << 8 | BUF[1]; //Combine MSB and LSB of X Data output register  最高有效位
//		z=(int16_t)BUF[2] << 8 | BUF[3]; //Combine MSB and LSB of Z Data output register
//		y=(int16_t)BUF[4] << 8 | BUF[5]; //Combine MSB and LSB of Y Data output register
		
		//angle= atan2((double)y,(double)x) * (180 / 3.14159265) + 180; // angle in degrees 
//		angle*=10;
		//atan2求出的angle取值范围为[-180,180]
//    angle_xy = atan2((double)y,(double)x)*(180/3.14159265)+180;
//   angle_xz = atan2((double)x,(double)z)*(180/3.14159265)+180;
//   angle_yz = atan2((double)y,(double)z)*(180/3.14159265)+180;
//	printf("angle_xy,%f \r\n",angle_xy);
//    
////atan求出的angle取值范围为[-90,90]
//    angle_xy = atan(y,x)*(180/3.14159265)+90;
//    angle_xz = atan(x,z)*(180/3.14159265)+90;
//    angle_yz = atan(y,z)*(180/3.14159265)+90;
    
    
    /* 将芯片焊到板子上时，测量方位时正常方位的反面，因而在原有基础上多加180度旋转，再进行判断 */
//		wan=temp_data/10000+0x30 ;
//		temp_data=temp_data%10000;   //取余运算
//		qian=temp_data/1000+0x30 ;
//		temp_data=temp_data%1000;    //取余运算
//		bai=temp_data/100+0x30   ;
//		temp_data=temp_data%100;     //取余运算
//		shi=temp_data/10+0x30    ;
//		temp_data=temp_data%10;      //取余运算
//		ge=temp_data+0x30; 	
//		
//		OLED_ShowString(5,0,"data_view  <",8);
//		OLED_ShowNumber(5,2,(unsigned int)angle,8,16);
//		 OLED_Num2(0,0,x) ;
//		 OLED_Num2(0,2,(int)angle_xy) ;
//		 OLED_Num2(0,4,angle_xz) ;
//		 OLED_Num2(0,6,angle_yz) ;
//		OLED_ShowNumber(5,4,BUF[1],3,16);
//		OLED_ShowNumber(5,6,BUF[2],3,16);
		osDelay(10);
//OLED_Clear();
//		OLED_ShowString(5,0,"data_view  <",16);
//		OLED_ShowString(5,2,"mode_channel ",16);
//		OLED_ShowString(5,4,"openmv_data ",16);
//		OLED_ShowString(5,6,"Back ",16);
	}
	
}
/* USER CODE END Application */

