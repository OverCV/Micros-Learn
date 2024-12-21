// uart_enhanced.cpp
#include "uart.h"
#include "stdlib.h"

UART *UART::instances[4] = { nullptr, nullptr, nullptr, nullptr };

UART::UART(UARTNumber num)
  : uartNum(num), rxHead(0), rxTail(0) {
  mapRegisters();
}

void UART::mapRegisters() {
  switch (uartNum) {
    case UART0:
      UCSRnA = &UCSR0A;
      UCSRnB = &UCSR0B;
      UCSRnC = &UCSR0C;
      UBRRn = &UBRR0;
      UDRn = &UDR0;
      break;
    case UART1:
      UCSRnA = &UCSR1A;
      UCSRnB = &UCSR1B;
      UCSRnC = &UCSR1C;
      UBRRn = &UBRR1;
      UDRn = &UDR1;
      break;
    case UART2:
      UCSRnA = &UCSR2A;
      UCSRnB = &UCSR2B;
      UCSRnC = &UCSR2C;
      UBRRn = &UBRR2;
      UDRn = &UDR2;
      break;
    case UART3:
      UCSRnA = &UCSR3A;
      UCSRnB = &UCSR3B;
      UCSRnC = &UCSR3C;
      UBRRn = &UBRR3;
      UDRn = &UDR3;
      break;
  }
}

UART *UART::getInstance(UARTNumber num) {
  if (instances[num] == nullptr) {
    instances[num] = new UART(num);
  }
  return instances[num];
}

void UART::init(uint32_t baud, DataBits dataBits, StopBits stopBits, Parity parity) {
  // Calcular baudrate
  uint16_t ubrr = F_CPU / 16 / baud - 1;
  *UBRRn = ubrr;

  // Configurar formato de frame
  uint8_t config = (parity << UPM00) | (stopBits << USBS0) | (dataBits << UCSZ00);
  *UCSRnC = config;

  // Si son 9 bits de datos, necesitamos setear un bit adicional
  if (dataBits == BITS_9) {
    *UCSRnB |= (1 << UCSZ02);
  }

  // Habilitar transmisor y receptor
  *UCSRnB |= (1 << RXEN0) | (1 << TXEN0);
}

void UART::send(const char *str) {
  if (!str) return;  // Protección contra punteros nulos

  while (*str) {
    send((uint8_t)*str++);
  }
}

void UART::send(long num) {
  char buffer[16];
  sprintf(buffer, "%ld", num);  // %ld para long
  send(buffer);
}

void UART::send(uint8_t data) {
  // Esperar que el buffer de transmisión esté vacío
  while (!(*UCSRnA & (1 << UDRE0)))
    ;
  *UDRn = data;
}

void UART::send(uint16_t num) {
  char buffer[8];
  uint8_t i = 0;

  // Manejar el caso especial de 0
  if (num == 0) {
    send('0');
    return;
  }

  // Convertir dígitos
  while (num > 0) {
    buffer[i++] = '0' + (num % 10);
    num /= 10;
  }

  // Enviar dígitos en orden inverso
  while (i > 0) {
    send(buffer[--i]);
  }
}

void UART::sendln(const char *str) {
  send(str);
  send("\r\n");
}

void UART::send(int num) {
  char buffer[16];
  sprintf(buffer, "%d", num);
  send(buffer);
}

void UART::send(float num, uint8_t decimals) {
  char buffer[16];
  dtostrf(num, 1, decimals, buffer);
  send(buffer);
}

bool UART::available() {
  return rxHead != rxTail;
}

uint8_t UART::read() {
  if (rxHead == rxTail) {
    return 0;
  }

  uint8_t data = rxBuffer[rxTail];
  rxTail = (rxTail + 1) & UART_RX_BUFFER_MASK;
  return data;
}

uint8_t UART::peek() {
  if (rxHead == rxTail) {
    return 0;
  }
  return rxBuffer[rxTail];
}

void UART::flush() {
  rxHead = rxTail = 0;
}

void UART::enableRxInterrupt(bool enable) {
  if (enable) {
    *UCSRnB |= (1 << RXCIE0);
  } else {
    *UCSRnB &= ~(1 << RXCIE0);
  }
}

void UART::enableTxInterrupt(bool enable) {
  if (enable) {
    *UCSRnB |= (1 << TXCIE0);
  } else {
    *UCSRnB &= ~(1 << TXCIE0);
  }
}

void UART::handleRxComplete(uint8_t data) {
  uint8_t nextHead = (rxHead + 1) & UART_RX_BUFFER_MASK;

  if (nextHead != rxTail) {
    rxBuffer[rxHead] = data;
    rxHead = nextHead;
  }
}

bool UART::isTxComplete() {
  return *UCSRnA & (1 << TXC0);
}

bool UART::isTxBufferEmpty() {
  return *UCSRnA & (1 << UDRE0);
}

bool UART::isFrameError() {
  return *UCSRnA & (1 << FE0);
}

bool UART::isParityError() {
  return *UCSRnA & (1 << UPE0);
}

bool UART::isOverrunError() {
  return *UCSRnA & (1 << DOR0);
}

void UART::clearErrors() {
  *UCSRnA |= (1 << TXC0);  // Limpiar flag de transmisión completa
  uint8_t dummy = *UDRn;   // Leer UDR para limpiar flags de error
  (void)dummy;             // Evitar warning de variable no usada
}