#include <avr/io.h>
#include <avr/interrupt.h>

// Definiciones para configuración
#define F_CPU 16000000UL          // Frecuencia del CPU
#define BAUD 9600                 // Velocidad en baudios deseada
#define UBRR_VALUE 103            // Valor calculado para 9600 baudios a 16MHz

// Usamos un buffer para almacenar los datos recibidos
volatile uint8_t buffer_recepcion[64];    // Arreglito para guardar datos
volatile uint8_t indice_escritura = 0;    // Índice donde se guardará el próximo dato
volatile uint8_t indice_lectura = 0;      // Índice del próximo dato a leer
volatile uint8_t hay_datos_nuevos = 0;    // Bandera para indicar datos sin procesar

// Inicialización del USART
void USART_inicializar(void) {
    // Activar el módulo USART (desactivar power reduction)
    PRR0 &= ~(1<<PRUSART0);
    
    // Configurar velocidad de transmisión
    UBRR0H = (uint8_t)(UBRR_VALUE>>8);
    UBRR0L = (uint8_t)UBRR_VALUE;
    
    // Habilitar:
    // - Receptor (RXEN0)
    // - Transmisor (TXEN0)
    // - Interrupción por recepción completa (RXCIE0)
    UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
    
    // Configurar formato de trama:
    // - 8 bits de datos (UCSZ01 y UCSZ00)
    // - 1 bit de parada (por defecto)
    // - Sin paridad (el por defecto)
    UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
    
    // Habilitar interrupciones globales
    sei();
}

// Función para transmitir un byte
void USART_transmitir(uint8_t dato) {
    // Esperar hasta que el buffer de transmisión esté vacío
    while (!(UCSR0A & (1<<UDRE0)));
    // Enviar el dato
    UDR0 = dato;
}

// Rutina de interrupción para recepción completada
ISR(USART0_RX_vect) {
    // Guardar el dato recibido en el buffer
    buffer_recepcion[indice_escritura] = UDR0;
    // Actualizar el índice de escritura (vuelve a 0 después de 63)
    indice_escritura = (indice_escritura + 1) & 0x3F;
    // Indicar que hay datos nuevos por procesar
    hay_datos_nuevos = 1;
}

int main(void) {
    // Inicializar la USART
    USART_inicializar();
    
    // Bucle infinito
    while(1) {
        // Si hay datos nuevos en el buffer
        if(hay_datos_nuevos) {
            // Procesar todos los datos en el buffer
            while(indice_lectura != indice_escritura) {
                // Leer y transmitir cada dato
                USART_transmitir(buffer_recepcion[indice_lectura]);
                // Actualizar el índice de lectura
                indice_lectura = (indice_lectura + 1) & 0x3F;
            }
            // Indicar que ya se procesaron todos los datos
            hay_datos_nuevos = 0;
        }
    }
}