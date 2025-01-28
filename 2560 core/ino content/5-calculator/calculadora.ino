#include <Arduino.h>

// Definiciones para el LCD16x2
#define RS 7
#define EN 8
#define RW 6
#define D4 9
#define D5 10
#define D6 11
#define D7 12

// Definición de los pines del tecladito
byte pinesFilas[] = { 2, 3, 4, 5 };
byte pinesColumnas[] = { 50, 51, 52, 53 };

// Matriz de símbolos del KeyPad
char teclas[4][4] = {
  { '1', '2', '3', 'a' },  // a = división (/)
  { '4', '5', '6', 'b' },  // b = multiplicación (*)
  { '7', '8', '9', 'c' },  // c = resta (-)
  { 'f', '0', 'e', 'd' }   // d = suma (+)
                           // f = clear, e = igual
};

// Variables de variables globales para la calculadorcita
volatile bool teclaDetectada = false;
volatile byte filaActual = 0;
char operacion = '\0';
long numero1 = 0;
long numero2 = 0;
bool ingresandoNumero2 = false;
bool resultadoMostrado = false;

// Funciones del LCD16x2
void lcdSendNibble(uint8_t nibble) {
  // Establecer datos antes del pulso de enable
  digitalWrite(D4, (nibble >> 0) & 0x01);
  digitalWrite(D5, (nibble >> 1) & 0x01);
  digitalWrite(D6, (nibble >> 2) & 0x01);
  digitalWrite(D7, (nibble >> 3) & 0x01);

  // Pulso de enable con timing correcto
  digitalWrite(EN, HIGH);
  delayMicroseconds(1530);  // Tiempo crítico
  digitalWrite(EN, LOW);
  delayMicroseconds(1530);  // Tiempo entre nibbles
}

void lcdCommand(uint8_t command) {
  digitalWrite(RS, LOW);
  delayMicroseconds(100);

  lcdSendNibble(command >> 4);
  lcdSendNibble(command);

  delay(2);
}

void lcdWriteChar(char c) {
  digitalWrite(RS, HIGH);
  delayMicroseconds(100);

  lcdSendNibble(c >> 4);
  lcdSendNibble(c);

  delay(2);
}

void lcdInit() {
  // Esperar después de encender
  delay(50);

  // Secuencia de inicialización 4 bits
  digitalWrite(D4, 0);
  digitalWrite(D5, 1);
  digitalWrite(D6, 0);
  digitalWrite(D7, 0);

  // Tres pulsos de inicialización
  for (int i = 0; i < 3; i++) {
    digitalWrite(EN, HIGH);
    delayMicroseconds(1530);
    digitalWrite(EN, LOW);
    delay(5);
  }
  // Configurar modo 4 bits
  digitalWrite(D4, 0);
  digitalWrite(D5, 0);
  digitalWrite(D6, 1);
  digitalWrite(D7, 0);
  digitalWrite(EN, HIGH);
  delayMicroseconds(1530);
  digitalWrite(EN, LOW);
  delay(5);

  // Configuración final con delays apropiados
  lcdCommand(0x28);  // 4 bits, 2 líneas
  delay(5);
  lcdCommand(0x0C);  // Display ON
  delay(5);
  lcdCommand(0x06);  // Incremento
  delay(5);
  lcdCommand(0x01);  // Clear
  delay(5);
}

void lcdPrint(const char* str) {
  while (*str) {
    lcdWriteChar(*str++);
  }
}

void lcdClear() {
  lcdCommand(0x01);
  delay(2);
}

void lcdSetCursor(uint8_t col, uint8_t row) {
  uint8_t address = row == 0 ? 0x80 + col : 0xC0 + col;
  lcdCommand(address);
}

// Funciones de la calculadora
void resetCalculadora() {
  numero1 = 0;
  numero2 = 0;
  operacion = '\0';
  ingresandoNumero2 = false;
  resultadoMostrado = false;
  lcdClear();
}

void mostrarResultado(long resultado) {
  lcdClear();
  char buffer[16];
  ltoa(resultado, buffer, 10);
  lcdPrint(buffer);
}

void procesarNumero(char numero) {
  if (resultadoMostrado) {
    resetCalculadora();
  }

  long* numeroActual = ingresandoNumero2 ? &numero2 : &numero1;
  *numeroActual = (*numeroActual * 10) + (numero - '0');
  lcdWriteChar(numero);
}

void procesarOperacion(char op) {
  if (!ingresandoNumero2 && numero1 != 0) {
    operacion = op;
    ingresandoNumero2 = true;

    char simbolo;
    switch (op) {
      case 'd': simbolo = '+'; break;
      case 'c': simbolo = '-'; break;
      case 'b': simbolo = '*'; break;
      case 'a': simbolo = '/'; break;
      default: return;
    }
    lcdWriteChar(simbolo);
  }
}

void calcularResultado() {
  if (operacion != '\0' && !resultadoMostrado) {
    long resultado;

    if (operacion == 'a' && numero2 == 0) {
      lcdClear();
      lcdPrint("Error Div/0");
      delay(1000);
      resetCalculadora();
      return;
    }

    switch (operacion) {
      case 'd':  // Adición
        resultado = numero1 + numero2;
        break;
      case 'c':  // Diferencia
        resultado = numero1 - numero2;
        break;
      case 'b':  // Producto
        resultado = numero1 * numero2;
        break;
      case 'a':  // Razón
        resultado = numero1 / numero2;
        break;
      default:
        return;
    }

    mostrarResultado(resultado);
    numero1 = resultado;
    numero2 = 0;
    operacion = '\0';
    ingresandoNumero2 = false;
    resultadoMostrado = true;
  }
}

void procesarTecla(char tecla) {
  if (tecla >= '0' && tecla <= '9') {
    procesarNumero(tecla);
  } else if (tecla == 'a' || tecla == 'b' || tecla == 'c' || tecla == 'd') {
    procesarOperacion(tecla);
  } else if (tecla == 'e') {
    calcularResultado();
  } else if (tecla == 'f') {
    resetCalculadora();
  }
}

void setup() {
  // Configurar pines LCD
  pinMode(RS, OUTPUT);
  pinMode(EN, OUTPUT);
  pinMode(RW, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);

  // Inicializar el LCD
  lcdInit();
  lcdPrint("Calculadora");
  delay(1000);
  lcdClear();

  // Configurar teclado
  for (byte i = 0; i < 4; i++) {
    pinMode(pinesFilas[i], OUTPUT);
    digitalWrite(pinesFilas[i], HIGH);
    pinMode(pinesColumnas[i], INPUT_PULLUP);
  }

  // Configurar las interrupciones
  PCICR |= (1 << PCIE0);
  PCMSK0 |= (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3);
  sei();
}

void loop() {
  digitalWrite(pinesFilas[filaActual], HIGH);
  filaActual = (filaActual + 1) % 4;
  digitalWrite(pinesFilas[filaActual], LOW);

  if (teclaDetectada) {
    for (byte columna = 0; columna < 4; columna++) {
      if (digitalRead(pinesColumnas[columna]) == LOW) {
        char tecla = teclas[filaActual][columna];
        procesarTecla(tecla);
        delay(200);
        break;
      }
    }
    teclaDetectada = false;
  }

  delayMicroseconds(500);
}

ISR(PCINT0_vect) {
  teclaDetectada = true;
}