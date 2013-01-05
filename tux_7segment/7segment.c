#include <avr/io.h>

// BCD display mask
short mask[10] = {
  0b0111010101,
  0b0011000000,
  0b1101000101,
  0b1111000100,
  0,
  0,
  0,
  0,
  0,
  0
};

void delay() {
  int i = 200; while (i--);
}

/*
 * Triggersignal auf Port B nach maske
 */
inline void triggerPortB(char mask) {
  PORTB = PORTB | mask;
  PORTB = PORTB & ~mask;
};
  
/*
 * BCD-Zähler weiterstellen
 */
inline void clockBCD() {
  triggerPortB(0x01);
};

/*
 * BCD-Zähler zurücksetzen
 */
inline void resetBCD() {
  triggerPortB(0x01<<4);
}

/*
 * Einen Durchlauf Ziffer idx anzeigen
 */
void display(int idx) {
  resetBCD();

  short m = mask[idx];
  int c = 10;
  while (c--) {
    if (m & 1)
      delay();
    m = m >> 1;
    clockBCD();
  }
}

int main(void)
{
  /*
   * Pin-Config PortB:
   *   PB0: Clock BCD-Counter (Out)
   *   PB3: ADC Poti (In)
   *   PB4: Reset BCD-Counter (Out)
   */
   DDRB = 0b11011111;

   // Test Frame
   int i = 0;
   while(1) {
    i++;
    if (i > 9999) i = 0;
    if (i%1000 == 0) 
     display(i/1000);
  }
        
  return 0;
}
