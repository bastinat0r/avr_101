#include <avr/io.h>
#include <util/delay.h>

#define WAIT_MS 100

int main (void) {
    int t = 0;

    DDRB = (1 << DDB0) | (1 << DDB1) | (1 << DDB2);
    PORTB = 0;

    while ( 1 ) {
        PORTB = (unsigned char) t++ & 0x07;
        _delay_ms( WAIT_MS );
    }

    return 0;
}
