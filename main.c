/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <math.h>
#include <stdlib.h>

#define margin 50
#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

uint16_t AnalogVal = 0;
uint8_t isXaxis = 1;
uint16_t Xzero;
uint16_t Yzero;
uint8_t flag;
uint16_t Yval;
uint16_t Xval;

uint16_t Xmin;
uint16_t Xmax;
uint16_t Ymin;
uint16_t Ymax;

int main (void)
{
		// ADC / I/O set up
		
		ADCSRA |= _BV(ACME);
		ADMUX |= _BV(MUX2);	// toggles between ADC4 and ADC5 instead of 0 and 1 now
		ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Set ADC prescaler to 128 - 125KHz sample rate @ 16MH
		DIDR0 = 48;
		
		DDRC = 0x0F; // set PORTC (3-0) to outputs
		ADCSRA |= (1 << ADEN);  // Enable ADC
		ADCSRA &= ~(1 << ADATE); // does not set ADC to Free-Running Mode 
		
		// save the first two values:
		ADMUX ^= (1 << 0);
		ADCSRA |= (1 << ADSC);
		while (ADCSRA & (1<<ADSC));
		Xzero = ADC;
		
		ADMUX ^= (1 << 0);
		ADCSRA |= (1 << ADSC);
		while (ADCSRA & (1<<ADSC));
		Yzero = ADC;
		
		
		// PWM set up
		
		Xmin = Xzero;
		Xmax = Xzero;
		Ymin = Yzero;
		Ymax = Yzero;
		
		DDRD |= (1 << PD5);
		DDRD |= (1 << PD6);

		OCR0A = 128;
		OCR0B = 100;
		TCCR0A |= (1 << COM0A1);
		TCCR0A |= (1 << COM0B1);
		TCCR0A |= (1 << WGM00);
		TCCR0B |= (1 << CS00);
		
		OCR0B = 100;
		
		while(1)
		{
		
			ADMUX ^= (1 << 0);
			ADCSRA |= (1 << ADSC);
			while (ADCSRA & (1<<ADSC));
			ADC = 0;
			ADCSRA |= (1 << ADSC);
			while (ADCSRA & (1<<ADSC));
			isXaxis = CHECK_BIT(ADMUX,0);	
			AnalogVal = ADC;
			ADC = 0;
			//0100 ADC4
			//0101 ADC5 (x axis)
					
			// LED / H-bridge handling
			//tracepoint: Xmin = {Xmin} | Xmax = {Xmax} | Ymin = {Ymin} | Ymax = {Ymax}
			if (isXaxis == 0) // uses OCR0A
			{
				Yval = AnalogVal;
				
				if (Ymax < Yval)
				{
					Ymax = Yval;
				}

				if (Ymin > Yval)
				{
					Ymin = Yval;
				}

				
				if (Yval > (Yzero + margin))
				{	
					PORTC &= ~(1 << PC1);
					PORTC |= (1 << PC0);
					OCR0A = round(((float) (Yval - Yzero) / (Ymax - Yzero))*255.0f);
				}
				if (Yval < (Yzero - margin))
				{
					PORTC &= ~(1 << PC0);
					PORTC |= (1 << PC1);
					OCR0A = round(((float) (Ymin) / (Yval))*255.0f);					
				}
				if ((Yval > (Yzero - margin)) & (Yval < (Yzero + margin)))
				{
					
					PORTC &= ~(1 << PC1);
					PORTC &= ~(1 << PC0);
					OCR0A = 0;
				}
				
			}

			if (isXaxis == 1) // uses OCR0B
			{
				Xval = AnalogVal;

				if (Xmax < Xval)
				{
					Xmax = Xval;
				}

				if (Xmin > Xval)
				{
					Xmin = Xval;
				}

				if (Xval > (Xzero + margin))
				{
					PORTC &= ~(1 << PC2);
					PORTC |= (1 << PC3);
					OCR0B = round(((float) (Xval - Xzero) / (Xmax - Xzero))*255.0f);
					
					

				}
				if (Xval < (Xzero - margin))
				{
					PORTC &= ~(1 << PC3);
					PORTC |= (1 << PC2);
					OCR0B = round(((float) (Xmin) / (Xval))*255.0f);
					
				}
				if ((Xval > (Xzero - margin)) & (Xval < (Xzero + margin)))
				{
					PORTC &= ~(1 << PC2);
					PORTC &= ~(1 << PC3);
					OCR0B = 0;
		
				}
				
			}

		}
		return(0);
}
