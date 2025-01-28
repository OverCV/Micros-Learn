#include "converter.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>  // Agregado para _delay_ms

volatile float dutyCycle = 0;
ADCConverter* adc;

void setupPWM() {
  // Configurar Timer2 para PWM rápido
  DDRH |= (1 << PH4);                     // OC4B como salida
  TCCR4A = (1 << COM4B1) | (1 << WGM40);  // Fast PWM, non-inverting
  TCCR4B = (1 << WGM42) | (1 << CS41);    // Prescaler 8
  OCR4B = 0;                              // Duty cycle inicial 0%
}

int main(void) {
  // Inicializar ADC
  adc = ADCConverter::getInstance();
  adc->init(ADCConverter::AVCC, ADCConverter::DIV_128);

  // Configurar PWM
  setupPWM();

  // Habilitar interrupciones globales
  sei();

  while (1) {
    // Leer potenciómetro (PF0/ADC0)
    uint16_t adcValue = adc->read(0);

    // Calcular duty cycle (0-100%)
    dutyCycle = (adcValue / 1023.0f) * 100.0f;

    // Actualizar PWM
    OCR4B = (uint16_t)((dutyCycle / 100.0f) * 255);

    _delay_ms(10);  // Usando la función correcta de avr/delay.h
  }

  return 0;
}

// Si usas interrupciones ADC
ISR(ADC_vect) {
  ADCConverter::conversionComplete();
}