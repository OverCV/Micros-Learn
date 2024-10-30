
/*
 * 1-any.cpp
 * Created: 2024-10-27 00:10:33
 * Author: OverCV
 */

#include <avr/io.h>
#include <avr/iom2560.h>
#include <util/delay.h>


class LED {
public:
  LED() { DDRB |= (1 << PB5); }
  void toggle() { PORTB ^= (1 << PB5); }
};

int main(void) {
  LED led;
  /* Código aquí 🤗 */
  while (1) {
    led.toggle();
    _delay_ms(10000);
  }
  return 0;
}
