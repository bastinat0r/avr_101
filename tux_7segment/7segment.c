#include <avr/io.h>
#include <util/delay.h>

// BCD display mask for hex digits
short mask[16] = {
  0b0111010101, // 0
  0b0011000000, // 1
  0b1101000101, // 2
  0b1111000100, // 3
  0b1011010000, // 4
  0b1110010100, // 5
  0b1110010101, // 6
  0b0111000000, // 7
  0b1111010101, // 8
  0b1111010100, // 9
  0b1111010001, // A
  0b1010010101, // B
  0b0100010101, // C
  0b1011000101, // D
  0b1100010101, // E
  0b1100010001  // F
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
      _delay_us(100);
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
    if (i > 0xf) i = 0;
    display(i);
    _delay_ms(500);
    i++;
  }
        
  return 0;
}
