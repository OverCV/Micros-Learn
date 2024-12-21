// display.cpp
#include "display.h"

Display::Display(uint8_t rs, uint8_t en, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7, bool simulationMode) :
    _rs(rs), _en(en), _d4(d4), _d5(d5), _d6(d6), _d7(d7), _simulationMode(simulationMode) {
}

void Display::init() {
    // Configurar los pines
    pinMode(_rs, OUTPUT);
    pinMode(_en, OUTPUT);
    pinMode(_d4, OUTPUT);
    pinMode(_d5, OUTPUT);
    pinMode(_d6, OUTPUT);
    pinMode(_d7, OUTPUT);

    // Poner los pines en low (la variable puede esté inicializada en otro valor)
    digitalWrite(_rs, LOW);
    digitalWrite(_en, LOW);
    digitalWrite(_d4, LOW);
    digitalWrite(_d5, LOW);
    digitalWrite(_d6, LOW);
    digitalWrite(_d7, LOW);

    // Esperar más de 40ms después de encender (según datasheet enviado)
    delay(50);

    // Secuencia de inicialización especial
    // Primer intento
    sendNibble(0x03);
    delay(5);  // Esperar > 4.1ms
    
    // Segundo intento
    sendNibble(0x03);
    delay(5);  // Esperar > 100us
    
    // Tercer intento
    sendNibble(0x03);
    delay(1);
    
    // Finalmente configurar a 4-bit
    sendNibble(0x02);
    delay(1);

    // Configuración final
    command(LCD_FUNCTIONSET | LCD_4BITMODE | LCD_2LINE | LCD_5x8DOTS); // 4-bit mode, 2 line, 5x8 dots
    command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF); // Display on, cursor off, blink off
    command(LCD_ENTRYMODESET | 0x02); // Increment cursor, no display shift
    clear();
}

void Display::pulseEnable() {
    digitalWrite(_en, LOW);
    if (_simulationMode) {
        delayMicroseconds(1);
    }
    digitalWrite(_en, HIGH);
    if (_simulationMode) {
        delayMicroseconds(1);
    }
    digitalWrite(_en, LOW);
    if (_simulationMode) {
        delayMicroseconds(100); // Extra delay en simulación, en la vida real no problem 
    } else {
        delayMicroseconds(50);
    }
}

void Display::sendNibble(uint8_t nibble) {
    digitalWrite(_d4, nibble & 0x01);
    digitalWrite(_d5, (nibble >> 1) & 0x01);
    digitalWrite(_d6, (nibble >> 2) & 0x01);
    digitalWrite(_d7, (nibble >> 3) & 0x01);
    pulseEnable();
}

void Display::command(uint8_t command) {
    digitalWrite(_rs, LOW);
    sendNibble(command >> 4);
    sendNibble(command & 0x0F);
    waitReady();
}

void Display::writeChar(char c) {
    digitalWrite(_rs, HIGH);
    sendNibble(c >> 4);
    sendNibble(c & 0x0F);
    waitReady();
}

void Display::print(const char* str) {
    while (*str) {
        writeChar(*str++);
    }
}

void Display::print(String str) {
    print(str.c_str());
}

void Display::clear() {
    command(LCD_CLEARDISPLAY);
    if (_simulationMode) {
        delay(2);
    } else {
        delay(2);  // El clear necesita 1.52ms
    }
}

void Display::home() {
    command(LCD_RETURNHOME);
    if (_simulationMode) {
        delay(2);
    } else {
        delay(2);  // El return home necesita 1.52ms
    }
}

void Display::setCursor(uint8_t col, uint8_t row) {
    const uint8_t row_offsets[] = { 0x00, 0x40, 0x14, 0x54 };
    command(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void Display::waitReady() {
    if (_simulationMode) {
        delay(2);
    } else {
        delayMicroseconds(50);
    }
}

void Display::display() {
    command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);
}

void Display::noDisplay() {
    command(LCD_DISPLAYCONTROL | LCD_DISPLAYOFF | LCD_CURSOROFF | LCD_BLINKOFF);
}

void Display::cursor() {
    command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSORON | LCD_BLINKOFF);
}

void Display::noCursor() {
    command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);
}

void Display::blink() {
    command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKON);
}

void Display::noBlink() {
    command(LCD_DISPLAYCONTROL | LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF);
}
