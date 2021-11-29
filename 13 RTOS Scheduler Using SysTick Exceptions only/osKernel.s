			AREA | .text|, CODE, READONLY, ALIGN = 2	; Code area
			THUMB										; Thumb directive
			EXTERN currentPt
			EXPORT SysTick_Handler
			EXPORT osSchedulerLaunch

SysTick_Handler	; Periodic interrupt, the proceesor will automatically saves 8 Registers to TCB_STACK[x] ( R0, R1, R2, R3, R12, LR, PC, xPSR ), so we have to save the remaining registers onto TCB_STACK[x].
	CPSID		I						; Disable interrupt
	PUSH		{R4 - R11}				; Save R4, R5, R6, R7, R8, R9, R10, R11 onto TCB_STACK[x].
	LDR			R0, =currentPt;			; R0 = currentPt(=&tcb[x])
	LDR			R1, [R0, #0]			; R1 = R0(=currentPt)->stackPt [Number #0 takes us to the first entry of TCB]
	STR			SP, [R1]				; R1(=currentPt->stackPt) = SP
	
	LDR			R1, [R1, #4]			; R1 = currentPt->nextPt [ Number #4 takes us to the second entry of TCB ]
	STR			R1, [R0]				; R0 (=currentPt) = R1(=currentPt->nextPt) ( Next TCB )
	LDR			SP, [R1]				; SP = currentPt->stackPt
	POP			{R4 - R11}				; Restore R4 - R11 from TCB_STACK[x] to Special Registers
	CPSIE		I						; Enable interrupt
	BX 			LR						; Causes a branch to the address contained in LR and exchanges the instruction set.
	
osSchedulerLaunch
	LDR			R0, =currentPt			; R0 = currentPt(&tcb[0]).
	LDR			R2, [R0, #0]			; R2 = R0 = currentPt(&tcb[0]).
	LDR			SP, [R2, #0]			; SP = R2(&tcb[0])->stackPt, now SP points to TCB_STACK[0].
	POP			{R4 - R11}				; Get R4 - R11 from TCB_STACK[0] to Special Registers.
	POP 		{R0 - R3}				; Get R0 - R3  from TCB_STACK[0] to Special Registers.
	POP			{R12}					; Get R12 from TCB_STACK[0] to Special Register.
	ADD			SP, SP, #4				; The next register in TCB_STACK[0] is  SP, we will skip it and move to the next register - LR.
	POP			{LR}					; Load LR from TCB_STACK[0] into Special Register LR. LR contains address of function LED1_Task.
	ADD			SP, SP, #4				; Skip xPSR. Now SP points at the lowest of our TCB_STACK[0].	
	CPSIE		I						; Enable Systick interrupt as we disabled it before. 
	BX			LR						; Causes a branch to the address contained in LR and exchanges the instruction set.
	
	ALIGN
	END