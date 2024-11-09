// Configuración de pines para filas, columnas y display de 7 segmentos
byte pinesFilas[] = {2, 3, 4, 5}; // Pines de filas
byte pinesColumnas[] = {
    6, 7, 8,
    9}; // Pines de columnas en PCINT22 a PCINT25 (Puerto H en ATmega2560)
char teclas[4][4] = {{'1', '2', '3', 'a'},
                     {'4', '5', '6', 'b'},
                     {'7', '8', '9', 'c'},
                     {'f', '0', 'e', 'd'}};

byte pinesDisplay[] = {22, 23, 24, 25,
                       26, 27, 28}; // Pines del display de 7 segmentos

volatile char teclaPresionada = '\0';

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(pinesFilas[i], OUTPUT);
    digitalWrite(pinesFilas[i], HIGH); // Inicializar filas en HIGH
  }

  // Configuración de pines de las columnas como entrada con resistencia pull-up
  for (int i = 0; i < 4; i++) {
    pinMode(pinesColumnas[i], INPUT_PULLUP);
  }

  // Configuración de interrupciones de cambio de pin en PCINT22-PCINT25 (pines
  // PH3 a PH6)
  PCICR |= (1 << PCIE2); // Habilitar interrupciones de cambio de pin para el
                         // grupo PCINT23:16 (Puerto H)
  PCMSK2 |= (1 << PCINT20) | (1 << PCINT21) | (1 << PCINT22) |
            (1 << PCINT23); // Habilitar interrupciones para pines PH3-PH6

  // Configuración de pines del display
  for (int i = 0; i < 7; i++) {
    pinMode(pinesDisplay[i], OUTPUT);
  }

  // Habilitar interrupciones globales
  sei();

  Serial.begin(9600);
}

void loop() {
  // Pooling para detectar tecla presionada en caso de que la interrupción no
  // funcione
  for (int fila = 0; fila < 4; fila++) {
    digitalWrite(pinesFilas[fila], LOW);
    for (int columna = 0; columna < 4; columna++) {
      if (digitalRead(pinesColumnas[columna]) == LOW) {
        teclaPresionada = teclas[fila][columna];
        mostrarEnDisplay(teclaPresionada);
        delay(300);
        while (digitalRead(pinesColumnas[columna]) == LOW)
          ; // Espera a que se libere la tecla
      }
    }
    digitalWrite(pinesFilas[fila], HIGH);
  }
}

// Interrupción de cambio de pin para el grupo PCINT23:16 (Columnas)
ISR(PCINT2_vect) {
  for (int fila = 0; fila < 4; fila++) {
    digitalWrite(pinesFilas[fila], LOW);
    for (int columna = 0; columna < 4; columna++) {
      if (digitalRead(pinesColumnas[columna]) == LOW) {
        teclaPresionada = teclas[fila][columna];
        while (digitalRead(pinesColumnas[columna]) == LOW)
          ; // Espera a que se libere la tecla
      }
    }
    digitalWrite(pinesFilas[fila], HIGH);
  }
}

void mostrarEnDisplay(char tecla) {
  byte segmentos[16][7] = {
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
      {1, 1, 1, 1, 1, 0, 1}, // a (en minúscula)
      {0, 0, 1, 1, 1, 1, 1}, // b (en minúscula)
      {0, 0, 0, 1, 1, 0, 1}, // c (en minúscula)
      {0, 1, 1, 1, 1, 0, 1}, // d (en minúscula)
      {1, 1, 0, 1, 1, 1, 1}, // e (en minúscula)
      {1, 0, 0, 0, 1, 1, 1}  // f (en minúscula)
  };

  int index;

  // Conversión de la tecla a índice numérico
  if (tecla >= '0' && tecla <= '9') {
    index = tecla - '0'; // Índices de 0 a 9
  } else if (tecla >= 'a' && tecla <= 'f') {
    index = 10 + (tecla - 'a'); // Índices de 10 a 15 para 'a' a 'f'
  } else {
    return; // Si el carácter no es soportado, salir de la función
  }

  // Activa los segmentos correspondientes en el display
  for (int i = 0; i < 7; i++) {
    digitalWrite(pinesDisplay[i], segmentos[index][i]);
  }
};
