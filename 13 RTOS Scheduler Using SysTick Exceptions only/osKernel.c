#include "osKernel.h"

#define NUM_OF_THREADS 		3
#define STACKSIZE 				100

#define BUS_FREQ 					16000000
uint32_t MILLIS_PRESCALER;

#define SYSPRI3 					(*((volatile uint32_t *)0xE000ED20))
	
void osSchedulerLaunch(void);

struct tcb{
	int32_t *stackPt;
	struct tcb *nextPt;
};

typedef struct tcb tcbType;

tcbType tcbs[NUM_OF_THREADS];		     // 3 TCBs for 3 threads 
tcbType *currentPt;									

int32_t TCB_STACK[NUM_OF_THREADS][STACKSIZE];

void osKernelStackInit(int i)
{
	tcbs[i].stackPt = &TCB_STACK[i][STACKSIZE - 16];			// stackPt is pointing to R0 register, the top of Stack
	TCB_STACK[i][STACKSIZE - 1] = 0x01000000;							// Runs Thumb Mode
}
																																												// Lowest     |  R0			  |		==> tcbs.stackPt is poiting here.
																																												//						|  R1				|
uint8_t osKernelAddThreads(void(*task0)(void), void(*task1)(void), void(*task2)(void))	//						|	 R2			  |
{																																												//						|	 R3			  |
	__disable_irq();																																			//						|	 R4			  |
	tcbs[0].nextPt = &tcbs[1];																														//						|	 R5			  |
	tcbs[1].nextPt = &tcbs[2];																														//						|	 R6			  |
	tcbs[2].nextPt = &tcbs[0];																														//						|	 R7			  |
	osKernelStackInit(0);																																	//						|  R8			  |
	TCB_STACK[0][STACKSIZE - 2] = (int32_t)(task0);																				//						|	 R9			  |
	osKernelStackInit(1);																																	//						|	 R10			|
	TCB_STACK[1][STACKSIZE - 2] = (int32_t)(task1);																				//						|	 R11			|
	osKernelStackInit(2);																																	//						|	 R12 (SP)	|
	TCB_STACK[2][STACKSIZE - 2] = (int32_t)(task2);																				//						|	 R13 (LR) |
	currentPt = &tcbs[0];																																	//						|  R14 (PC) |		==> &task0 / &task1 / & task2
	__enable_irq();																																				//	Highest		|  R15(xPsr)|		==> TCB[99] = 0000 0001 0000 0000  0000 0000 0000 0000 
	return 1;
}

void osKernelInit(void)
{
	__disable_irq();
	MILLIS_PRESCALER = BUS_FREQ / 1000;
}

void osKernelLaunch(uint32_t quanta)
{
	SysTick->CTRL = 0;																// Disable SysTick
	SysTick->VAL = 0;																	// 
	SYSPRI3 =(SYSPRI3&0x00FFFFFF)|0xE0000000; 				// Priority 7
	SysTick->LOAD = (quanta * MILLIS_PRESCALER) - 1;	// Count donw from 0
	SysTick->CTRL = 0x00000007;
	osSchedulerLaunch();															// Use processor clock and enable SysTick interrupt.
}
