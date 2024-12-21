// main.cpp
#include "display.h"

// Pines del LCD
const uint8_t LCD_RS = 22;
const uint8_t LCD_EN = 23;
const uint8_t LCD_D4 = 24;
const uint8_t LCD_D5 = 25;
const uint8_t LCD_D6 = 26;
const uint8_t LCD_D7 = 27;

Display lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

void setup() {
  lcd.init();
  lcd.print("Iniciando...");
  delay(2000);
  lcd.clear();
}

void loop() {
  // Mostrar mensaje con cursor parpadeante
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hola, mundo!");
  lcd.cursor();
  lcd.blink();
  delay(3000);

  // Mover cursor y mostrar mensaje sin cursor
  lcd.noCursor();
  lcd.noBlink();
  lcd.clear();
  lcd.setCursor(5, 1);
  lcd.print("Arduino");
  delay(2000);

  // Desplazar texto a la izquierda
  for (int i = 0; i < 10; i++) {
    lcd.command(0x18);
    delay(300);
  }

  // Apagar y encender display
  lcd.noDisplay();
  delay(1000);
  lcd.display();
  delay(1000);

  // Mostrar caracteres especiales
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Caracteres:");
  lcd.setCursor(0, 1);
  lcd.writeChar(0xDF);  // Grado
  lcd.writeChar(' ');
  lcd.writeChar(0xA5);  // Yen
  lcd.writeChar(' ');
  lcd.writeChar(0xE0);  // è
  delay(3000);

  // Mostrar contador
  lcd.clear();
  for (int i = 0; i <= 10; i++) {
    lcd.setCursor(0, 0);
    lcd.print("Contador:");
    lcd.setCursor(0, 1);
    lcd.print(String(i));  // Convertir el número a una cadena de caracteres
    delay(500);
  }
}