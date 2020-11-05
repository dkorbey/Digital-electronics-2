/*
 * snake_application.c
 *
 * Created: 27.10.2020 11:51:05
 * Author : dkorb
 */ 

/* Includes ----------------------------------------------------------*/
#define F_CPU 16000000
#include <util/delay.h>
#include <avr/io.h>         // AVR device-specific IO definitions
#include "segment.h"        // Seven-segment display library for AVR-GCC
// Note: Specially for this application, "input_type" parameter added to the 
//		 "SEG_update_shift_regs" function in segment.h, for setting the
//		  input type for the function.

/* Variables ---------------------------------------------------------*/

int main(void)
{
	// Configure SSD signals
	SEG_init();
	
    /* Replace with your application code */
    while (1) 
    {
		SEG_update_shift_regs(0b11101111, 0b00010000, 0); // Position 0, Led D
		_delay_ms(250);
		SEG_update_shift_regs(0b11011111, 0b00010000, 0); // Position 0, Led C
		_delay_ms(250);
		SEG_update_shift_regs(0b10111111, 0b00010000, 0); // Position 0, Led B
		_delay_ms(250);
		SEG_update_shift_regs(0b01111111, 0b00010000, 0); // Position 0, Led A
		_delay_ms(250);
		SEG_update_shift_regs(0b01111111, 0b00100000, 0); // Position 1, Led A
		_delay_ms(250);
		SEG_update_shift_regs(0b11111011, 0b00100000, 0); // Position 1, Led F
		_delay_ms(250);
		SEG_update_shift_regs(0b11110111, 0b00100000, 0); // Position 1, Led E
		_delay_ms(250);
		SEG_update_shift_regs(0b11101111, 0b00100000, 0); // Position 1, Led D
		_delay_ms(250);
    }
}

