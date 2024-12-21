// proyect.ino (El main)
#include "machine.h"
#include "product.h"

// #include "MeMCore.h"

// Definición de pines //

// LCD
const uint8_t RS_PIN = 22;
const uint8_t EN_PIN = 23;
const uint8_t D4_PIN = 24;
const uint8_t D5_PIN = 25;
const uint8_t D6_PIN = 26;
const uint8_t D7_PIN = 27;

// Keypad
const uint8_t ROW_PINS[4] = { 46, 47, 48, 49 };  // Pines para filas
const uint8_t COL_PINS[4] = { 50, 51, 52, 53 };  // Pines para columnas
const char KEYS[4][4] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '<', '0', '>', 'D' },
};

// Hardware de dispensación
const uint8_t RELAY_WATER = 7;
const uint8_t RELAY_SODA = 6;

const uint8_t LED_COIN_100 = 16;
const uint8_t LED_COIN_200 = 17;
const uint8_t LED_COIN_500 = 18;

const uint8_t BTN_COIN_500 = 21;
const uint8_t BTN_COIN_200 = 20;
const uint8_t BTN_COIN_100 = 19;

// Interrupciones
volatile bool coin100Inserted = false;
volatile bool coin200Inserted = false;
volatile bool coin500Inserted = false;

Display lcd(RS_PIN, EN_PIN, D4_PIN, D5_PIN, D6_PIN, D7_PIN, false);
Screen screen(&lcd);
Keypad keypad(ROW_PINS, COL_PINS, KEYS);
Menu menu;
Payment payment(coin100Inserted, coin200Inserted, coin500Inserted,
                LED_COIN_100, LED_COIN_200, LED_COIN_500);

Dispenser dispenser(RELAY_WATER, RELAY_SODA);

Machine machine(&screen, &keypad, &menu, &payment, &dispenser);

// ISRs para los botones de monedas
ISR(INT0_vect) {
  coin500Inserted = true;
  Serial.println("INT0");
}

ISR(INT1_vect) {
  coin200Inserted = true;
  Serial.println("INT1");
}

ISR(INT2_vect) {
  coin100Inserted = true;
  Serial.println("INT2");
}

ISR(PCINT0_vect) {
  keypad.setKeyDetected(true);
}

void setup() {
  Serial.begin(9600);
  Serial.println(F("Iniciando sistema..."));

  // Inicializar LCD primero
  lcd.init();

  // Configurar pines de monedas
  pinMode(BTN_COIN_100, INPUT_PULLUP);
  pinMode(BTN_COIN_200, INPUT_PULLUP);
  pinMode(BTN_COIN_500, INPUT_PULLUP);

  // Configurar interrupciones para detección por nivel bajo
  EICRA &= ~((1 << ISC00) | (1 << ISC01)); // INT0 - Nivel bajo
  EICRA &= ~((1 << ISC10) | (1 << ISC11)); // INT1 - Nivel bajo
  EICRA &= ~((1 << ISC20) | (1 << ISC21)); // INT2 - Nivel bajo

  EIMSK |= (1 << INT0) | (1 << INT1) | (1 << INT2); // Habilitar INT0, INT1, INT2

  // Inicializar máquina de estados
  machine.init();

  Serial.println(F("Sistema listo"));
}

void loop() {
  machine.run();
}