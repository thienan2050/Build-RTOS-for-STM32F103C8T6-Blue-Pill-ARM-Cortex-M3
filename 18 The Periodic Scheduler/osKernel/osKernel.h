#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H
#include <stdio.h>
#include <stdint.h>

#define PERIOD		100
#define INTCTRL         (*((volatile uint32_t *)0xE000ED04))

void osKernelLaunch(uint32_t quanta);
void osKernelInit(void);
uint8_t osKernelAddThreads(
  void(*task0)(void),
	void(*task1)(void),
	void(*task2)(void));	
void Task3(void);
#endif
