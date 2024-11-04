#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/iom2560.h>
#include <avr/iomxx0_1.h>

int main(void) {
  pinMode(13, DD0);
  // Configuración del puerto de salida
  DDRB |= (1 << PB7); // Configura PB7 (OC0A) como salida

  // Configuración del Timer 0 en modo Fast PWM
  TCCR0A |= (1 << WGM00) | (1 << WGM01); // WGM00 y WGM01 para modo Fast PWM
  TCCR0A |=
      (1 << COM0A1); // Configuración de COM0A1 para modo no-invertido en OC0A
  TCCR0B |= (1 << CS01); // Prescaler de 8, para ajustar la frecuencia de PWM

  // Configuración del ciclo de trabajo
  OCR0A = 128; // Valor para un ciclo de trabajo del 50% (0x80)

  while (1) {
    // Bucle principal
  }

  return 0;
}