#include "rtc.h"
#include "lib.h"
#include "i8259.h"

#define RTC_CONTROL_PORT 0x70
#define RTC_DATA_PORT 0x71
/* The below are used to select control registers with NMI disabled*/
#define RTC_CREG_A 0x8A
#define RTC_CREG_B 0x8B
#define RTC_CREG_C 0x8C
#define ENABLE_PERIODIC_INT 0x40
#define MAINTAIN_SIG_BYTE 0xF0
#define DESIRED_PIE_RATE 0x02
#define RTC_LINE_NO 8

// registers declared with nonmaskable interrupts


/*
# void init_rtc()
# initializes RTC 
# Initialized rtc registers by selecting the periodic interupt enable bit to true and altering the
# Rate which is indicated by the least significant nibble of register A 
# Should be called upon initialization of the kernel
# INPUTS   : none
# OUTPUTS  : none
# RETURN VALUE: none
# REGISTER USAGE: We save all registers and modify control registers A and B of the RTC
*/

void init_rtc(){
	unsigned long flags; 
	cli_and_save(flags); // save flags and halt interupts for critical section
	outb(RTC_CONTROL_PORT, RTC_CREG_B); // selects control register B of the RTC
	io_wait();
	char portdata = inb(RTC_DATA_PORT) | ENABLE_PERIODIC_INT; //get current state and set the disired rate
	io_wait();
	outb(RTC_DATA_PORT, portdata); //update register B with PIE enabled
	io_wait();
	outb(RTC_CONTROL_PORT, RTC_CREG_A); // selects control register A of the RTC
	io_wait();
	portdata = (inb(RTC_DATA_PORT) & MAINTAIN_SIG_BYTE) | DESIRED_PIE_RATE; //Set interrupt handle rate to 2 while maintaining significant bits
	io_wait();
	outb(RTC_DATA_PORT, portdata); //updatew register B with PIE enabled
	io_wait();
	restore_flags(flags);	// restore our flags

	enable_irq(RTC_LINE_NO);
}
