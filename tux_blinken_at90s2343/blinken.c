#include <avr/io.h>
/* getestet und funktionabel */
int main(void)
{
        DDRB = 0xFF;
        PORTB = 0x01;
        int i;
        while(1)
        {
                i++;
                if(i%500 == 0)
                        PORTB = ~PORTB & 0x01;
        }
        return 0;
}
