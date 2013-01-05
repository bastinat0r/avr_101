#include <avr/io.h>

void delay() {
  int i = 250; while (i--);
}

inline void triggerPortB(char mask) {
  PORTB = PORTB | mask;
  PORTB = PORTB & ~mask;
};
  

inline void clockBCD() {
  triggerPortB(0x01);
};

inline void resetBCD() {
  triggerPortB(0x01<<4);
}

int main(void)
{
  /*
   * Pin-Config PortB:
   * 	PB0: Clock BCD-Counter (Out)
   * 	PB3: ADC Poti (In)
   * 	PB4: Reset BCD-Counter (Out)
   */
        DDRB = 0b11011111;
 	
	resetBCD();

	//short mask = 0b1010101110;
	short mask[4] = {0b0111010101,
		         0b0011000000,
			 0b1101000101,
			 0b1111000100
	};
        while(1)
        {
	  resetBCD();

	  short m = mask[3];
	  int i = 11;
	  while (i--) {
	    if (m & 1)
	      delay();
	    m = m >> 1;
	    clockBCD();
	  }

	}
        return 0;
}
