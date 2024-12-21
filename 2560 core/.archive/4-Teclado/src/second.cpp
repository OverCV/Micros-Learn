#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t rotar = 0x01; // Control de rotación en las filas
uint8_t columnas = 0b0000; // Almacena la columna activa
char teclaPresionada = '\0'; // Tecla seleccionada para mostrar

// Mapeo de caracteres para el display
char teclas[4][4] = {
  {'1', '2', '3', 'a'},
  {'4', '5', '6', 'b'},
  {'7', '8', '9', 'c'},
  {'f', '0', 'e', 'd'}
};

// Valores de los segmentos para mostrar en display de 7 segmentos
uint8_t segmentos[16] = {
  0b0111111, // 0
  0b0000110, // 1
  0b1011011, // 2
  0b1001111, // 3
  0b1100110, // 4
  0b1101101, // 5
  0b1111101, // 6
  0b0000111, // 7
  0b1111111, // 8
  0b1101111, // 9
  0b1110111, // a
  0b1111111, // b
  0b0111001, // c
  0b0111111, // d
  0b1111001, // e
  0b1110001  // f
};

void setup() {
  // Configuración de filas como salida
  DDRB = 0x0F; // PB0-PB3 como salida
  PORTB = 0x0F; // Inicializa filas en HIGH

  // Configuración de columnas como entrada con pull-up
  DDRB &= ~((1 << PB4) | (1 << PB5) | (1 << PB6) | (1 << PB7)); // PB4-PB7 como entrada
  PORTB |= (1 << PB4) | (1 << PB5) | (1 << PB6) | (1 << PB7); // Activa resistencias pull-up

  // Configuración de interrupciones en columnas (PCINT4-7)
  PCICR |= (1 << PCIE0); // Habilita interrupción de cambio de pin para PCINT7:0
  PCMSK0 |= (1 << PCINT4) | (1 << PCINT5) | (1 << PCINT6) | (1 << PCINT7); // Activa PCINT4-7

  // Habilita interrupciones globales
  sei();
}

void loop() {
  // Rotación de la señal en las filas
  PORTB = (PORTB & 0xF0) | rotar; // Activa la fila correspondiente en PB0-PB3
  _delay_ms(5); // Breve retardo para estabilizar la señal

  // Muestra el carácter correspondiente si hay una tecla presionada
  if (teclaPresionada != '\0') {
    mostrarEnDisplay(teclaPresionada); // Mostrar en el display
    teclaPresionada = '\0'; // Resetea para evitar mostrar repetidamente
  }

  // Rotación de la variable `rotar` entre 0x01 y 0x08
  if (rotar == 0x08) {
    rotar = 0x01;
  } else {
    rotar <<= 1;
  }
}

void mostrarEnDisplay(char tecla) {
  uint8_t index;
  if (tecla >= '0' && tecla <= '9') {
    index = tecla - '0';
  } else if (tecla >= 'a' && tecla <= 'f') {
    index = 10 + (tecla - 'a');
  } else {
    return; // Si el carácter no es válido, termina la función
  }

  PORTD = segmentos[index]; // Asigna el valor de segmentos al display
}

// Interrupción de cambio de pin para el grupo PCINT7:0
ISR(PCINT0_vect) {
  uint8_t estadoColumnas = PINB & 0xF0; // Leer el estado de las columnas (PB4-PB7)

  // Detectar cuál columna está activa
  switch (estadoColumnas) {
    case 0xE0: columnas = 0; break; // Columna 0 activa
    case 0xD0: columnas = 1; break; // Columna 1 activa
    case 0xB0: columnas = 2; break; // Columna 2 activa
    case 0x70: columnas = 3; break; // Columna 3 activa
    default: return; // Si no hay columna activa, salir
  }

  // Determinar tecla presionada según `rotar` y `columnas`
  for (int fila = 0; fila < 4; fila++) {
    if (rotar == (1 << fila)) { // Detecta la fila activa según `rotar`
      teclaPresionada = teclas[fila][columnas]; // Asigna la tecla correspondiente
      break;
    }
  }
}
