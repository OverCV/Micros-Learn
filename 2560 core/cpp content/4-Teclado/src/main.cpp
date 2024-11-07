#include <Arduino.h>
// Pines para filas (salidas)
const int filaPins[4] = {2, 3, 4, 5};

// Pines para columnas (entradas con PCINT)
const int columnaPins[4] = {6, 7, 8, 9}; 

// Mapeo del teclado 4x4 (números y letras)
char keypadMapping[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Pines del display de 7 segmentos
int displayPins[7] = {22, 23, 24, 25, 26, 27, 28};

// Mapeo de segmentos para caracteres
byte displayMapping[16][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 0, 0, 1, 1}, // 9
  {1, 1, 1, 0, 1, 1, 1}, // A
  {0, 0, 1, 1, 1, 1, 1}, // b
  {1, 0, 0, 1, 1, 1, 0}, // C
  {0, 1, 1, 1, 1, 0, 1}, // d
  {1, 0, 0, 1, 1, 1, 1}, // E
  {1, 0, 0, 0, 1, 1, 1}  // F
};

volatile char teclaPresionada = '\0';  // Tecla presionada

void setup() {
  // Configurar pines de filas como salidas
  for (int i = 0; i < 4; i++) {
    pinMode(filaPins[i], OUTPUT);
    digitalWrite(filaPins[i], HIGH); // Inicialmente en HIGH
  }

  // Configurar pines de columnas como entradas con pull-up
  for (int i = 0; i < 4; i++) {
    pinMode(columnaPins[i], INPUT_PULLUP);
  }

  // Configuración de interrupciones PCINT para las columnas
  PCICR |= (1 << PCIE0); // Habilitar interrupciones para PCINT0-7 (ajustar según tu pin)
  PCMSK0 |= (1 << PCINT6) | (1 << PCINT7) | (1 << PCINT8) | (1 << PCINT9); // Ajusta según los pines de columna

  // Configurar pines del display como salidas
  for (int i = 0; i < 7; i++) {
    pinMode(displayPins[i], OUTPUT);
  }

  Serial.begin(9600);
}

void loop() {
  if (teclaPresionada != '\0') {
    mostrarEnDisplay(teclaPresionada);
    Serial.print("Tecla presionada: ");
    Serial.println(teclaPresionada);
    teclaPresionada = '\0';  // Limpiar la tecla presionada para esperar la siguiente
  }
}

// Interrupción por cambio en las columnas
ISR(PCINT0_vect) {
  for (int fila = 0; fila < 4; fila++) {
    digitalWrite(filaPins[fila], LOW); // Enviar un pulso bajo en la fila actual
    delayMicroseconds(10); // Pequeño retardo para permitir el cambio

    for (int col = 0; col < 4; col++) {
      if (digitalRead(columnaPins[col]) == LOW) { // Si hay un cambio en la columna
        teclaPresionada = keypadMapping[fila][col]; // Guardar la tecla presionada
      }
    }

    digitalWrite(filaPins[fila], HIGH); // Regresar la fila a HIGH
  }
}

// Función para mostrar un carácter en el display de 7 segmentos
void mostrarEnDisplay(char caracter) {
  int indice = caracter >= '0' && caracter <= '9' ? caracter - '0' :
               (caracter >= 'A' && caracter <= 'F') ? caracter - 'A' + 10 : -1;

  if (indice >= 0) {
    for (int i = 0; i < 7; i++) {
      digitalWrite(displayPins[i], displayMapping[indice][i]);
    }
  }
}
