// Definición de pines del keypad
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL
#define ADDR_ESCLAVO 0x50

// Definición de pines
const byte PIN_FILAS[] = {2, 3, 4, 5};
const byte PIN_COLUMNAS[] = {50, 51, 52, 53};

// Matriz de símbolos del keypad
const char TECLAS[4][4] = {
    {'1', '2', '3', 'a'},
    {'4', '5', '6', 'b'},
    {'7', '8', '9', 'c'},
    {'f', '0', 'e', 'd'}
};

// Variables globales para el keypad
volatile bool teclaDetectada = false;
volatile byte filaActual = 0;
volatile uint8_t ultimaTecla = 0;
volatile uint32_t ultimoTiempo = 0;

// Función de inicialización I2C
void I2C_inicializar_maestro(void) {
    TWSR = 0;
    TWBR = ((F_CPU/100000UL)-16)/2;
    TWCR = (1<<TWEN);
}

// Funciones I2C básicas
void I2C_start(void) {
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
}

void I2C_stop(void) {
    TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void I2C_write(uint8_t data) {
    TWDR = data;
    TWCR = (1<<TWINT)|(1<<TWEN);
    while(!(TWCR & (1<<TWINT)));
}

// Inicialización del keypad
void keypad_init(void) {
    // Configurar pines de fila como salida
    for(byte i = 0; i < 4; i++) {
        pinMode(PIN_FILAS[i], OUTPUT);
        digitalWrite(PIN_FILAS[i], HIGH);
        // Configurar pines de columna como entrada con pull-up
        pinMode(PIN_COLUMNAS[i], INPUT_PULLUP);
    }
    
    // Configurar interrupciones
    PCICR |= (1 << PCIE0);
    PCMSK0 |= (1 << PCINT0) | (1 << PCINT1) | (1 << PCINT2) | (1 << PCINT3);
}

// Enviar carácter por I2C
void enviar_caracter(char c) {
    I2C_start();
    I2C_write(ADDR_ESCLAVO<<1);
    I2C_write(c);
    I2C_stop();
}

// ISR para detección de tecla
ISR(PCINT0_vect) {
    uint32_t tiempoActual = millis();
    if(tiempoActual - ultimoTiempo > 200) { // Debounce de 200ms
        teclaDetectada = true;
        ultimoTiempo = tiempoActual;
    }
}

// Función para escanear el keypad
char escanear_keypad(void) {
    char teclaPresionada = 0;
    
    if(teclaDetectada) {
        digitalWrite(PIN_FILAS[filaActual], HIGH);
        filaActual = (filaActual + 1) % 4;
        digitalWrite(PIN_FILAS[filaActual], LOW);
        
        for(byte columna = 0; columna < 4; columna++) {
            if(digitalRead(PIN_COLUMNAS[columna]) == LOW) {
                teclaPresionada = TECLAS[filaActual][columna];
                break;
            }
        }
        teclaDetectada = false;
    }
    return teclaPresionada;
}

int main(void) {
    // Inicialización
    I2C_inicializar_maestro();
    keypad_init();
    sei();
    
    char teclaActual;
    
    while(1) {
        teclaActual = escanear_keypad();
        
        if(teclaActual != 0) {
            enviar_caracter(teclaActual);
            _delay_ms(100); // Pequeña pausa entre envíos
        }
        
        _delay_us(500); // Pequeña pausa en el loop principal
    }
    
    return 0;
}