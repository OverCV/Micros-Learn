// proyecto.ino
#include "machine.h"
#include "product.h"
#include "pins.h"


// Interrupciones
volatile bool coin100Inserted = false;
volatile bool coin200Inserted = false;
volatile bool coin500Inserted = false;
volatile bool coin1000Inserted = false;

// Keypad
const char KEYS[4][4] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '<', '0', '>', 'D' },
};
// Inicialización de componentes
Display lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN, false);
Screen screen(&lcd);
Keypad keypad(ROW_PINS, COL_PINS, KEYS);
Menu menu;
Payment payment;
// Payment payment(coin100Inserted, coin200Inserted, coin500Inserted,
                // LED_COIN_100, LED_COIN_200, LED_COIN_500);
Dispenser dispenser;  // Ya no necesita argumentos en el constructor

Machine machine(&screen, &keypad, &menu, &payment, &dispenser);

// ISRs para los botones de monedas
ISR(INT0_vect) {
  coin500Inserted = true;
  Serial.println(F("INT0"));
}

ISR(INT1_vect) {
  coin200Inserted = true;
  Serial.println(F("INT1"));
}

ISR(INT2_vect) {
  coin100Inserted = true;
  Serial.println(F("INT2"));
}

ISR(PCINT0_vect) {
  keypad.setKeyDetected(true);
}

void setup() {
  Serial.begin(9600);
  Serial.println(F("Iniciando sistema..."));

  // Configurar pines de monedas
  pinMode(BTN_COIN_100, INPUT_PULLUP);
  pinMode(BTN_COIN_200, INPUT_PULLUP);
  pinMode(BTN_COIN_500, INPUT_PULLUP);

  // Configurar interrupciones para detección por nivel bajo
  EICRA &= ~((1 << ISC00) | (1 << ISC01));  // INT0 - Nivel bajo
  EICRA &= ~((1 << ISC10) | (1 << ISC11));  // INT1 - Nivel bajo
  EICRA &= ~((1 << ISC20) | (1 << ISC21));  // INT2 - Nivel bajo

  EIMSK |= (1 << INT0) | (1 << INT1) | (1 << INT2);  // Habilitar INT0, INT1, INT2

  // Inicializar LCD
  lcd.init();

  // Inicializar máquina de estados - esto inicializará todos los componentes
  machine.init();

  Serial.println(F("Sistema listo"));
}

void loop() {
  machine.run();
}