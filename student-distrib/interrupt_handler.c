#include "interrupt_handler.h"

/*
 * void ignore_int_sub()
 * A subroutine used by our null interrupt handler. 
 * The null interrupt handler will be called when an unfilled IDT location is referenced,
 * whether due to a kernel issue or a hardware issue.  Basically, it should never be called.
 * INPUTS   : none
 * OUTPUTS  : none
 * RETURN VALUE: none
 * SIDE EFFECTS: Displays "Unknown interrupt" system message.
*/
void ignore_int_sub()
{
	printf("Unknown interrupt");
}

/* 18 Exceptions */
void exception_handler_0_sub()
{
	printf("EXCEPTION: DIVIDE BY ZERO ERROR\n");
	while(1);
}	

void exception_handler_2_sub()
{
	printf("EXCEPTION: NMI INTERRUPT\n");
	while(1);
}	

void exception_handler_3_sub()
{
	printf("EXCEPTION: BREAKPOINT\n");
	while(1);
}	

void exception_handler_4_sub()
{
	printf("EXCEPTION: OVERFLOW\n");
	while(1);
}	

void exception_handler_5_sub()
{
	printf("EXCEPTION: BOUND RANGE EXCEEDED ERROR\n");
	while(1);
}	

void exception_handler_6_sub()
{
	printf("EXCEPTION: INVALID OPCODE ERROR\n");
	while(1);
}	

void exception_handler_7_sub()
{
	printf("EXCEPTION: DEVICE NOT AVAILABLE\n");
	while(1);
}	

void exception_handler_8_sub()
{
	printf("EXCEPTION: DOUBLE FAULT\n");
	while(1);
}	

void exception_handler_9_sub()
{
	printf("EXCEPTION: COPROCESSOR SEGMENT OVERRUN\n");
	while(1);
}	

void exception_handler_10_sub()
{
	printf("EXCEPTION: INVALID TSS ERROR\n");
	while(1);
}	

void exception_handler_11_sub()
{
	printf("EXCEPTION: SEGMENT NOT PRESENT ERROR\n");
	while(1);
}	

void exception_handler_12_sub()
{
	printf("EXCEPTION: STACK-SEGMENT FAULT\n");
	while(1);
}	

void exception_handler_13_sub()
{
	printf("EXCEPTION: GENERAL PROTECTION FAULT\n");
	while(1);
}	

void exception_handler_14_sub()
{
	printf("EXCEPTION: PAGE FAULT ERROR\n");
	while(1);
}	

void exception_handler_16_sub()
{
	printf("EXCEPTION: x87 FPU FLOATING-POINT ERROR\n");
	while(1);
}	

void exception_handler_17_sub()
{
	printf("EXCEPTION: ALIGNMENT CHECK\n");
	while(1);
}	

void exception_handler_18_sub()
{
	printf("EXCEPTION: MACHINE CHECK\n");
	while(1);
}	

void exception_handler_19_sub()
{
	printf("EXCEPTION: SIMD FLOATING-POINT EXCEPTION\n");
	while(1);
}	



/* 
 * 15 total interrupts
 *
 * Master Interrupts 
 */
void IRQ0_handler_sub()
{
	printf("IRQ: Timer Chip\n");
}	

void IRQ1_handler_sub()
{
	printf("IRQ: Keyboard\n");
}	

void IRQ3_handler_sub()
{
	printf("IRQ: COM2 Serial Port\n");
}	

void IRQ4_handler_sub()
{
	printf("IRQ: COM1 Serial Port\n");
}	

void IRQ5_handler_sub()
{
	printf("IRQ: LPT2\n");
}	

void IRQ6_handler_sub()
{
	printf("IRQ: Floppy Disk\n");
}	

void IRQ7_handler_sub()
{
	printf("IRQ: LPT1\n");
}	

/* Slave Interrupts */
void IRQ8_handler_sub()
{
	printf("IRQ: Real Time Clock\n");
}	

void IRQ9_handler_sub()
{
	printf("IRQ: Free for peripherals\n");
}	

void IRQ10_handler_sub()
{
	printf("IRQ: Free for peripherals\n");
}	

void IRQ11_handler_sub()
{
	printf("IRQ: Eth0 (network) \n");
}	

void IRQ12_handler_sub()
{
	printf("IRQ: PS/2 Mouse\n");
}	

void IRQ13_handler_sub()
{
	printf("IRQ: FPU/Coprocessor/Inter-processor\n");
}	

void IRQ14_handler_sub()
{
	printf("IRQ: Ide0 (hard drive)\n");
}	

void IRQ15_handler_sub()
{
	printf("IRQ: Ide1 (hard drive)\n");
}	


/* System call */
void system_call_handler_sub()
{
	printf("System Call initiated by User\n");
}	
