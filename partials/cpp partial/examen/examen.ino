// main.ino
#include "display.h"
#include "manager.h"
#include "cronometer.h"

// Constantes
const float KPA_TO_PSI = 0.145;
const uint8_t PIN_BUTTON = 21;  // Usaré la INT0
const uint8_t PIN_RS = 22;
const uint8_t PIN_EN = 23;
const uint8_t PIN_D4 = 24;
const uint8_t PIN_D5 = 25;
const uint8_t PIN_D6 = 26;
const uint8_t PIN_D7 = 27;

// Variables globales
volatile bool showPSI = false;
volatile bool inputN = false;  // Uso de bandera para solicitar el N
uint8_t multiplierN = 1;
const float baseTime = 0.5 * 6;  // b = 0.5 * X, X = 6 porque es mi código del SIA

// Objetización de los componentes
Display lcd(PIN_RS, PIN_EN, PIN_D4, PIN_D5, PIN_D6, PIN_D7);
ADCManager adc;
Cronometer timer;

// Función para obtener la presión promedio
float getPressure() {
  float suma = 0;
  for (uint8_t i = 0; i < 4; i++) {
    float voltaje = adc.readVoltage(i);
    float presion = 11.0 + ((voltaje * 1000.0) / 45.0);
    suma += presion;
  }
  return suma / 4.0;
}

// Callback del botón
void buttonPressed() {
  showPSI = true;
  inputN = true;  // Activar la bandera para solicitar N
  timer.reset();
  timer.start();
  Serial.println(F("Botón presionado. Ingrese N para definir el tiempo de visualización en PSI:"));
}

void setup() {
  // Inicializar Serial
  Serial.begin(9600);
  Serial.println(F("Sistema de medición de presión"));
  Serial.println(F("Al presionar el botón, se le solicitará ingresar N para mostrar PSI durante T segundos."));
  Serial.println(F("Donde T = N * 3 segundos."));

  // Inicializar periféricos
  lcd.init();
  adc.init();
  timer.init();

  // Configurar pines analógicos como entradas (opcional, ya están configurados como entradas por defecto)
  for (uint8_t i = 0; i < 4; i++) {
    pinMode(A0 + i, INPUT);
  }

  // Configurar interrupción del botón
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  EICRA |= (1 << ISC01);  // Flanco de bajada para INT0
  EIMSK |= (1 << INT0);   // Habilitar INT0

  // Inicializar el Timer (ya configurado en Cronometer)
}

void loop() {
  // Lectura de la presión
  float pressure = getPressure();

  // Uso de un buffer para la conversión
  char buffer[16];

  // Mostrar en LCD según el modo
  lcd.clear();
  if (!showPSI) {
    dtostrf(pressure, 5, 1, buffer);
    lcd.print("P: ");
    lcd.print(buffer);
    lcd.print(" kPa");
  } else {
    float pressurePSI = pressure * KPA_TO_PSI;
    dtostrf(pressurePSI, 5, 1, buffer);
    lcd.print("P: ");
    lcd.print(buffer);
    lcd.print(" PSI");

    // Revisar si se acabó el tiempo
    if (timer.getSeconds() >= (baseTime * multiplierN)) {
      showPSI = false;
      timer.stop();
      Serial.println(F("Tiempo de visualización en PSI finalizado."));
    }
  }

  // Manejar entrada serial solo si se solicita
  if (inputN) {
    if (Serial.available()) {
      Serial.print(F("Ingrese N (valor positivo): "));
      int value = Serial.parseInt();
      if (value > 0) {
        multiplierN = value;
        Serial.print(F("N = "));
        Serial.println(multiplierN);
        Serial.print(F("Tiempo de visualización = "));
        Serial.print(multiplierN * baseTime);
        Serial.println(F(" segundos"));
      } else {
        Serial.println(F("Error: N debe ser un valor positivo."));
      }
      inputN = false;  // Desactivo la bandera después de recibir el N
      while (Serial.available())
        Serial.read();  // Limpieza del buffer
    }
  }

  delay(100);  // Pequeño retardo para tener algo de estabilidad, jeje
}

// ISR para el botón
ISR(INT0_vect) {
  buttonPressed();
}

// ISR para Timer1 (mi Cronometer)
ISR(TIMER1_COMPA_vect) {
  Cronometer::timerISR();
}

// ISR para ADC completa
ISR(ADC_vect) {
  ADCManager::conversionComplete();
}
