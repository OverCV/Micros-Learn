#include <Arduino.h>

// Definición de pines
byte pinesFilas[] = {2, 3, 4, 5};
// PORTB0-PORTB3 para interrupciones PCINT
byte pinesColumnas[] = {50, 51, 52, 53};

// Matriz de símbolos del keypad
char teclas[4][4] = {{'1', '2', '3', 'a'},
                     {'4', '5', '6', 'b'},
                     {'7', '8', '9', 'c'},
                     {'f', '0', 'e', 'd'}};

// Pines del display 7 segmentos
byte pinesDisplay[] = {22, 23, 24, 25, 26, 27, 28}; // a-g

// Patrones del display (1 = encendido, 0 = apagado)
const byte patrones[][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 1, 0, 1, 1}, // 9
    {1, 1, 1, 1, 1, 0, 1}, // a
    {0, 0, 1, 1, 1, 1, 1}, // b
    {0, 0, 0, 1, 1, 0, 1}, // c
    {0, 1, 1, 1, 1, 0, 1}, // d
    {1, 1, 0, 1, 1, 1, 1}, // e
    {1, 0, 0, 0, 1, 1, 1}  // f
};

// Variables globales
volatile bool teclaDetectada = false;
volatile byte filaActual = 0;
volatile char teclaPresionada = '\0';

void setup() {
  // Configurar pines de fila como salida
  for (byte i = 0; i < 4; i++) {
    pinMode(pinesFilas[i], OUTPUT);
    digitalWrite(pinesFilas[i], HIGH);
    // Configurar pines de columna como entrada con pull-up
    pinMode(pinesColumnas[i], INPUT_PULLUP);
  }

  // Configurar pines del display como salida
  for (byte i = 0; i < 7; i++) {
    pinMode(pinesDisplay[i], OUTPUT);
  }

  // Habilitar interrupciones PCINT para los pines de columna
  PCICR |= (1 << PCIE0); // Habilitar PCINT0
  PCMSK0 |= (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3);

  sei(); // Habilitar interrupciones globales
}

void loop() {
  // Rotar el cero por las filas
  digitalWrite(pinesFilas[filaActual], HIGH);
  filaActual = (filaActual + 1) % 4;
  digitalWrite(pinesFilas[filaActual], LOW);

  // Si se detectó una tecla, identificarla y mostrarla
  if (teclaDetectada) {
    for (byte columna = 0; columna < 4; columna++) {
      if (digitalRead(pinesColumnas[columna]) == LOW) {
        char tecla = teclas[filaActual][columna];
        // Mostrar en display (solo aquí se hace la impresión)
        mostrarEnDisplay(tecla);
        break;
      }
    }
    teclaDetectada = false;
  }

  delayMicroseconds(500);
}

// ISR minimalista - solo marca el flag
ISR(PCINT0_vect) { teclaDetectada = true; }

void mostrarEnDisplay(char tecla) {
  byte index;

  if (tecla >= '0' && tecla <= '9') {
    index = tecla - '0';
  } else if (tecla >= 'a' && tecla <= 'f') {
    index = 10 + (tecla - 'a');
  } else {
    return;
  }

  for (byte i = 0; i < 7; i++) {
    digitalWrite(pinesDisplay[i], patrones[index][i]);
  }
}