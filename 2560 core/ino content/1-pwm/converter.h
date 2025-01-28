// converter.h
#ifndef ADC_CONVERTER_H
#define ADC_CONVERTER_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>  // Agregado para _delay_us

class ADCConverter {
private:
  static ADCConverter* instance;
  static const uint8_t MAX_CHANNELS = 16;
  volatile uint16_t lastReadings[MAX_CHANNELS];

  // Configuración
  uint8_t referenceVoltage;
  bool autoTriggerEnabled;
  uint8_t prescalerValue;
  bool interruptsEnabled;

  ADCConverter();

public:
  enum Reference {
    AREF = 0,
    AVCC = 1,
    INTERNAL_1V1 = 2,
    INTERNAL_2V56 = 3
  };

  enum Prescaler {
    DIV_2 = 1,
    DIV_4 = 2,
    DIV_8 = 3,
    DIV_16 = 4,
    DIV_32 = 5,
    DIV_64 = 6,
    DIV_128 = 7
  };

  static ADCConverter* getInstance();

  void init(Reference ref = AVCC,
            Prescaler prescaler = DIV_128,
            bool useInterrupts = false);

  void setChannel(uint8_t channel);
  void startConversion();
  bool isConversionComplete();
  uint16_t read(uint8_t channel);
  float readVoltage(uint8_t channel);
  void enableAutoTrigger(bool enable);

  static void conversionComplete();

  void setPrescaler(Prescaler prescaler);
  void setReference(Reference ref);
  void enableInterrupts(bool enable);
};

#endif