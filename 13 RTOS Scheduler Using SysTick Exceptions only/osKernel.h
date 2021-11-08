#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H
#include <stdio.h>
#include <stdint.h>
#include "stm32f10x.h"                  // Device header

void osKernelLaunch(uint32_t quanta);
void osKernelInit(void);
uint8_t osKernelAddThreads(
  void(*task0)(void),
	void(*task1)(void),
	void(*task2)(void));	

#endif
