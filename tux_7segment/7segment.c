/*
 * BCD-Anzeige mit Schieberegister und PWM
 * Autor: Stefan Haun <tux@netz39.de>
 * 
 * Entwickelt für AT90S2343
 */

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

// BCD display mask for hex digits
short mask[16] = {
  0b0111010110, // 0
  0b0011000000, // 1
  0b1101000110, // 2
  0b1111000100, // 3
  0b1011010000, // 4
  0b1110010100, // 5
  0b1110010110, // 6
  0b0111000000, // 7
  0b1111010110, // 8
  0b1111010100, // 9
  0b1111010010, // A
  0b1010010110, // B
  0b0100010110, // C
  0b1011000110, // D
  0b1100010110, // E
  0b1100010010  // F
};

// globale Variable: die aktuell anzuzeigende Ziffer
volatile char current_digit = 0;

inline void set_current_digit(char _digit) {
  current_digit = _digit;
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
      _delay_us(2000);
    m = m >> 1;
    clockBCD();
  }
}

void init(void) {
  /*
   * Pin-Config PortB:
   *   PB0: Clock BCD-Counter (Out)
   *   PB3: ADC Poti (In)
   *   PB4: Reset BCD-Counter (Out)
   */
  DDRB = 0b11011111;
   
  // BCD auf Ausgangszustand
  resetBCD();
   
  /*
   * Timer-Code nach http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Die_Timer_und_Z%C3%A4hler_des_AVR#8-Bit_Timer.2FCounter
   */
  
  // Timer 0 konfigurieren
  TCCR0 = (1<<CS01)+(1<<CS00); // Prescaler 64
 
  // Overflow Interrupt erlauben
  TIMSK |= (1<<TOIE0); 
  // Global Interrupts aktivieren
  sei();
  
  
}
int main(void)
{
  // initialisieren
  init();

  // Test Frame
  int i = 0;
  while(1) {
   if (i > 0xF) i = 0;
   set_current_digit(i);
   _delay_ms(250);
   i++;
  }
        
  return 0;
}

/*
 * Timer-Code nach http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Die_Timer_und_Z%C3%A4hler_des_AVR#8-Bit_Timer.2FCounter
 */

/*
 * Der Overflow Interrupt Handler
 * wird aufgerufen, wenn TCNT0 von
 * 255 auf 0 wechselt (256 Schritte),
 * d.h. ca. alle 2 ms
 */
#ifndef TIMER0_OVF_vect
// Für ältere WinAVR Versionen z.B. WinAVR-20071221 
#define TIMER0_OVF_vect TIMER0_OVF0_vect
#endif
 
/* Interrupt Aktion alle
 * (1000000/8)/256 Hz = 488,28125 Hz
 * bzw.
 * 1/488,28125 s = 2,048 ms  
 */
ISR (TIMER0_OVF_vect)
{
  // Neuer Anzeigezyklus für LCD
  display(current_digit);
}
