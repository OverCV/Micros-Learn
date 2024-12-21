// display.h
#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

class Display {
private:
  // Pines
  const uint8_t _rs;
  const uint8_t _en;
  const uint8_t _d4;
  const uint8_t _d5;
  const uint8_t _d6;
  const uint8_t _d7;

  // Configuración
  const bool _simulationMode;

  // Comandos LCD
  static const uint8_t LCD_CLEARDISPLAY = 0x01;
  static const uint8_t LCD_RETURNHOME = 0x02;
  static const uint8_t LCD_ENTRYMODESET = 0x04;
  static const uint8_t LCD_DISPLAYCONTROL = 0x08;
  static const uint8_t LCD_CURSORSHIFT = 0x10;
  static const uint8_t LCD_FUNCTIONSET = 0x20;
  static const uint8_t LCD_SETCGRAMADDR = 0x40;
  static const uint8_t LCD_SETDDRAMADDR = 0x80;

  // Flags para display control
  static const uint8_t LCD_DISPLAYON = 0x04;
  static const uint8_t LCD_DISPLAYOFF = 0x00;
  static const uint8_t LCD_CURSORON = 0x02;
  static const uint8_t LCD_CURSOROFF = 0x00;
  static const uint8_t LCD_BLINKON = 0x01;
  static const uint8_t LCD_BLINKOFF = 0x00;

  // Flags para function set
  static const uint8_t LCD_8BITMODE = 0x10;
  static const uint8_t LCD_4BITMODE = 0x00;
  static const uint8_t LCD_2LINE = 0x08;
  static const uint8_t LCD_1LINE = 0x00;
  static const uint8_t LCD_5x10DOTS = 0x04;
  static const uint8_t LCD_5x8DOTS = 0x00;

  void sendNibble(uint8_t nibble);
  void pulseEnable();
  void waitReady();

  // Prueba de scrolling
  uint32_t _lastScrollTime = 0;  // Última vez que se actualizó el scroll
  uint16_t _scrollIndex = 0;     // Índice actual del scroll
  String _scrollText = "";       // Texto a desplazar
  uint8_t _scrollRow = 0;        // Fila del texto desplazado


public:
  Display(uint8_t rs, uint8_t en, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, bool simulationMode = true);
  void init();
  void command(uint8_t command);
  void writeChar(char c);
  void print(const char* str);
  void print(String str);
  void clear();
  void home();
  void setCursor(uint8_t col, uint8_t row);
  void display();
  void noDisplay();
  void cursor();
  void noCursor();
  void blink();
  void noBlink();

  // void scrollText(String& text, uint8_t row, uint8_t visibleLength, uint16_t updateInterval);
};

#endif