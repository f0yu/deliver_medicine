/*
 * Copyright (c) 2016 Zibin Zheng <znbin@qq.com>
 * All rights reserved
 */

#ifndef _MULTI_BUTTON_H_
#define _MULTI_BUTTON_H_
//#include "main.h"
#include "stdint.h"
#include "string.h"
#include "FreeRTOS.h"
#include "stm32f1xx_hal.h"
#include "task.h"
#include "stdio.h"
//According to your need to modify the constants.
#define TICKS_INTERVAL    5	//ms
#define DEBOUNCE_TICKS    3	//MAX 8
#define SHORT_TICKS       (200 /TICKS_INTERVAL)
#define LONG_TICKS        (500 /TICKS_INTERVAL)


typedef void (*BtnCallback)(void*);

typedef enum {
	PRESS_DOWN = 0,
	PRESS_UP,
	PRESS_REPEAT,
	SINGLE_CLICK,
	DOUBLE_CLICK,
	LONG_PRESS_START,
	LONG_PRESS_HOLD,
	number_of_event,
	NONE_PRESS
}PressEvent;
//typedef enum {
//	enter ,
//	down_change,
//	up_change,
//}Key_state;

#define enter 1
#define down_change 2
#define up_change 3

typedef struct Button {
	uint16_t ticks;
	uint8_t  repeat : 4;
	uint8_t  event : 4;
	uint8_t  state : 3;
	uint8_t  debounce_cnt : 3;
	uint8_t  active_level : 1;
	uint8_t  button_level : 1;
	uint8_t  (*hal_button_Level)(void);
	BtnCallback  cb[number_of_event];
	struct Button* next;
}Button;

typedef struct Key_Data {
	uint8_t key_data;
}Key_Data;

#ifdef __cplusplus
extern "C" {
#endif

void button_init(struct Button* handle, uint8_t(*pin_level)(), uint8_t active_level);
void button_attach(struct Button* handle, PressEvent event, BtnCallback cb);
PressEvent get_button_event(struct Button* handle);
int  button_start(struct Button* handle);
void button_stop(struct Button* handle);
void button_ticks(void);
void key_task(void* parameter);
#ifdef __cplusplus
}
#endif

#endif
