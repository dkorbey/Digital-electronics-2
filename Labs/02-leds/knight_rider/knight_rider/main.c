/*
 * knight_rider.c
 *
 * Created: 5.10.2020 09:29:44
 * Author : dkorb
 */ 

/* Defines -----------------------------------------------------------*/
#define LED1 PB1
#define LED2 PB2
#define LED3 PB3
#define LED4 PB4
#define LED5 PB5
#define BLINK_DELAY 500
#define BTN		    PD0
#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions

/* Functions ---------------------------------------------------------*/

void clear();

/**
 * Main function where the program execution begins. Toggle two LEDs 
 * when a push button is pressed.
 */
int main(void)
{
    /* Setting the DDRx as output for LEDs */
    DDRB = DDRB | (1<<LED1);
    DDRB = DDRB | (1<<LED2);
    DDRB = DDRB | (1<<LED3);
    DDRB = DDRB | (1<<LED4);
    DDRB = DDRB | (1<<LED5);
		
	/* PUSH BUTTON*/
	DDRD = DDRD & ~(1<<BTN); //input
	PORTD = PORTD | (1<<BTN); //enable internal pull up
	
    // Infinite loop
    while (1)
    {
       
			clear();
			loop_until_bit_is_clear(PIND, BTN);
			PORTB = PORTB | (1<<LED1);
			loop_until_bit_is_clear(PIND, BTN);
			clear();
			PORTB = PORTB | (1<<LED2);
			loop_until_bit_is_clear(PIND, BTN);
			clear();
			PORTB = PORTB | (1<<LED3);
			loop_until_bit_is_clear(PIND, BTN);
			clear();
			PORTB = PORTB | (1<<LED4);
			loop_until_bit_is_clear(PIND, BTN);
			clear();
			PORTB = PORTB | (1<<LED5);	
			loop_until_bit_is_clear(PIND, BTN);
    }

    // Will never reach this
    return 0;
}

void clear() {
	
	// Pause several milliseconds
	_delay_ms(BLINK_DELAY);
	
	//Making all outputs low for the LEDs
	PORTB = PORTB & ~(1<<LED1);
	PORTB = PORTB & ~(1<<LED2);
	PORTB = PORTB & ~(1<<LED3);
	PORTB = PORTB & ~(1<<LED4);
	PORTB = PORTB & ~(1<<LED5);
	
}