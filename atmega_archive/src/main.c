#include "fft.h"
#include <avr/io.h>
#include <util/delay.h>

#define BLINK_DELAY_MS 100
#define F_CPU 16000000UL

int main(void) {
  /* set pin 5 of PORTB for output*/
  DDRB |= _BV(DDB5);
  test_arithmetic();

  while (1) {
    /* set pin 5 high to turn led on */
    PORTB |= _BV(PORTB5);
    _delay_ms(BLINK_DELAY_MS);

    if (process_flag) {
	process_flag = 0;
	
        fix32_10 complex_frequency_real[SAMPLE_BUFFER_SIZE];
        fix32_10 complex_frequency_imaginary[SAMPLE_BUFFER_SIZE];
    }
    /* set pin 5 low to turn led off */
    PORTB &= ~_BV(PORTB5);
    _delay_ms(BLINK_DELAY_MS);
  }
}
