// cronometer.cpp
#include "cronometer.h"

// Inicialización de las variables estáticas
volatile uint32_t Cronometer::_seconds = 0;
volatile uint8_t Cronometer::_halfSeconds = 0;
bool Cronometer::_isRunning = false;
void (*Cronometer::_timerCallback)() = nullptr;
Cronometer* Cronometer::instance = nullptr;

Cronometer::Cronometer() {
    if (instance == nullptr) {
        instance = this;
    }
}

void Cronometer::init() {
    cli();
    
    // Configuramos el Timer1
    TCCR1A = 0;
    TCCR1B = 0;
    TCNT1 = 0;
    
    // Para 0.5 segundos serían: 16MHz/(1024*2Hz) - 1 = 7811
    OCR1A = 7811;
    TCCR1B |= (1 << WGM12);  // CTC mode
    TCCR1B |= (1 << CS12) | (1 << CS10);  // Prescaler 1024
    
    sei();
}

void Cronometer::start() {
    TIMSK1 |= (1 << OCIE1A);
    _isRunning = true;
}

void Cronometer::stop() {
    TIMSK1 &= ~(1 << OCIE1A);
    _isRunning = false;
}

void Cronometer::reset() {
    cli();
    _seconds = 0;
    _halfSeconds = 0;
    TCNT1 = 0;
    sei();
}

uint32_t Cronometer::getSeconds() const {
    return _seconds;
}

uint32_t Cronometer::getTotalHalfSeconds() const {
    return (_seconds * 2) + _halfSeconds;
}

void Cronometer::setTimerCallback(void (*callback)()) {
    _timerCallback = callback;
}

void Cronometer::timerISR() {
    if(!_isRunning) return;

    _halfSeconds++;
    if(_halfSeconds >= 2) {
        _halfSeconds = 0;
        _seconds++;
    }

    if(_timerCallback) {
        _timerCallback();
    }
}
