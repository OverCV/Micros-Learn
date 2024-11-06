#include <Arduino.h>

volatile float dutyCicle = 0;

void setup() {

  // PORTB |= (1 << PB4); // Set pin 10 as output
  // PORTF |= (0 << A0);  // Set pin A0 as input

  pinMode(10, OUTPUT);
  pinMode(A0, INPUT);

  TCCR2A = (1 << COM2A1) | (1 << WGM21) | (1 << WGM20); // Fast PWM
  TCCR2B = (1 << CS22);                                 // Prescaler 64
  OCR2A = 0;                                            // Duty cicle 0%

  sei();
}

void loop() {
  delay(25);
  dutyCicle = analogRead(A0);
  // Serial.println(dutyCicle);
  OCR2A = 255 * dutyCicle / 1023;
}

ISR(TIMER2_OVF_vect) { OCR2A = 255 * dutyCicle / 100; }
