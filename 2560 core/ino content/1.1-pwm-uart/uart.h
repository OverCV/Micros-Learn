// uart.h
#define UART_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

// Tamaño del buffer circular (debe ser potencia de 2)
#define UART_RX_BUFFER_SIZE 64
#define UART_RX_BUFFER_MASK (UART_RX_BUFFER_SIZE - 1)

class UART {
public:
  // Enums para configuración
  enum UARTNumber {
    UART0,
    UART1,
    UART2,
    UART3
  };

  enum StopBits {
    STOP_1 = 0,
    STOP_2 = 1
  };

  enum Parity {
    NONE = 0,
    EVEN = 2,
    ODD = 3
  };

  enum DataBits {
    BITS_5 = 0,
    BITS_6 = 1,
    BITS_7 = 2,
    BITS_8 = 3,
    BITS_9 = 7
  };

private:
  static UART *instances[4];
  UARTNumber uartNum;

  // Registros específicos para cada UART
  volatile uint8_t *UCSRnA;
  volatile uint8_t *UCSRnB;
  volatile uint8_t *UCSRnC;
  volatile uint16_t *UBRRn;
  volatile uint8_t *UDRn;

  // Buffer circular para recepción
  volatile uint8_t rxBuffer[UART_RX_BUFFER_SIZE];
  volatile uint8_t rxHead;
  volatile uint8_t rxTail;

  UART(UARTNumber num);
  void mapRegisters();

public:
  static UART *getInstance(UARTNumber num = UART0);

  // Configuración básica
  void init(uint32_t baud,
            DataBits dataBits = BITS_8,
            StopBits stopBits = STOP_1,
            Parity parity = NONE);

  // Funciones de transmisión
  void send(uint8_t data);
  void send(const char *str);
  void sendln(const char *str);
  void send(int num);
  void send(long num);
  void send(float num, uint8_t decimals = 2);
  void send(uint16_t num);  // Nueva declaración

  // Funciones de recepción
  bool available();
  uint8_t read();
  uint8_t peek();
  void flush();

  // Habilitar/deshabilitar interrupciones
  void enableRxInterrupt(bool enable);
  void enableTxInterrupt(bool enable);

  // Manejo de interrupción (llamado desde ISR)
  void handleRxComplete(uint8_t data);

  // Estado del UART
  bool isTxComplete();
  bool isTxBufferEmpty();
  bool isFrameError();
  bool isParityError();
  bool isOverrunError();

  // Limpieza de errores
  void clearErrors();
};
