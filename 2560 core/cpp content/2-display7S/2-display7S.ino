// main.cpp
#include "dis7.h"
#include "converter.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

// Pines para los segmentos compartidos
const uint8_t PIN_A = 22;  // PORTA0
const uint8_t PIN_B = 23;  // PORTA1
const uint8_t PIN_C = 24;  // PORTA2
const uint8_t PIN_D = 25;  // PORTA3
const uint8_t PIN_E = 26;  // PORTA4
const uint8_t PIN_F = 27;  // PORTA5
const uint8_t PIN_G = 28;  // PORTA6

// Pines de selección para cada display (cátodo común)
const uint8_t DISPLAY_1_SELECT = 36;  // PC0
const uint8_t DISPLAY_2_SELECT = 37;  // PC1

// Variables globales
ADCConverter* adc;
Display7* display;

void setup() {
  // Configurar pines como salidas usando registros directamente
  // PORTA 0-6 como salidas para segmentos
  DDRA |= 0b01111111;

  // PC0 y PC1 como salidas para selección de display
  DDRC |= (1 << PC0) | (1 << PC1);

  // Inicialmente ambos displays apagados (HIGH para cátodo común)
  PORTC |= (1 << PC0) | (1 << PC1);

  // Inicializar ADC
  adc = ADCConverter::getInstance();
  adc->init(ADCConverter::AVCC, ADCConverter::DIV_128);

  // Inicializar display (compartido para ambos dígitos)
  display = new Display7(PIN_A, PIN_B, PIN_C, PIN_D, PIN_E, PIN_F, PIN_G);
  display->init();
}

void showDigit(uint8_t display_select, uint8_t number) {
  // Apagar ambos displays
  PORTC |= (1 << PC0) | (1 << PC1);

  // Mostrar el número
  display->showNumber(number);

  // Activar el display seleccionado (LOW para cátodo común)
  PORTC &= ~(1 << (display_select == 0 ? PC0 : PC1));
}

int main(void) {
  setup();

  while (1) {
    // Leer y convertir voltaje
    float voltage = adc->readVoltage(0);

    // Separar parte entera y decimal
    uint8_t entero = (uint8_t)voltage;
    uint8_t decimal = (uint8_t)((voltage - entero) * 10);

    // Multiplexar displays
    showDigit(0, entero);
    _delay_ms(5);

    showDigit(1, decimal);
    _delay_ms(5);
  }

  return 0;
}

ISR(ADC_vect) {
  ADCConverter::conversionComplete();
}