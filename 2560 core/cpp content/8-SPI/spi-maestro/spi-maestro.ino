#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <Arduino.h>

// Definiciones SPI
#define DD_MOSI DDB2  // PB2 en MEGA
#define DD_SCK  DDB1  // PB1 en MEGA
#define DD_SS   DDB0  // PB0 en MEGA
#define SPI_PORT PORTB
#define SPI_DDR  DDRB

// Definición keypad
const byte PIN_FILAS[] = {2, 3, 4, 5};
const byte PIN_COLUMNAS[] = {A8, A9, A10, A11}; // PCINT16-19

// Matriz del keypad
const char TECLAS[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

// Variables globales
volatile bool teclaDetectada = false;
volatile byte filaActual = 0;

void SPI_MasterInit(void) {
    // Configurar MOSI, SCK y SS como salidas
    SPI_DDR |= (1<<DD_MOSI)|(1<<DD_SCK)|(1<<DD_SS);
    // MISO como entrada
    SPI_DDR &= ~(1<<DDB3);
    
    // Habilitar SPI, Modo Maestro, velocidad fck/16
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);
    
    // SS inicia en alto
    SPI_PORT |= (1<<DD_SS);
}

void SPI_Transmit(uint8_t data) {
    SPI_PORT &= ~(1<<DD_SS);    // SS bajo
    SPDR = data;                // Enviar dato
    while(!(SPSR & (1<<SPIF))); // Esperar transmisión
    SPI_PORT |= (1<<DD_SS);     // SS alto
}

void keypad_init(void) {
    // Configurar filas como salidas
    for(byte i = 0; i < 4; i++) {
        pinMode(PIN_FILAS[i], OUTPUT);
        digitalWrite(PIN_FILAS[i], HIGH);
    }
    
    // Configurar columnas como entradas con pull-up
    for(byte i = 0; i < 4; i++) {
        pinMode(PIN_COLUMNAS[i], INPUT_PULLUP);
    }
    
    // Configurar interrupciones PCINT
    PCICR |= (1 << PCIE2);    // Habilitar PCINT16:23
    PCMSK2 |= (1 << PCINT16) | (1 << PCINT17) | 
              (1 << PCINT18) | (1 << PCINT19);
}

// ISR para detección de tecla
ISR(PCINT2_vect) {
    teclaDetectada = true;
}

void setup() {
    // Inicialización
    Serial.begin(9600);
    SPI_MasterInit();
    keypad_init();
    sei();
    
    Serial.println("Sistema Maestro iniciado");
}

void loop() {
    digitalWrite(PIN_FILAS[filaActual], HIGH);
    filaActual = (filaActual + 1) % 4;
    digitalWrite(PIN_FILAS[filaActual], LOW);
    
    if(teclaDetectada) {
        for(byte columna = 0; columna < 4; columna++) {
            if(digitalRead(PIN_COLUMNAS[columna]) == LOW) {
                char tecla = TECLAS[filaActual][columna];
                
                Serial.print("Tecla enviada por SPI: ");
                Serial.println(tecla);
                
                SPI_Transmit(tecla);
                break;
            }
        }
        teclaDetectada = false;
    }
    
    _delay_us(500);
}
