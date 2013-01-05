#include <avr/io.h>

#define WAIT_CYCLES 5000

int main (void) {
    int i, t = 0;

    DDRB = 0xFF;
    PORTB = 0x01;

    while ( 1 ) {
        if ( !(i++ % WAIT_CYCLES) ) {
            PORTB = (unsigned char) t++ & 0x07;
        }
    }

    return 0;
}
