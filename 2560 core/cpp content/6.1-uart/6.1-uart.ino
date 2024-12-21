#include <avr/io.h>
#include <util/delay.h>

// Función básica para inicializar UART
void uart_init(uint32_t baud) {
  uint16_t ubrr = F_CPU / 16 / baud - 1;

  // Establecer baudrate
  UBRR0H = (uint8_t)(ubrr >> 8);
  UBRR0L = (uint8_t)ubrr;

  // Habilitar transmisión y recepción
  UCSR0B = (1 << RXEN0) | (1 << TXEN0);

  // Formato: 8 bits datos, 1 bit parada
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

// Función para enviar un carácter
void uart_send(uint8_t data) {
  // Esperar que el buffer de transmisión esté vacío
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = data;
}

// Función para enviar una cadena
void uart_print(const char* str) {
  while (*str) {
    uart_send(*str++);
  }
}

int main(void) {
  uart_init(9600);  // Inicializar a 9600 baud

  while (1) {
    uart_print("Test\r\n");  // Enviar "Test" y nueva línea
    _delay_ms(1000);         // Esperar 1 segundo
  }

  return 0;
}