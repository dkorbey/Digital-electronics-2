/***********************************************************************
 * 
 * Stopwatch with LCD display output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2017-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function

/* Function definitions ----------------------------------------------*/

/* Variables ---------------------------------------------------------*/
uint8_t customChar[64] = {
	// addr 0: .....
	0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000,
	// addr 1: |....
	0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000,
	// addr 2: ||...
	0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000,
	// addr 3: |||..
	0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100,
	// addr 4: ||||.
	0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110,
	// addr 5: |||||
	0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111,
	// addr 6: Right Arrow
	0b00000, 0b00100, 0b00010, 0b11111, 0b00010, 0b00100, 0b00000, 0b00000,
	// addr 7: Left Arrow
	0b00000, 0b00100, 0b01000, 0b11111, 0b01000, 0b00100, 0b00000, 0b00000
};

/**
 * Main function where the program execution begins. Update stopwatch
 * value on LCD display when 8-bit Timer/Counter2 overflows.
 */
int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
	
	/*Defining and Printing a custom characters*/
	// Set pointer to beginning of CGRAM memory
	lcd_command(1 << LCD_CGRAM);
	for (uint8_t i = 0; i < 64; i++)
	{
		// Store all new chars to memory line by line
		lcd_data(customChar[i]);
	}
	// Set DDRAM address
	lcd_command(1 << LCD_DDRAM);
	
	// Display the arrows on each side of the timer
	lcd_gotoxy(0, 0);
	lcd_putc(6);
	lcd_gotoxy(8, 0);
	lcd_putc(7);
	
    // Put initial string(s) at LCD display
    lcd_gotoxy(1, 0);
    lcd_puts("00");
    lcd_putc(':');
	lcd_puts("00");
	lcd_putc('.');
	lcd_puts("0");
	
	lcd_gotoxy(11, 0);
	lcd_putc('0');
	
	/*lcd_gotoxy(1, 1);
	lcd_putc('b');*/
	
	lcd_gotoxy(11, 1);
	lcd_putc('c');
	
    // Configure 8-bit Timer/Counter2 for Stopwatch
    // Enable interrupt and set the overflow prescaler to 16 ms
	TIM2_overflow_1ms();
	TIM2_overflow_interrupt_enable();
	
	// Configure 8-bit Timer/Counter0 for Progress Bar
	// Enable interrupt and set the overflow prescaler to 16 ms
	TIM0_overflow_16ms();
	TIM0_overflow_interrupt_enable();
	
    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**
 * ISR starts when Timer/Counter2 overflows. Update the stopwatch on
 * LCD display every sixth overflow, ie approximately every 100 ms
 * (6 x 16 ms = 100 ms).
 */
ISR(TIMER2_OVF_vect)
{
	static uint8_t number_of_overflows = 0;
	static uint8_t tens = 0;        // Tenths of a second
	static uint8_t secs = 0;        // Seconds
	static uint8_t mins = 0;        // Minutes
	char lcd_string[2] = "  ";      // String for converting numbers by itoa()
	char lcd_longstring[4];

    number_of_overflows++;
    if (number_of_overflows >= 6)
    {
        // Do this every 6 x 16 ms = 100 ms
        number_of_overflows = 0;
		
		// Update the tenths of a second
		tens++;
		if (tens >= 10)
		{
			tens = 0;

			//Update the seconds
			secs++;
			if(secs >= 60)
			{
				secs = 0;
				
				// Update the minutes
				mins++;
				if(mins >= 60)
					mins = 0;

				// Display minutes
				lcd_gotoxy(1,0);
				if(mins < 10)
					lcd_putc('0');		
				itoa(mins, lcd_string, 10);
				lcd_puts(lcd_string);
				
			}
			
			//Display Seconds
			lcd_gotoxy(4,0);
			if(secs < 10)
				lcd_putc('0');
			itoa(secs, lcd_string, 10);
			lcd_puts(lcd_string);
			
			// Display the square value of the Seconds
			lcd_gotoxy(11, 0);
			itoa((secs * secs), lcd_longstring, 10);
			lcd_puts(lcd_longstring);
			if(secs == 0)
				lcd_puts("   ");
		}
		
		// Display hundredths of seconds
	    lcd_gotoxy(7,0);
	    // Converting cnt0 in decimal to string
	    itoa(tens, lcd_string, 10);
	    lcd_puts(lcd_string);

    }
}

/**
 * ISR starts when Timer/Counter0 overflows. Update the progress bar on
 * LCD display every 16 ms.
 */
ISR(TIMER0_OVF_vect)
{
	static uint8_t symbol = 0;
	static uint8_t position = 0;

	lcd_gotoxy(1 + position, 1);
	lcd_putc(symbol);

	symbol++;
	if(symbol >= 6)
	{
		symbol = 0;
		position++;
		if(position >= 10) {
			position = 0;
			lcd_gotoxy(1,1);
			for(uint8_t i; i < 10; i++) 
			{
				lcd_putc(0);
			}
		}
	}
}