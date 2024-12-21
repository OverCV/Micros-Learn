// manager.cpp
#include "manager.h"

// Inicialización del puntero pues será un Singletón, lo aprendí en Software III.
ADCManager* ADCManager::instance = nullptr;

// El constructor
ADCManager::ADCManager() {
    if (instance == nullptr) {
        instance = this;
    }
    for(uint8_t i = 0; i < MAX_CHANNELS; i++) {
        _lastReadings[i] = 0;
    }
}

// Inicializar el ADC
void ADCManager::init() {
    cli();  // Deshabilitar las interrupciones globales durante la configuración
    
    // Configurar referencia AVCC y alineación derecha
    ADMUX = (1 << REFS0);  // REFS1:0 = 01 para AVcc
    
    // Deshabilitar entradas digitales en ADC(0->3)
    DIDR0 = (1 << ADC3D) | (1 << ADC2D) | (1 << ADC1D) | (1 << ADC0D);
    
    // Configurar ADC
    ADCSRA = (1 << ADEN) |   // Habilitar ADC
             (1 << ADIE) |   // Habilitar interrupciones ADC
             (1 << ADPS2) |  // Prescaler 128 (16MHz/128 = 125KHz)
             (1 << ADPS1) | 
             (1 << ADPS0);
             
    // Primera conversión (con cualquier cosa realmente) para estabilizar el ADC
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
      // Esperar a que termine la conversión
    
    sei();  // Habilitar interrupciones globales
}

// Seleccionar canal ADC
void ADCManager::setChannel(uint8_t channel) {
    // Asegurar que el canal esté en rango del (0 al 7)
    channel &= 0x07;
    
    // Limpiar esos bits de selección de canal y establecer el nuevo canalcito
    ADMUX = (ADMUX & 0xF8) | channel;
}

// Iniciar conversión ADC
void ADCManager::startConversion() {
    ADCSRA |= (1 << ADSC);  // Iniciar la conversión
}

// Verificar si la conversión ha terminado
bool ADCManager::isConversionComplete() {
    return !(ADCSRA & (1 << ADSC));
}

// Leer valor ADC de un canal específico
uint16_t ADCManager::read(uint8_t channel) {
    if(channel >= MAX_CHANNELS) return 0;
    
    cli();  // Deshabilitar interrupciones durante la lectura

    // Seleccionar canal y esperar un momento para estabilización
    setChannel(channel);
    delayMicroseconds(10);  // Tiempo de estabilización (ajustable según necesidad)

    // Iniciar conversión
    startConversion();
    while (!(ADCSRA & (1 << ADIF)));
      // Esperamos que termine la conversión
    ADCSRA |= (1 << ADIF);  // Limpiar esa flag de conversión completa
    
    // Leer resultado
    uint16_t result = ADC;
    
    sei();  // Rehabilitar las interrupciones
    return result;
}

// Leer voltaje de un canal específico y hacer debug
float ADCManager::readVoltage(uint8_t channel) {
    uint16_t raw = read(channel);
    float voltage = (raw * 5.0f) / 1023.0f;
    return voltage;
}

// ISR para la conversión ADC completa
void ADCManager::conversionComplete() {
    if(instance) {
        uint8_t currentChannel = ADMUX & 0x07;
        instance->_lastReadings[currentChannel] = ADC;
    }
}
