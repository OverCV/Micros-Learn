    .include    "m2560def.inc"

    .org        (0x0000)
    rjmp    main

    .org        (OC1Aaddr)
    rjmp    tim_isr_1

    ; configuracion de los puertos A y C como salida
main:
    LDI     R16,        0xFF
    OUT     DDRA,       R16
    OUT     DDRC,       R16

    ; Movedor del puntero
    LDI     R17,        2

    SEI

conf_tim_1:
    ; configurar el modo de operacion para que cuente hasta el numero que esta en OCRA
    LDI     R16,        0x00
    STS     TCCR1A,     R16
    ; configurar el preescalar en 1024 para alcanzar el numero 15.625
    LDI     R16,        0x3D
    STS     OCR1AH,     R16
    LDI     R16,        0x09
    STS     OCR1AL,     R16
    ; configurar el modo de operacion para que cuente hasta el numero que esta en OCRA
    LDI     R16,        0x0D
    STS     TCCR1B,     R16
    ; Habilitar la interrupción de comparación para el Timer1 (OCIE1A)
    LDI     R16,        0x02
    STS     TIMSK1,     R16
    ; Contador de iteraciones
    LDI     R25,        5

reset:
    LDI     R31,        HIGH(table *  2)
    LDI     ZL,         LOW(table  *  2)

loop:
    ; Se complementan los valores porque se guardaron para Cátodo.
    LPM     R18,        Z+
    COM     R18
    OUT     PORTA,      R18

    LPM     R19,        Z+
    COM     R19
    OUT     PORTC,      R19

    SUB     ZL,         R17

    CPI     ZL,         LOW(table  *  2) + 20
    BREQ    reset

    RJMP    loop

tim_isr_1:
    DEC     R25
    BRNE    reti_tim_1
    ; Los 5 son arbitrarios y parecen un buen tiempo
    ; Pero falta encontrar la ecuación para este timer y tener tiempos exactos
    LDI     R25,        5

    ADD     ZL,         R17

reti_tim_1:
    RETI

    .org        (0x050)
table:
    ; P, A, U, L, A
    .dw         0x2233, 0x2237, 0x003E, 0x0038, 0x2237
    ; D, A, V, I, D
    .dw         0x088F, 0x2237, 0x1130, 0x0889, 0x088F
