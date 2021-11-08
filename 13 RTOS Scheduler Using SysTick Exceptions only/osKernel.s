			AREA | .text|, CODE, READONLY, ALIGN = 2	; Code area
			THUMB										; Thumb directive
			EXTERN currentPt
			EXPORT SysTick_Handler
			EXPORT osSchedulerLaunch

SysTick_Handler	; Periodic interrupt, the proceesor will automatically saves 8 Registers to Stack Frame ( R0, R1, R2, R3, R12, LR, PC, xPSR), so we have to save the remaining registers.
	CPSID		I						; Disable interrupt
	PUSH		{R4 - R11}				; Save R4, R5, R6, R7, R8, R9, R10, R11
	LDR			R0, =currentPt;			; Load value of currentPt(taskX's address) to R0
	LDR			R1, [R0]				; R1 = R0 =  currentPt
	STR			SP, [R1]				; SP = currentPt
	LDR			R1, [R1, #4]			; R1 = currentPt->Next [ Number #4 takes us to the second entry in TCB ]
	STR			R1, [R0]				; currentPt = R1
	LDR			SP, [R1]				; SP = currentPt->stackPt
	POP			{R4 - R11}
	CPSIE		I						; Enable interrupt
	BX 			LR						; Automatically restore 8 stored registers 
	
osSchedulerLaunch
	LDR			R0, =currentPt
	LDR			R2, [R0]				; R2 = currentPt
	LDR			SP, [R2]				; SP = currentPt->stackPt
	POP			{R4 - R11}
	POP 		{R0 - R3}
	POP			{R12}
	ADD			SP, SP, #4
	POP			{LR}
	ADD			SP, SP, #4
	CPSIE		I
	BX			LR
	
	ALIGN
	END