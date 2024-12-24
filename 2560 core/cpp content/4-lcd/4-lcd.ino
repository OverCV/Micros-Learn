// main.cpp
#include "display.h"


// Pines del LCD
const uint8_t LCD_RS = 22;
const uint8_t LCD_EN = 23;
const uint8_t LCD_D4 = 24;
const uint8_t LCD_D5 = 25;
const uint8_t LCD_D6 = 26;
const uint8_t LCD_D7 = 27;

// Definición de caracteres especiales
const uint8_t HEART[8] = {
  0b00000,
  0b01010,
  0b11111,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

const uint8_t BELL[8] = {
  0b00100,
  0b01110,
  0b01110,
  0b01110,
  0b11111,
  0b00000,
  0b00100,
  0b00000
};

const uint8_t ALIEN[8] = {
  0b11111,
  0b10101,
  0b11111,
  0b11111,
  0b01110,
  0b01010,
  0b11011,
  0b00000
};

Display lcd(LCD_RS, LCD_EN, LCD_D4, LCD_D5, LCD_D6, LCD_D7);

// Variables de control para las pruebas
uint8_t currentTest = 0;
unsigned long lastTestTime = 0;
const unsigned long TEST_INTERVAL = 1000;  // segundos por prueba

void setup() {
  lcd.init();

  // Crear caracteres especiales
  lcd.createChar(0, HEART);
  lcd.createChar(1, BELL);
  lcd.createChar(2, ALIEN);

  lcd.print("Iniciando");
  for (int i = 0; i < 3; i++) {
    delay(500);
    lcd.print(".");
  }
  lcd.clear();
}

void loop() {
  if (millis() - lastTestTime >= TEST_INTERVAL) {
    switch (currentTest) {
      case 0:
        testCustomChars();
        break;
      case 1:
        testCustomScroll();
        break;
      case 2:
        testCursorPosition();
        break;
      case 3:
        testBuiltInScroll();
        break;
      case 4:
        testDisplayOnOff();
        break;
      case 5:
        testCounter();
        break;
      case 6:
        testCursorAndBlink();
        break;
      default:
        currentTest = -1;
        break;
    }
    currentTest++;
    lastTestTime = millis();
  }
}

void testCustomChars() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test 0:");
  lcd.setCursor(0, 1);
  lcd.print("Custom Chars ");

  // Mostrar caracteres especiales
  lcd.writeCustomChar(0);  // Corazón
  delay(1000);
  lcd.writeCustomChar(1);  // Campana
  delay(1000);
  lcd.writeCustomChar(2);  // Alien
  delay(1000);

  // Animación simple
  for (int i = 0; i < 3; i++) {
    lcd.setCursor(13, 1);
    lcd.writeCustomChar(i);
    delay(500);
  }
}

void testCursorAndBlink() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test 1:");
  lcd.setCursor(0, 1);
  lcd.print("Cursor & Blink");
  lcd.cursor();
  lcd.blink();
  delay(3000);
  lcd.noCursor();
  lcd.noBlink();
}

void testCursorPosition() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test 2:");
  lcd.setCursor(5, 1);
  lcd.print("Position");
  delay(2000);
}

void testBuiltInScroll() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test 3:");
  lcd.setCursor(0, 1);
  lcd.print("Built-in Scroll");
  delay(1000);

  for (int i = 0; i < 10; i++) {
    lcd.command(0x18);  // Scroll a la izquierda
    delay(300);
  }
}

void testDisplayOnOff() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test 4:");
  lcd.setCursor(0, 1);
  lcd.print("Display On/Off");
  delay(1000);

  lcd.noDisplay();
  delay(1000);
  lcd.display();
  delay(1000);
}

void testCounter() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test 5:");
  lcd.setCursor(0, 1);
  lcd.print("Counter");
  delay(1000);

  for (int i = 0; i <= 5; i++) {
    lcd.setCursor(9, 1);
    lcd.print(String(i));
    delay(500);
  }
}

void testCustomScroll() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Test 6:");
  lcd.setCursor(0, 1);
  lcd.print("Custom Scroll");
  delay(1000);

  // Probar scroll a la izquierda
  lcd.startScroll(" Este es un texto largo que necesita rotar en el LCD", 1, false);
  for (int i = 0; i < 30; i++) {
    lcd.updateScroll();
    delay(300);
  }

  // Probar scroll a la derecha
  lcd.setScrollDirection(true);
  for (int i = 0; i < 30; i++) {
    lcd.updateScroll();
    delay(300);
  }

  lcd.stopScroll();
}