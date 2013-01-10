#include <avr/interrupt.h>
 
volatile unsigned char timer_overflow_count = 0;
volatile unsigned char pulse[16];
volatile char i = 0;
volatile char pause = 0;
signed char diff = 1;

ISR(TIM0_OVF_vect) 
{
	timer_overflow_count++;
	if(timer_overflow_count >= 100) {
		timer_overflow_count = 0;
		if(pause == 0) 
		{
			i += diff;
			if(i >= 15 || i <= 0)
				diff = -1 * diff;
			pause = 1;
		}
		else
		{
			pause--;
		}
	}

	if(timer_overflow_count < pulse[i]) 
	{
		PORTB = 0x01;
	}
	else
	{
		PORTB = 0x00;
	}
}
 
int main(void) 
{
	int i;
	for(i = 0; i<16; i++) 
	{
		/*
		if(i%2 == 0) 
		{
			pulse[i] = 1<<i/2;
		} 
		else
		{
			pulse[i] = 3<<i/2;
		}
		*/
		if(i % 2 == 0) 
		{
			pulse[i] = (2 << i/2) >> 2;
		}
		else
		{
			pulse[i] = (3 << i/2) >> 2;
		}
	}
   // Set up Port B pin 4 mode to output
    DDRB = 0xFF;
 
   // prescale timer to 1/1024th the clock rate
   TCCR0B |= (1<<CS00);
 
   // enable timer overflow interrupt
   TIMSK |=1<<TOIE0;
   sei();
 
   while(1) 
		{
      // let ISR handle the LED forever
   }
}
