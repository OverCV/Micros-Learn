volatile int dutyCicle = 0;

#include <avr/iom2560.h>
#include <avr/interrupt.h>

void setup() {
  pinMode(13, OUTPUT);
  pinMode(13, INPUT);
}

void loop() {
    
  
}

ISR(TIMER0_COMPA_vect) {
  OCR0A = dutyCicle;
}