
// adc_converter.cpp
#include "converter.h"

ADCConverter* ADCConverter::instance = nullptr;

ADCConverter::ADCConverter() {
  for (uint8_t i = 0; i < MAX_CHANNELS; i++) {
    lastReadings[i] = 0;
  }
}

ADCConverter* ADCConverter::getInstance() {
  if (instance == nullptr) {
    instance = new ADCConverter();
  }
  return instance;
}

void ADCConverter::init(Reference ref, Prescaler prescaler, bool useInterrupts) {
  // Deshabilitar interrupciones durante configuración
  cli();

  // Configurar referencia
  ADMUX = (ref << REFS0);

  // Configurar ADC
  ADCSRA = (1 << ADEN) |              // Habilitar ADC
           (useInterrupts << ADIE) |  // Habilitar/deshabilitar interrupciones
           (prescaler & 0x07);        // Configurar prescaler

  // Deshabilitar entradas digitales en ADC0-7
  DIDR0 = 0xFF;  // Todos los pines ADC como entrada analógica

// Si tenemos ADC8-15 (ATmega2560)
#ifdef DIDR2
  DIDR2 = 0xFF;
#endif

  // Primera conversión dummy para estabilizar
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC))
    ;

  sei();
}

void ADCConverter::setChannel(uint8_t channel) {
  // Asegurar que estamos en rango válido
  if (channel >= MAX_CHANNELS) return;

// En ATmega2560 necesitamos manejar MUX5
#ifdef MUX5
  if (channel > 7) {
    ADCSRB |= (1 << MUX5);
    channel -= 8;
  } else {
    ADCSRB &= ~(1 << MUX5);
  }
#endif

  // Configurar canal en ADMUX
  ADMUX = (ADMUX & 0xF0) | (channel & 0x07);
}

uint16_t ADCConverter::read(uint8_t channel) {
  cli();

  setChannel(channel);
  _delay_us(10);  // Tiempo de estabilización

  // Iniciar conversión
  ADCSRA |= (1 << ADSC);
  while (ADCSRA & (1 << ADSC))
    ;

  uint16_t result = ADC;

  sei();
  return result;
}

float ADCConverter::readVoltage(uint8_t channel) {
  uint16_t rawValue = read(channel);
  float voltage = 0.0f;

  // Calcular voltaje según referencia
  switch (referenceVoltage) {
    case AVCC:
      voltage = (rawValue * 5.0f) / 1023.0f;
      break;
    case INTERNAL_1V1:
      voltage = (rawValue * 1.1f) / 1023.0f;
      break;
    case INTERNAL_2V56:
      voltage = (rawValue * 2.56f) / 1023.0f;
      break;
    default:
      // Para AREF externo, necesitamos que el usuario proporcione el voltaje
      voltage = rawValue;
      break;
  }

  return voltage;
}

void ADCConverter::conversionComplete() {
  if (instance) {
    uint8_t currentChannel = ADMUX & 0x07;
#ifdef MUX5
    if (ADCSRB & (1 << MUX5)) {
      currentChannel += 8;
    }
#endif
    instance->lastReadings[currentChannel] = ADC;
  }
}