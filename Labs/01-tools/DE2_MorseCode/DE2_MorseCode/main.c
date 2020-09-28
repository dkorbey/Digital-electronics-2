/*
 * DE2_MorseCode.c
 *
 * Created: 28.9.2020 15:14:47
 * Author : dkorb
 */ 

/* Defines -----------------------------------------------------------*/
#define LED_GREEN   PB5     // AVR pin where green LED is connected
#define SHORT_DELAY 500      // Delay in ms
#define LONG_DELAY  1000


#ifndef F_CPU
#define F_CPU 16000000      // CPU frequency in Hz required for delay func
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions

/* Variables ---------------------------------------------------------*/

/* Function prototypes -----------------------------------------------*/

void shortDot();
void longDot();

int main(void)
{
	
	// Set pin as output in Data Direction Register
	// DDRB = DDRB or 0010 0000
	DDRB = DDRB | (1<<LED_GREEN);
	
	// Set pin LOW in Data Register (LED off)
	// PORTB = PORTB and 1101 1111
	PORTB = PORTB & ~(1<<LED_GREEN);

    /* Replace with your application code */
    while (1) 
    {
		
		// Start delay ms
		_delay_ms(LONG_DELAY);
		
		//D in morse code
		longDot();
		shortDot();
		shortDot();
		
		//Interval delay
		_delay_ms(SHORT_DELAY);
		
		//E
		shortDot();
		
		//Interval Delay
		_delay_ms(SHORT_DELAY);
		
		//2
		shortDot();
		shortDot();
		longDot();
		longDot();
		longDot();
		
    }
	
	return 0;
}

/* Functions ---------------------------------------------------------*/

void shortDot() {
	
	// Start delay ms
	_delay_ms(SHORT_DELAY);
	
	// Invert LED in Data Register
	// PORTB = PORTB xor 0010 0000
	PORTB = PORTB ^ (1<<LED_GREEN);
	
	_delay_ms(SHORT_DELAY);
	
	PORTB = PORTB ^ (1<<LED_GREEN);
}

void longDot(){
	
	// Start delay ms
	_delay_ms(SHORT_DELAY);
	
	// Invert LED in Data Register
	// PORTB = PORTB xor 0010 0000
	PORTB = PORTB ^ (1<<LED_GREEN);
	
	// Start delay ms
	_delay_ms(LONG_DELAY);
	
	PORTB = PORTB ^ (1<<LED_GREEN);
}