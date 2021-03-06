#include <stdio.h>
#include <stdint.h>
#include "stm32f10x.h"         // Device header

#define GPIOC_ADDR  0x40011000 // Base address of GPIO Port C, page 50/1133 
#define RCC_ADDR    0x40021000 // Base address of Reset and clock control RCC, page 49/1133


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



volatile uint32_t tick;
volatile uint32_t _tick;
REGISTER_32 *GPIOC_ODR;
uint32_t LED1_stack[40];
uint32_t LED2_stack[40];

uint32_t *SP_LED1 = &LED1_stack[40];
uint32_t *SP_LED2 = &LED2_stack[40];

void GPIO_Init(void);
void DelayS(uint32_t seconds);
void LED1_ON(void);
void LED1_OFF(void);
void LED2_ON(void);
void LED2_OFF(void);
void LED3_ON(void);
void LED3_OFF(void);
void LED1_main(void);
void LED2_main(void);
void LED3_main(void);
int main(void)
{
	GPIO_Init();
	/* Stack frame for LED1 thread */
	*(--SP_LED1) = (1U << 24); 						/* xPSR */
	*(--SP_LED1) = (uint32_t)&LED1_main;  /* PC */
	*(--SP_LED1) = 0x0000000DU; 					/* LR */
	*(--SP_LED1) = 0x0000000EU; 					/* R12 */
	*(--SP_LED1) = 0x0000000AU; 					/* R3 */
	*(--SP_LED1) = 0x0000000EU; 					/* R2 */
	*(--SP_LED1) = 0x0000000AU; 					/* R1 */
	*(--SP_LED1) = 0x0000000DU; 					/* R0 */
	
	/* Stack frame for LED2 therad */
	*(--SP_LED2) = (1U << 24); 						/* xPSR */
	*(--SP_LED2) = (uint32_t)&LED2_main;  /* PC */
	*(--SP_LED2) = 0x0000000BU; 					/* LR */
	*(--SP_LED2) = 0x0000000EU; 					/* R12 */
	*(--SP_LED2) = 0x0000000EU; 					/* R3 */
	*(--SP_LED2) = 0x0000000EU; 					/* R2 */
	*(--SP_LED2) = 0x0000000EU; 					/* R1 */
	*(--SP_LED2) = 0x0000000FU; 					/* R0 */
	while(1);
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
	
	
	SysTick_Config(SystemCoreClock/100U);
	__enable_irq();
}

void SysTick_Handler(void)
{
	++tick;
}

uint32_t getTick(void)
{
	__disable_irq();
	_tick = tick;
	__enable_irq();
	
	return _tick;
}

void DelayS(uint32_t seconds)
{
	uint32_t temp = getTick();
	seconds *= 100;
	while((getTick() - temp) < seconds);
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


void LED1_main(void)
{
	while(1)
	{
		LED1_ON();
		DelayS(1);
		LED1_OFF();
		DelayS(1);
	}
}

void LED2_main(void)
{
	while(1)
	{
		LED2_ON();
		DelayS(1);
		LED2_OFF();
		DelayS(1);
	}
}


void LED3_main(void)
{
	while(1)
	{
		LED3_ON();
		DelayS(1);
		LED3_OFF();
		DelayS(1);
	}
}


