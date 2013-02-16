#include <avr/io.h>
#include <avr/interrupt.h>


#define F_CPU					8000000UL
#include <util/delay.h>

#define fucking				unsigned
#define PORT_MASK 		0x07
#define MAX_INTERVAL	64
#define WAIT_MS				3

/* globals */
volatile fucking char comp_R, comp_G, comp_B;
volatile fucking char timer_overflow_count = 0;
volatile fucking char pin_level = 0x00;

const unsigned char pwmtable[32] = { 0, 0, 1, 1, 2, 3, 3, 4, 5, 6, 6, 7, 8, 9, 10, 11, 12, 13, 15, 16, 18, 19, 21, 23, 25, 28, 30, 34, 38, 43, 51, 64 };

/* ISR */
ISR( TIMER0_OVF_vect )
{

	PORTB = pin_level & PORT_MASK;
	timer_overflow_count = (timer_overflow_count + 1) % MAX_INTERVAL;
	
	if(timer_overflow_count < comp_R) {
		pin_level |= 0x01;
	} else {
		pin_level &= ~0x01;
	}
	
	if(timer_overflow_count < comp_G) {
		pin_level |= 0x02;
	} else {
		pin_level &= ~0x02;
	}

	
	if(timer_overflow_count < comp_B) {
		pin_level |= 0x04;
	} else {
		pin_level &= ~0x04;
	}

}

/* initialisation */
void init( void )
{
	cli();
	
	/* clock prescaler */
	CLKPR = (1 << CLKPCE);
	CLKPR = 0;

	/* enable output ports */
	DDRB = (1 << DDB0) | (1 << DDB1) | (1 << DDB2);
	PORTB = 0x00;

	/* timer init */
	TIFR &= ~(1 << TOV0);
	TIMSK |= (1 << TOIE0);

	TCCR0B = (TCCR0B & 0xF8) | (0x01 & 0x07);

	sei();

	comp_R = 10;
	comp_G = 10;
	comp_B = 10;
}

void set_RGB( fucking char r, fucking char g, fucking char b )
{
	comp_R = pwmtable[ r >> 3 ];
	comp_G = pwmtable[ g >> 3 ];
	comp_B = pwmtable[ b >> 3 ];
}

volatile fucking char x = 37;
volatile fucking char a = 3;
volatile fucking char c = 137;
volatile fucking char m = 255;

fucking char random() {
	
	x = ((x * a) + c) % m;
	return x;

}

void demoLoop( char rand ) {

	fucking char r = 0;
	fucking char g = 0;
	fucking char b = 0;

	fucking char nr = 0;
	fucking char ng = 0;
	fucking char nb = 0;
	
	fucking char i;

	signed char dr = 1;
	signed char dg = 2;
	signed char db = 3;
  char num = 16;
	while(num > 0) {
		num--;	
		if(rand > 0) {
			nr = random();
			ng = random();
			nb = random();
		} else {
			if((num & 8) > 0) {
				nr = 255;
			} else {
				nr = 0;
			}
			if((num & 4) > 0) {
				ng = 255;
			} else {
				ng = 0;
			}
			if((num & 2) > 0) {
				nb = 255;
			} else {
				nb = 0;
			}
			if((num & 1) == 0) {
				nr = 0;
				ng = 0;
				nb = 0;
			}
		}

		i = 0;
		while(i++ < 255) {
			_delay_ms(WAIT_MS);
			set_RGB((r + (nr - r) * i / 255) , (g + (ng - g) * i / 255), (b + (nb - b) * i / 255));
		}
		r = nr;
		g = ng;
		b = nb;
		//set_RGB(0, 62, 0);
	}
}

int main( void ) 
{	
	init();
	while(1) {
		demoLoop(0);
		demoLoop(1);
	}
}
