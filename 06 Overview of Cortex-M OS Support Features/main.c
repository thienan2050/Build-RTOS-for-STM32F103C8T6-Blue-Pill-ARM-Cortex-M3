#include <stdio.h>
#include <stdlib.h>

int __svc(0x00) svc_service_add(int x, int y);
int __svc(0x01) svc_service_sub(int x, int y);
int __svc(0x02) svc_service_mul(int x, int y);
int __svc(0x03) svc_service_div(int x, int y);
void SVC_Handler_C(unsigned int *svc_args);
int x, y, z;
int main(void)
{
	x = 1;
	y = 5;
	z = svc_service_add(x, y);
	
	x = 9;
	y = 2;
	z = svc_service_sub(x, y);
	
	x = 3;
	y = 4;
	z = svc_service_mul(x, y);
	
	x = 12;
	y = 6;
	z = svc_service_div(x, y);
} 

__asm void SVC_Handler(void)
{
	TST LR, #4 				// Perform (LR & 0100) as bit 2 tells us the return STACK (MSP or PSP), result is discarded. 
	ITE EQ						// Begin "If Then Else" using "Equal" as the instuction.
	MRSEQ R0, MSP 		// If result is equal to 0, MSP is being used, copy value stored in MSP to R0.
	MRSNE R0, PSP			// If result is not equal to 0, PSP is being used, copy value stored in PSP to R0.
	B		__cpp(SVC_Handler_C)
}

void SVC_Handler_C(unsigned int *svc_args)
{
	unsigned int svc_number;
	svc_number = ((char *)svc_args[6])[-2]; // [6]:R15(PC), [-2]: get the first bite of SVC instruction (SVC number)
	/* First parameter is stored in R0
	   Second parameter is stored R1
		 R0 is also defined as return value.
	*/
	switch(svc_number)
	{
		case 0:
			svc_args[0] = svc_args[0] + svc_args[1];
		break;
		case 1:
			svc_args[0] = svc_args[0] - svc_args[1];
		break;
		case 2:
			svc_args[0] = svc_args[0] * svc_args[1];
		break;
		case 3:
			svc_args[0] = svc_args[0] / svc_args[1];
		break;
		default:
			break;
	}
		
}

