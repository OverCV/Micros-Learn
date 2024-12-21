// manager.h
#ifndef ADC_MANAGER_H
#define ADC_MANAGER_H

// Incluir Arduino.h sólo para definir los tipos y registros
#include <Arduino.h> 

class ADCManager {
private:
    static const uint8_t MAX_CHANNELS = 8;
    volatile uint16_t _lastReadings[MAX_CHANNELS];
    static ADCManager* instance;

public:
    ADCManager();
    
    // Inicialización
    void init();
    
    // La Configuración
    void setChannel(uint8_t channel);
    void startConversion();
    bool isConversionComplete();
    
    // Lectura
    uint16_t read(uint8_t channel);
    float readVoltage(uint8_t channel);
    
    // Para usar en ISR
    static void conversionComplete();
};

#endif
