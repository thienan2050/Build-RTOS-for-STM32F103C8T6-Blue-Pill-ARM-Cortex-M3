#include "osKernel.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define GPIOC_ADDR  0x40011000 // Base address of GPIO Port C, page 50/1133 
#define RCC_ADDR    0x40021000 // Base address of Reset and clock control RCC, page 49/1133
#define QUANTA 			10

typedef struct
{
	uint8_t bit0:1;
	uint8_t bit1:1;
	uint8_t bit2:1;
	uint8_t bit3:1;
	uint8_t bit4:1;
	uint8_t bit5:1;
	uint8_t bit6:1;
	uint8_t bit7:1;
	uint8_t bit8:1;
	uint8_t bit9:1;
	uint8_t bit10:1;
	uint8_t bit11:1;
	uint8_t bit12:1;
	uint8_t bit13:1;
	uint8_t bit14:1;
	uint8_t bit15:1;
	uint8_t bit16:1;
	uint8_t bit17:1;
	uint8_t bit18:1;
	uint8_t bit19:1;
	uint8_t bit20:1;
	uint8_t bit21:1;
	uint8_t bit22:1;
	uint8_t bit23:1;
	uint8_t bit24:1;
	uint8_t bit25:1;
	uint8_t bit26:1;
	uint8_t bit27:1;
	uint8_t bit28:1;
	uint8_t bit29:1;
	uint8_t bit30:1;
	uint8_t bit31:1;
}REGISTER_32;

void GPIO_Init(void);
void LED1_ON(void);
void LED1_OFF(void);
void LED2_ON(void);
void LED2_OFF(void);
void LED3_ON(void);
void LED3_OFF(void);
void LED1_main(void);
void LED2_main(void);
void LED3_main(void);
void Delay(void);

REGISTER_32 *GPIOC_ODR;
uint32_t count0, count1, count2;

void LED1_Task(void)
{
	while(1)
	{
		count0++;
		osThreadYield();
	}
}

void LED2_Task(void)
{
	while(1)
	{
		count1++;
	}
}


void LED3_Task(void)
{
	while(1)
	{
		count2++;
	}
}

int main(void)
{
	GPIO_Init();
	osKernelInit();
	osKernelAddThreads(&LED1_Task, &LED2_Task, &LED3_Task);
	osKernelLaunch(QUANTA);
}


void GPIO_Init(void)
{
	REGISTER_32 *GPIOC_CRH, *RCC_APB2ENR;
	
	// APB2 peripheral clock enable register (RCC_APB2ENR), page 111/1133
	RCC_APB2ENR = (REGISTER_32 *)(RCC_ADDR + 0x18);
	// IO port C clock enable
	RCC_APB2ENR->bit4 = 1;

	// Port configuration register high (GPIOC_CRH) (x=A..G), page 171/1133
	GPIOC_CRH = (REGISTER_32 *)(GPIOC_ADDR + 0x04);
	// Set bit 23 22 21 20 : 0 0 1 1 as output C13
	GPIOC_CRH->bit20 = 1;
	GPIOC_CRH->bit21 = 1;
	GPIOC_CRH->bit22 = 0;
	GPIOC_CRH->bit23 = 0;
	// Set bit 27 26 25 24 : 0 0 1 1 as output C14
	GPIOC_CRH->bit24 = 1;
	GPIOC_CRH->bit25 = 1;
	GPIOC_CRH->bit26 = 0;
	GPIOC_CRH->bit27 = 0;
	// Set bit 31 30 29 28 : 0 0 1 1 as output C15
	GPIOC_CRH->bit28 = 1;
	GPIOC_CRH->bit29 = 1;
	GPIOC_CRH->bit30 = 0;
	GPIOC_CRH->bit31 = 0;
}


void LED1_ON(void)
{
	// Port output data register (GPIOC_ODR) (x=A..G), page 172/1133
	GPIOC_ODR = (REGISTER_32 *)(GPIOC_ADDR + 0x0C);
	GPIOC_ODR->bit15 = 1;
}

void LED1_OFF(void)
{
	// Port output data register (GPIOC_ODR) (x=A..G), page 172/1133
	GPIOC_ODR = (REGISTER_32 *)(GPIOC_ADDR + 0x0C);
	GPIOC_ODR->bit15 = 0;
}

void LED2_ON(void)
{
	// Port output data register (GPIOC_ODR) (x=A..G), page 172/1133
	GPIOC_ODR = (REGISTER_32 *)(GPIOC_ADDR + 0x0C);
	GPIOC_ODR->bit13 = 1;
}

void LED2_OFF(void)
{
	// Port output data register (GPIOC_ODR) (x=A..G), page 172/1133
	GPIOC_ODR = (REGISTER_32 *)(GPIOC_ADDR + 0x0C);
	GPIOC_ODR->bit13 = 0;
}

void LED3_ON(void)
{
	// Port output data register (GPIOC_ODR) (x=A..G), page 172/1133
	GPIOC_ODR = (REGISTER_32 *)(GPIOC_ADDR + 0x0C);
	GPIOC_ODR->bit14 = 1;
}

void LED3_OFF(void)
{
	// Port output data register (GPIOC_ODR) (x=A..G), page 172/1133
	GPIOC_ODR = (REGISTER_32 *)(GPIOC_ADDR + 0x0C);
	GPIOC_ODR->bit14 = 0;
}

void Delay(void)
{
	int i;
	for (i = 0; i < 500000; i++);
}






