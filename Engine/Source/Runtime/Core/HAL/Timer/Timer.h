#ifndef CORE_HAL_TIMER_H
#define CORE_HAL_TIMER_H
#include "../../InnerCore.h"

f64 RT_HAL_AbsoluteTime();

void RT_HAL_InitTime();

void RT_HAL_Sleep(u64 ms);

#endif
