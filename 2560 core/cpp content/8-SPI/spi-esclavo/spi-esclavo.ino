#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

// Definición pines LCD
#define RS 4
#define EN 6
#define D4 9
#define D5 10
#define D6 11
#define D7 12

// Definiciones SPI
#define DD_MISO DDB3
#define SPI_PORT PORTB
#define SPI_DDR  DDRB

// Variables globales
volatile char datoRecibido;
volatile bool nuevoDato = false;
const bool MODO_SIMULACION = true; // Cambiar a FALSE para hardware real

void lcdSendNibble(uint8_t nibble) {
    digitalWrite(D4, (nibble >> 0) & 0x01);
    digitalWrite(D5, (nibble >> 1) & 0x01);
    digitalWrite(D6, (nibble >> 2) & 0x01);
    digitalWrite(D7, (nibble >> 3) & 0x01);
    
    digitalWrite(EN, HIGH);
    if (MODO_SIMULACION) {
        delayMicroseconds(1530); // Tiempo para simulación
    } else {
        delayMicroseconds(1);    // Tiempo para hardware real
    }
    digitalWrite(EN, LOW);
    if (MODO_SIMULACION) {
        delayMicroseconds(1530);
    } else {
        delayMicroseconds(1);
    }
}

void lcdCommand(uint8_t command) {
    digitalWrite(RS, LOW);
    lcdSendNibble(command >> 4);
    lcdSendNibble(command);
    
    if (MODO_SIMULACION) {
        delay(2);
    } else {
        delayMicroseconds(100);
    }
}

void lcdWriteChar(char c) {
    digitalWrite(RS, HIGH);
    lcdSendNibble(c >> 4);
    lcdSendNibble(c);
    
    if (MODO_SIMULACION) {
        delay(2);
    } else {
        delayMicroseconds(100);
    }
}

void lcdInit() {
    delay(50);
    
    // Secuencia de inicialización simplificada como en tu calculadora
    lcdCommand(0x03);
    delay(5);
    lcdCommand(0x03);
    delayMicroseconds(150);
    lcdCommand(0x03);
    lcdCommand(0x02);
    
    lcdCommand(0x28); // 4 bits, 2 líneas
    lcdCommand(0x0C); // Display ON, cursor OFF
    lcdCommand(0x06); // Incremento
    lcdCommand(0x01); // Clear
    delay(2);
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

void SPI_SlaveInit(void) {
    SPI_DDR |= (1<<DD_MISO);
    SPI_DDR &= ~((1<<DDB2)|(1<<DDB1)|(1<<DDB0));
    SPCR = (1<<SPE)|(1<<SPIE);
    uint8_t dummy = SPSR;
    dummy = SPDR;
}

ISR(SPI_STC_vect) {
    datoRecibido = SPDR;
    nuevoDato = true;
}

void setup() {
    Serial.begin(9600);
    
    // Configurar pines LCD
    pinMode(RS, OUTPUT);
    pinMode(EN, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    
    // Inicializar LCD
    lcdInit();
    lcdPrint("Esperando...");
    
    // Inicializar SPI
    SPI_SlaveInit();
    sei();
    
    Serial.println("Sistema Esclavo iniciado");
}

void loop() {
    if(nuevoDato) {
        Serial.print("Dato recibido: ");
        Serial.println(datoRecibido);
        
        lcdClear();
        lcdSetCursor(0, 0);
        lcdPrint("Tecla recibida:");
        lcdSetCursor(0, 1);
        lcdWriteChar(datoRecibido);
        
        nuevoDato = false;
    }
}