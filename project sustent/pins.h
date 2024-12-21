// pins.h
#ifndef PINS_H
#define PINS_H

// Hardware de dispensación
const uint8_t RELAY_SODA = 6;
const uint8_t RELAY_WATER = 7;
const uint8_t RELAY_PONY = 8;

// Monedas
const uint8_t LED_COIN_100 = 16;
const uint8_t LED_COIN_200 = 17;
const uint8_t LED_COIN_500 = 18;

const uint8_t BTN_COIN_500 = 21;
const uint8_t BTN_COIN_200 = 20;
const uint8_t BTN_COIN_100 = 19;

// Lcd16x2
const uint8_t RS_PIN = 22;
const uint8_t EN_PIN = 23;
const uint8_t D4_PIN = 24;
const uint8_t D5_PIN = 25;
const uint8_t D6_PIN = 26;
const uint8_t D7_PIN = 27;

// Keypad
const uint8_t ROW_PINS[4] = { 46, 47, 48, 49 };  // Pines para filas
const uint8_t COL_PINS[4] = { 50, 51, 52, 53 };  // Pines para columnas

#endif