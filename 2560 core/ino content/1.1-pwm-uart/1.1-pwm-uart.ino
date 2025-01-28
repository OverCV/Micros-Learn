#include "converter.h"
#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile float dutyCycle = 0;
ADCConverter* adc;
UART* serial;

void setupPWM() {
  // Configurar Timer4 para PWM rápido
  DDRH |= (1 << PH4);                     // OC4B como salida
  TCCR4A = (1 << COM4B1) | (1 << WGM40);  // Fast PWM, non-inverting
  TCCR4B = (1 << WGM42) | (1 << CS41);    // Prescaler 8
  OCR4B = 0;                              // Duty cycle inicial 0%
}

void printSystemInfo() {
  serial->sendln("=================================");
  serial->sendln("Sistema de Control PWM con ADC");
  serial->sendln("=================================");
  serial->sendln("Configuración:");
  serial->send("- PWM en pin PH4 (OC4B)\r\n");
  serial->send("- ADC en pin PF0 (ADC0)\r\n");
  serial->send("- Frecuencia PWM: ");
  serial->send(F_CPU / (8 * 256));  // Prescaler 8, TOP 255
  serial->sendln(" Hz");
  serial->sendln("=================================");
}

int main(void) {
  // Inicializar UART
  serial = UART::getInstance(UART::UART0);
  serial->init(9600);

  // Imprimir información del sistema
  printSystemInfo();

  // Inicializar ADC
  adc = ADCConverter::getInstance();
  adc->init(ADCConverter::AVCC, ADCConverter::DIV_128);
  serial->sendln("ADC inicializado");

  // Configurar PWM
  setupPWM();
  serial->sendln("PWM configurado");

  // Habilitar interrupciones globales
  sei();

  uint16_t lastAdcValue = 0;
  float lastDutyCycle = 0;

  while (1) {
    // Leer potenciómetro (PF0/ADC0)
    uint16_t adcValue = adc->read(0);

    // Solo actualizar si hay cambio significativo
    if (abs(adcValue - lastAdcValue) > 2) {
      // Calcular duty cycle (0-100%)
      dutyCycle = (adcValue / 1023.0f) * 100.0f;

      // Actualizar PWM
      OCR4B = (uint16_t)((dutyCycle / 100.0f) * 255);

      // Debug por serial
      serial->send("ADC: ");
      serial->send((int)adcValue);
      serial->send(" (");
      serial->send(adc->readVoltage(0));
      serial->send("V) → Duty Cycle: ");
      serial->send(dutyCycle);
      serial->sendln("%");

      lastAdcValue = adcValue;
      lastDutyCycle = dutyCycle;
    }

    _delay_ms(50);  // Actualizar cada 50ms
  }

  return 0;
}

ISR(ADC_vect) {
  ADCConverter::conversionComplete();
}