#ifndef __CONTROL_H
#define __CONTROL_H

#include "global.h"
#include "motor.h"
#include "pid.h"
#include "led.h"
#include "key.h"

void MotorErrorOver(void);
void MotorError(void);
void MotorStop(void);
void MotorRunPID(void);
void MotorStartPID(void);
void MotorStartCheck(void);
void EnDirCheck(void);

#endif

