#ifndef _OS_KERNEL_H
#define _OS_KERNEL_H
#include <stdio.h>
#include <stdint.h>


#define INTCTRL         (*((volatile uint32_t *)0xE000ED04))

void osKernelLaunch(uint32_t quanta);
void osKernelInit(void);
uint8_t osKernelAddThreads(
  void(*task0)(void),
	void(*task1)(void),
	void(*task2)(void));	
void osSemaphoreInit(int32_t *semaphore, int32_t value);
void osSignalSet(int32_t *semaphore);
void osSignalWait(int32_t *semaphore);	
void Task3(void);
#endif
