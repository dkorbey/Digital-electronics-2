/***********************************************************************
 * 
 * Control LEDs using functions from GPIO and Timer libraries. Do not 
 * use delay library any more.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-2020 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_D1  PB5
#define LED_D2  PB4
#define LED_D3  PB3
#define LED_D4  PB2
#define BTN		PD0

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "gpio.h"           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC

/* Function definitions ----------------------------------------------*/
/**
 * Main function where the program execution begins. Toggle three LEDs
 * on Multi-function shield with internal 8- and 16-bit timer modules.
 */

// Global Variables for leds
uint8_t leds[4] = {LED_D1, LED_D2, LED_D3, LED_D4};
int led_count = 0;								
int back = 0;

int main(void)
{	
		
    /* Configuration of three LEDs */
    GPIO_config_output(&DDRB, leds[0]);
    GPIO_write_low(&PORTB, leds[0]);
	
    GPIO_config_output(&DDRB, leds[1]);
    GPIO_write_high(&PORTB, leds[1]);
	
	GPIO_config_output(&DDRB, leds[2]);
    GPIO_write_high(&PORTB, leds[2]);
	
	GPIO_config_output(&DDRB, leds[3]);
	GPIO_write_high(&PORTB, leds[3]);
	
	
	/*Setting up the push button*/
	GPIO_config_input_pullup(&DDRD, BTN);

    // Enables interrupts by setting the global interrupt mask
    sei();
	
    // Infinite loop
    while (1)
    {
		//Determining if the button is pressed or not and adjusting the speed according to that
        if(GPIO_read(&PIND, BTN)) 
		{
			/* Configuration of 16-bit Timer/Counter1
			 * Set prescaler and enable overflow interrupt */
			 TIM1_overflow_1s();
		     TIM1_overflow_interrupt_enable();
		}
		else
		{
			 /* Configuration of 16-bit Timer/Counter1
			 * Set prescaler and enable overflow interrupt */
			 TIM1_overflow_262ms();
		     TIM1_overflow_interrupt_enable();
		}
    }
			
    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
ISR(TIMER1_OVF_vect)
{	
	//Toggling off the previous led
	GPIO_toggle(&PORTB, leds[led_count]);
	
	//Changing the direction
	if(led_count == 3) {
		back = 1;
	}
	if(led_count == 0) {
		back = 0;
	}
	
	//Adjusting the led_count value
	if(back == 0){
		led_count = led_count + 1;
	}
	else if(back == 1){
		led_count = led_count - 1;
	}
		
	//Toggling on the new led
	GPIO_toggle(&PORTB, leds[led_count]);
}
