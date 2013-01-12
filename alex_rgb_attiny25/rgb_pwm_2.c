#include <stdbool.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*  defines */
#define PORT_MASK       0x07
#define INVERTED_LED    true

/*  constants   */
const unsigned char pwmtable[32] = {
    0, 1, 2, 2, 2, 3, 3, 4, 5, 6, 7, 8, 10, 11, 13, 16, 19, 23, 27, 32,
    38, 45, 54, 64, 76, 91, 108, 128, 152, 181, 215, 255
};

/*  globals */
volatile unsigned char comp_buf_R, comp_buf_G, comp_buf_B;

/*  declaration */
void init( void );

/*  ISR     */
ISR(TIMER0_OVF_vect) {
    static unsigned char    pin_level = ( INVERTED_LED ? 0 : PORT_MASK );
    static unsigned char    led_status = 0;
    static unsigned char    comp_R, comp_G, comp_B;
    static unsigned char    soft_cnt_R = 0xFF;
    static unsigned char    soft_cnt_G = 0xFF - 85;
    static unsigned char    soft_cnt_B = 0xFF - 170;

    /*  set new pin level first for low jitter and do calculation
     *  afterwards  */
    PORTB = pin_level;

    /*  increment counter and if overflow update compare value from
     *  main loop buffer and set LED on */
    if ( ++soft_cnt_R == 0 ) {
        comp_R = comp_buf_R;
        led_status |= 0x01;
    }
    if ( ++soft_cnt_G == 0 ) {
        comp_G = comp_buf_G;
        led_status |= 0x02;
    }
    if ( ++soft_cnt_B == 0 ) {
        comp_B = comp_buf_B;
        led_status |= 0x04;
    }

    /*  on compare match set LED off (written to port on next
     *  interrupt)  */
    if ( comp_R == soft_cnt_R ) {
        led_status &= ~0x01;
    }
    if ( comp_G == soft_cnt_G ) {
        led_status &= ~0x02;
    }
    if ( comp_B == soft_cnt_B ) {
        led_status &= ~0x04;
    }

    pin_level = ( INVERTED_LED ? ~led_status : led_status ) & PORT_MASK;
}

/*  main    */
int main (void) {
    int i;

    init();

    /*  main loop   */
    while ( 1 ) {
        for ( i = 0; i < 32; i++ ) {
            comp_buf_R = pwmtable[i];
            comp_buf_G = pwmtable[i];
            comp_buf_B = pwmtable[i];
            _delay_ms(50);
        }
        _delay_ms(1000);
        for ( i = 31; i >= 0; i-- ) {
            comp_buf_R = pwmtable[i];
            comp_buf_G = pwmtable[i];
            comp_buf_B = pwmtable[i];
            _delay_ms(50);
        }
        _delay_ms(1000);
    }

    return 0;
}

/*  implementation  */
void init( void ) {
    /*  start values    */
    comp_buf_R = 10;
    comp_buf_G = 80;
    comp_buf_B = 170;

    /*  disable interrupts  */
    cli();

    /*  set clock   */
    CLKPR = (1 << CLKPCE);  /*  enable clock prescaler update       */
    CLKPR = 0;              /*  set clock to maximum                */

    /*  set port pins to output and value 0 */
    DDRB = (1 << DDB0) | (1 << DDB1) | (1 << DDB2);
    PORTB = 0;

    /*  timer init  */
    TIFR &= ~(1 << TOV0);   /*  clear timer0 overflow interrupt flag    */
    TIMSK |= (1 << TOIE0);  /*  enable timer0 overflow interrupt        */

    /*  start timer0 by setting last 3 bits in timer0 control register B
     *  to any clock source */
    TCCR0B = (TCCR0B & 0xF8) | (0x01 & 0x07);

    sei();
}
