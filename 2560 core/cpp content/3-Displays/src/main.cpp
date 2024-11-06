#include <Arduino.h>

// Pines para los segmentos compartidos de los dos displays
int displayPins[7] = {22, 23, 24, 25, 26, 27, 28}; // Pines de los segmentos A-G

// Pines para seleccionar cada display
int displaySelect[2] = {36, 37}; // Pines para activar cada display por separado

// Mapeo de segmentos para cada dígito (0-9) en displays de 7 segmentos
byte numeros[10][7] = {
    {1, 1, 1, 1, 1, 1, 0}, // 0
    {0, 1, 1, 0, 0, 0, 0}, // 1
    {1, 1, 0, 1, 1, 0, 1}, // 2
    {1, 1, 1, 1, 0, 0, 1}, // 3
    {0, 1, 1, 0, 0, 1, 1}, // 4
    {1, 0, 1, 1, 0, 1, 1}, // 5
    {1, 0, 1, 1, 1, 1, 1}, // 6
    {1, 1, 1, 0, 0, 0, 0}, // 7
    {1, 1, 1, 1, 1, 1, 1}, // 8
    {1, 1, 1, 0, 0, 1, 1}  // 9
};

void setup() {
  // Configuración de los pines de los segmentos como salidas
  for (int i = 0; i < 7; i++) {
    pinMode(displayPins[i], OUTPUT);
  }

  // Configuración de los pines de selección de displays como salidas
  pinMode(displaySelect[0], OUTPUT); // Control del primer display
  pinMode(displaySelect[1], OUTPUT); // Control del segundo display

  Serial.begin(9600); // Para verificar el voltaje leído
}

// Función para mostrar un dígito en un display específico
void mostrarNumero(int display, int numero) {
  // Desactivar ambos displays antes de cambiar el valor
  digitalWrite(displaySelect[0], LOW);
  digitalWrite(displaySelect[1], LOW);

  // Configurar los segmentos para mostrar el número deseado
  for (int i = 0; i < 7; i++) {
    digitalWrite(displayPins[i], numeros[numero][i]);
  }

  // Activar el display correspondiente
  digitalWrite(displaySelect[display], HIGH);
}

void loop() {
  // Lectura de voltaje del voltímetro (conectado al pin A0)
  int lecturaADC = analogRead(A0);
  float voltaje = lecturaADC * (5.0 / 1023.0);

  // Extraer parte entera y decimal
  int parteEntera = (int)voltaje;
  int parteDecimal = (int)((voltaje - parteEntera) * 10);

  // Multiplexado: alternar entre ambos displays
  mostrarNumero(0, parteEntera); // Mostrar parte entera en el primer display
  delay(1); // Tiempo de retardo para mantener el display visible
  mostrarNumero(1, parteDecimal); // Mostrar parte decimal en el segundo display
  delay(1); // Tiempo de retardo para mantener el display visible
}
