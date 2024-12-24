/*
 * LCD_Custom_Character.c
 *
 * Created: 5/23/2020 11:59:10 PM
 * Author : Zaw Myat
 */

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>

void Lcd_Port(char a) {
  if (a & 1)  // 1 = 0001 (check bit 0)
    PORTD |= (1 << PORTD4);
  else
    PORTD &= ~(1 << PORTD4);

  if (a & 2)  // 2 = 0010 (check bit 1)
    PORTD |= (1 << PORTD5);
  else
    PORTD &= ~(1 << PORTD5);

  if (a & 4)  // 4 = 0`00 (check bit 2)
    PORTD |= (1 << PORTD6);
  else
    PORTD &= ~(1 << PORTD6);

  if (a & 8)  // 8 = 1000 (check bit 3)
    PORTD |= (1 << PORTD7);
  else
    PORTD &= ~(1 << PORTD7);
}

void Lcd_Cmd(char a) {
  PORTD &= ~(1 << PORTD0);  // RS = 0
  Lcd_Port(a);
  PORTD |= (1 << PORTD1);  // E = 1
  _delay_ms(1);
  PORTD &= ~(1 << PORTD1);  // E = 0
  _delay_ms(1);
}

void Lcd_Init() {
  Lcd_Port(0x00);
  _delay_ms(20);  // Step (1)
  Lcd_Cmd(0x03);  // Step (2)
  _delay_ms(5);   // Step (3)
  Lcd_Cmd(0x03);  // Step (4)
  _delay_ms(11);  // Step (5)
  Lcd_Cmd(0x03);  // Step (6)
  Lcd_Cmd(0x02);  // Step (7)
  Lcd_Cmd(0x02);  // Step (8)
  Lcd_Cmd(0x08);  // Step (9)
  Lcd_Cmd(0x00);  // Step (10)
  Lcd_Cmd(0x0C);  // Step (11)
  Lcd_Cmd(0x00);  // Step (12)
  Lcd_Cmd(0x06);  // Step (13)
}
void Lcd4_Clear() {
  Lcd_Cmd(0);
  Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b) {
  char temp, z, y;
  if (a == 1) {
    temp = 0x80 + b;
    z = temp >> 4;
    y = (0x80 + b) & 0x0F;
    Lcd_Cmd(z);
    Lcd_Cmd(y);
  } else if (a == 2) {
    temp = 0xC0 + b;
    z = temp >> 4;
    y = (0xC0 + b) & 0x0F;
    Lcd_Cmd(z);
    Lcd_Cmd(y);
  }
}

void Lcd_Write_Char(char a) {
  char temp, y;            // temp = Lower 4 bits of Data, y= Upper 4 bits of data
  temp = a & 0x0F;         // For Lower 4 bits = 0x0F = 0000 1111
  y = a & 0xF0;            // For Upper 4 bits = 0xF0 = 1111 0000
  PORTD |= (1 << PORTD0);  // RS=1
  Lcd_Port(y >> 4);        // Send Upper 4 bits first
  PORTD |= (1 << PORTD1);  // EN =1
  _delay_ms(1);
  PORTD &= ~(1 << PORTD1);  // EN=0
  _delay_ms(1);
  Lcd_Port(temp);          // Then, Send Lower 4 bits
  PORTD |= (1 << PORTD1);  // EN=1;
  _delay_ms(1);
  PORTD &= ~(1 << PORTD1);  // EN=0;
  _delay_ms(1);
}

void Lcd_Write_String(char *a) {
  int i;
  for (i = 0; a[i] != '\0'; i++)
    Lcd_Write_Char(a[i]);
}

void Lcd_Clear() {
  Lcd_Cmd(0);
  Lcd_Cmd(1);
}

void Lcd_Shift_Right() {
  Lcd_Cmd(0x01);
  Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left() {
  Lcd_Cmd(0x01);
  Lcd_Cmd(0x08);
}

const unsigned short Custom_Char[] = {
  0x0E, 0x0E, 0x04, 0x0E, 0x15, 0x04, 0x0A, 0x0A,                          // CGRAM memory space 1
  0b00000, 0b00000, 0b01010, 0b00100, 0b00100, 0b10001, 0b01110, 0b00000,  // CGRAM memory space 2
  0b00100, 0b01110, 0b11111, 0b11111, 0b01110, 0b01110, 0b01010, 0b01010,  // CGRAM memory space 3
  0b01110, 0b10001, 0b10001, 0b11111, 0b11011, 0b11011, 0b11111, 0b00000,  // CGRAM memory space 4
  0b01110, 0b10000, 0b10000, 0b11111, 0b11011, 0b11011, 0b11111, 0b00000,  // CGRAM memory space 5
  0b00000, 0b10001, 0b01010, 0b10001, 0b00100, 0b01110, 0b10001, 0b00000,  // CGRAM memory space 6
  0b00000, 0b00000, 0b01010, 0b10101, 0b10001, 0b01110, 0b00100, 0b00000,  // CGRAM memory space 7
  0b11111, 0b11111, 0b10101, 0b11011, 0b11011, 0b11111, 0b10001, 0b11111   // CGRAM memory space 8
};

int main(void) {
  DDRD = 0xFF;
  Lcd_Init();

  Lcd_Cmd(0x04);  // Set CGRAM Address
  Lcd_Cmd(0x00);  // .. set CGRAM Address
  for (int i = 0; i <= 63; i++)
    Lcd_Write_Char(Custom_Char[i]);

  Lcd_Clear();

  while (1) {
    Lcd_Set_Cursor(1, 1);

    Lcd_Write_Char(0);  // Display Custom Character 0
    Lcd_Write_Char(1);  // Display Custom Character 1
    Lcd_Write_Char(2);  // Display Custom Character 2
    Lcd_Write_Char(3);  // Display Custom Character 3
    Lcd_Write_Char(4);  // Display Custom Character 4
    Lcd_Write_Char(5);  // Display Custom Character 5
    Lcd_Write_Char(6);  // Display Custom Character 6
    Lcd_Write_Char(7);  // Display Custom Character 7
    _delay_ms(1000);
  }
}
