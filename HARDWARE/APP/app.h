#ifndef __APP_H
#define __APP_H

#include "led.h"
#include "key.h"
#include "buzzer.h"
#include "oled_iic.h"

#include "atim.h"
#include "btim.h"
#include "gtim.h"

#include "adc.h"
#include "uart.h"
#include "iwdt.h"

#include "motor.h"
#include "encoder.h"

void APP_config(void);

void TasksHandle_10MS(void);
void TasksHandle_20MS(void);
void TasksHandle_200MS(void);
void TasksHandle_250MS(void);
void TasksHandle_500MS(void);
void State_Machine(void);

#endif


