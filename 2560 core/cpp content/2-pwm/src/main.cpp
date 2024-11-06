#include <Arduino.h>

volatile float dutyCicle = 0;

void setup() {
  pinMode(PB4, OUTPUT);
  pinMode(PF0, INPUT);

  TCCR2A = (1 << COM2A1) | (1 << WGM21) | (1 << WGM20); // Fast PWM
  TCCR2B = (1 << CS22);                                 // Prescaler 64
  OCR2A = 125;                                          // Duty cicle 0%
  sei();
}

void loop() {
  // delay(25);
  dutyCicle = analogRead(PF0) / 10.23;
  OCR2A = 255 * (dutyCicle / 100); // Duty cicle k%
  Serial.println(dutyCicle);
}

ISR(TIMER2_OVF_vect) { OCR2A = 255 * (dutyCicle / 100); }
