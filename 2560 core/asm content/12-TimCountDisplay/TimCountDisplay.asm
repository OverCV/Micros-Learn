    .include    "m2560def.inc"

    .org        (0x0000)
    RJMP    main

main:
    ; Configurar el Puerto A como salida
    LDI     R16,    0xFF
    OUT     DDRA,   R16

    ; Configurar el Timer en modo CTC (Clear Timer on Compare Match)
    LDI     R16,    0x02                                ; WGM01 = 1 para habilitar modo CTC (en TCCR0A)
    OUT     TCCR0A, R16

    ; Establecer fuente de reloj externa en T0 y configurar para flanco de subida
    LDI     R16,    0x07                                ; External clock source on T0 pin, clock on rising edge (en TCCR0B)
    OUT     TCCR0B, R16

    ; Establecer el valor máximo en OCR0A (contador llegará hasta 3)
    LDI     R16,    9
    OUT     OCR0A,  R16                                 ; Comparación con OCR0A, reset del contador cuando llegue a 3

    ; Inicializar valor del contador en 0
    LDI     R16,    0x00
    STS     TIMSK0, R16

loop:
    ; Multiplicamos por 2 para acceder correctamente
    LDI     ZH,     HIGH(table * 2)
    LDI     ZL,     LOW (table * 2)

    ; Iniciar contador y puntero Z
    ; LSL     ZL
    ; ROL     ZH, 60

    ; Leer el valor del contador TCNT0
    IN      R18,    TCNT0

    ; Enviar el valor de TCNT0 a PORTA
    ADD     ZL,     R18
    LPM     R20,    Z

    ; Mostrar el dígito
    OUT     PORTA,  R20

    RJMP    loop

.org        (0x0040)
table:
    ; Tabla de dígitos 0-9 en ánodo común
    .dw         0xF9C0, 0xB0A4, 0x9299, 0xF882, 0x9080

    ; 1100.0000; 0xC0 11111001 0xF9
