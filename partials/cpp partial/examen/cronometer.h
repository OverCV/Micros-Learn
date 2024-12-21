// cronometer.h
#ifndef CRONOMETER_H
#define CRONOMETER_H

#include <Arduino.h>  // Para tipos/registros

class Cronometer {
private:
    // Variables del tiempo (intenté que fueran realistas pero proteus parece está en otro universo)
    static volatile uint32_t _seconds;
    static volatile uint8_t _halfSeconds;  // Para contar segundos medios
    static bool _isRunning;
    static void (*_timerCallback)();  // Mantenemos el callback por si acaso
    static Cronometer* instance;

public:
    Cronometer();
    
    void init();
    void start();
    void stop();
    void reset();
    uint32_t getSeconds() const;
    uint32_t getTotalHalfSeconds() const;
    void setTimerCallback(void (*callback)());
    static void timerISR();
};

#endif
