#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <util/delay.h>

#define F_CPU 16000000UL
#define MI_DIRECCION 0x50

// Definición de pines LCD
#define RS 7
#define EN 8
#define RW 6
#define D4 9
#define D5 10
#define D6 11
#define D7 12

// Variables globales
volatile uint8_t dato_recibido;
volatile uint8_t posicion_cursor = 0;
volatile uint8_t linea_actual = 0;

// Funciones LCD
void lcdCommand(uint8_t command);
void lcdInit(void);
void lcdWriteChar(char c);
void setCursor(uint8_t col, uint8_t row);
void lcdSendNibble(uint8_t nibble);

// Inicialización I2C
void I2C_inicializar_esclavo(void) {
    // Configurar I2C
    TWAR = MI_DIRECCION << 1;
    TWCR = (1<<TWEN)|(1<<TWEA)|(1<<TWIE);
}

// Inicialización de pines LCD
void configurar_pines_lcd(void) {
    // Configurar pines como salida
    pinMode(RS, OUTPUT);
    pinMode(EN, OUTPUT);
    pinMode(RW, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    
    // Estado inicial
    digitalWrite(RS, LOW);
    digitalWrite(EN, LOW);
    digitalWrite(RW, LOW);
}

// Procesar carácter recibido
void procesar_caracter(char c) {
    switch(c) {
        case 'f': // Clear display
            lcdCommand(0x01);
            posicion_cursor = 0;
            linea_actual = 0;
            _delay_ms(2);
            break;
            
        case 'e': // Nueva línea
            if(linea_actual == 0) {
                setCursor(0, 1);
                linea_actual = 1;
                posicion_cursor = 0;
            }
            break;
            
        case 'd': // Retroceso
            if(posicion_cursor > 0) {
                posicion_cursor--;
                setCursor(posicion_cursor, linea_actual);
                lcdWriteChar(' ');
                setCursor(posicion_cursor, linea_actual);
            }
            break;
            
        default: // Carácter normal
            if(posicion_cursor < 16) {
                lcdWriteChar(c);
                posicion_cursor++;
            }
            else if(linea_actual == 0) {
                linea_actual = 1;
                posicion_cursor = 0;
                setCursor(0, 1);
                lcdWriteChar(c);
                posicion_cursor++;
            }
            break;
    }
}

// ISR para I2C
ISR(TWI_vect) {
    switch(TW_STATUS) {
        case TW_SR_DATA_ACK:
            dato_recibido = TWDR;
            procesar_caracter(dato_recibido);
            TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWEA)|(1<<TWIE);
            break;
            
        default:
            TWCR = (1<<TWEN)|(1<<TWINT)|(1<<TWEA)|(1<<TWIE);
            break;
    }
}

int main(void) {
    // Inicialización
    configurar_pines_lcd();
    lcdInit();
    I2C_inicializar_esclavo();
    sei();
    
    // Mensaje inicial
    setCursor(0, 0);
    const char* msg = "LCD Listo";
    char* ptr = msg;
    while(*ptr) {
        lcdWriteChar(*ptr++);
        _delay_ms(1);
    }
    
    _delay_ms(1000);
    lcdCommand(0x01); // Limpiar pantalla
    
    while(1) {
        // Todo se maneja en la interrupción
    }
    
    return 0;
}

void lcdCommand(uint8_t command) {
  digitalWrite(RS, LOW); 
  digitalWrite(RW, LOW);
  delayMicroseconds(100);  // Tiempo para estabilizar RS/RW
  
  lcdSendNibble(command >> 4);  
  lcdSendNibble(command);      
  
  delay(2);  // Tiempo después del comando
}

void lcdSendNibble(uint8_t nibble) {
  digitalWrite(D4, (nibble >> 0) & 0x01);
  digitalWrite(D5, (nibble >> 1) & 0x01);
  digitalWrite(D6, (nibble >> 2) & 0x01);
  digitalWrite(D7, (nibble >> 3) & 0x01);
  
  digitalWrite(EN, HIGH);
  delayMicroseconds(1530);  // Tiempo de enable corregido
  digitalWrite(EN, LOW);
  delayMicroseconds(1530);  // Tiempo entre nibbles
}

void lcdInit() {
  delay(50); 
  lcdCommand(0x03); 
  delay(5);
  lcdCommand(0x03);
  delayMicroseconds(150);
  lcdCommand(0x03);
  lcdCommand(0x02); // Modo de 4 bits configurado

  // Configuración de pantalla
  lcdCommand(0x28); // LCD de 2 líneas, 5x8 matriz de puntos
  delay(5);
  lcdCommand(0x0f); // Display encendido, cursor apagado
  delay(5);
  lcdCommand(0x06); // Incremento automático, sin desplazamiento
  delay(5);
  lcdCommand(0x01); // Limpiar pantalla
  delay(5);
}

void setCursor(uint8_t col, uint8_t row) {
  uint8_t address;
  if (row == 0) {
    address = col;
  } else {
    address = 0x40 + col;
  }
  lcdCommand(0x80 | address);
  delay(5);  // Tiempo para procesar comando
}

void lcdWriteChar(char c) {
  digitalWrite(RS, HIGH); 
  digitalWrite(RW, LOW);   
  delayMicroseconds(100);  // Tiempo para estabilizar RS
  
  lcdSendNibble(c >> 4);  
  lcdSendNibble(c);       
  
  delay(2);  // Tiempo entre caracteres
}

void lcdPrint(const char* str) {
  while (*str) {
    lcdWriteChar(*str++);
    delay(1);  // Pequeña pausa entre caracteres
  }
}
