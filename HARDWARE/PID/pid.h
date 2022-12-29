#ifndef	__PID_H
#define	__PID_H

#include "motor.h"
#include "global.h"

void PID_init(void);
void PIDcompute(unsigned int Target,unsigned int Real);
void PIDIcompute(unsigned int Target,unsigned int Real);

#endif

