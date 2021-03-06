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
  0b1010010110, // b
  0b1000000110, // c
  0b1011000110, // d
  0b1100010110, // E
  0b1100010010  // F
};

// globale Variable: die aktuell anzuzeigende Ziffer
volatile char current_digit = -1;

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
  // keine Anzeige, wenn idx -1 ist
  if (idx == -1)
    return;
  
  // wird nicht benötigt, wenn zu Beginn resetted wird 
  // und die sync nicht verloren geht
  //  resetBCD();

  const int delay = 1000;
    
  // maske für idx auslesen
  short m = mask[idx];
  // die bits einzeln durch-shiften
  int c = 10;
  int d = 0;
  while (c--) {
    // bei gesetztem Bit längere Anzeigedauer
    // -> LED leuchtet
    if (m & 1)
      _delay_us(delay);
    else d++;
    // shift und Zähler auslösen
    m = m >> 1;
    clockBCD();
  }
  
  while (d--)
    _delay_us(delay);
}

void init(void) {
  /*
   * Pin-Config PortB:
   *   PB0: Clock BCD-Counter (Out)
   *   PB1: HEX-Codierer 01 (In)
   *   PB3: HEX-Codierer 02 (In)
   *   PB2: HEX-Codierer 04 (In)
   *   PB4: HEX-Codierer 08 (In)
   */
  DDRB  = 0b11100001;
  // PullUp für Eingänge
  /*
   * Aus bisher nicht geklärten Gründen ist der PullUp an PB4 nicht wirksam und
   * musste in der Schaltung ergänzt werden!
   */
  PORTB = 0b11111111;
   
  // BCD auf Ausgangszustand
  resetBCD();
   
  /*
   * Timer-Code nach http://www.mikrocontroller.net/articles/AVR-GCC-Tutorial/Die_Timer_und_Z%C3%A4hler_des_AVR#8-Bit_Timer.2FCounter
   */
  
  // Timer 0 konfigurieren
  TCCR0 = (1<<CS02)+(0<<CS01)+(0<<CS00); // Prescaler 256
 
  // Overflow Interrupt erlauben
  TIMSK |= (1<<TOIE0); 
  
  // Global Interrupts aktivieren
  sei();
}

/*
 */

int main(void)
{
  // initialisieren
  init();

  while(1) {
   // Pin-Belegung Port B auslesen
   char b = PINB;
   // 4 Pins in Hex-Ziffer umwandeln
   /*
    * Bei günstigerer PIN-Belegung lässt sich das auch als
    * int val = (b>>1)&0xF;
    * darstellen!
    */
   int val = bit_is_set(b, PB1) ? 0 : 1;
   val += bit_is_set(b, PB3) ? 0 : 2;
   val += bit_is_set(b, PB2) ? 0 : 4; 
   val += bit_is_set(b, PB4) ? 0 : 8;
    
   // val als angezeigte Ziffer einstellen
   set_current_digit(val);
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
 
/* 
 * Interrupt-Aktion bei Timer-Überlauf
 */
ISR (TIMER0_OVF_vect)
{
  // Neuer Anzeigezyklus für LCD
  display(current_digit);
  
  // Counter hochsetzen, wir wollen keinen ganzen Zyklus warten
  TCNT0 = 256-4;
}
