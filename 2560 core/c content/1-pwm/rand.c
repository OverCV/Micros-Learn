
#include <avr/io.h>
#include <avr/iom2560.h>
#include <avr/iomxx0_1.h>
#include <avr/iotn10.h>

int main(void) {
  // Configuración del ADC
  ADMUX = (1 << REFS0) | ADC0; // Ref. AVcc y canal ADC0
  ADCSRA = (1 << ADEN) | (1 << ADPS2) |
           (1 << ADPS1); // Habilitar ADC con preescalador de 64

  // Configuración del temporizador 1 para PWM en modo Fast PWM 8-bit
  TCCR1A = (1 << WGM10) | (1 << COM1A1); // Modo PWM rápido, canal OC1A
  TCCR1B = (1 << WGM12) | (1 << CS11);   // Preescalador de 8

  // Configuración de pines
  DDRB |= (1 << PB5); // Configura PB como salida para OC1A (PWM)

  // Inicia una conversión ADC
  ADCSRA |= (1 << ADSC); // Inicia la conversión ADC
  while (ADCSRA & (1 << ADSC))
    ; // Espera a que termine la conversión

  int analogValue = ADC; // Lee el valor del ADC (0-1023)

  // Mapear el valor de ADC al rango PWM (0-255)
  int pwmValue = (analogValue * 255) / 1023;

  OCR1A = pwmValue; // Ajusta el ancho de pulso PWM

  // Bucle principal
  while (1) {
    // Inicia una conversión ADC
    ADCSRA |= (1 << ADSC); // Inicia la conversión ADC
    while (ADCSRA & (1 << ADSC))
      ; // Espera a que termine la conversión

    int analogValue = ADC; // Lee el valor del ADC (0-1023)

    // Mapear el valor de ADC al rango PWM (0-255)
    int pwmValue = (analogValue * 255) / 1023;

    OCR1A = pwmValue; // Ajusta el ancho de pulso PWM
  }

  return 0;
}