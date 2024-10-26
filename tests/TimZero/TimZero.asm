    .include    "m2560def.inc"

    .org        (0x0000)
    RJMP    main

    ; timer/counter compare match
    .org        (OC1Aaddr)
    rjmp    tim_isr_1

    .equ        ITERS  =  5

main:
    SEI
    LDI     R16,        0xFF
    OUT     DDRA,       R16
    OUT     DDRC,       R16
    ; Movedor del puntero
    LDI     R17,        2

conf_tim_1:
    ; configurar el modo de CTC operacion para que cuente hasta el numero que esta en OCRnA
    LDI     R16,        0x01
    STS     TCCR1A,     R16
    ; Definimos el valor máximo a comparar 15625
    LDI     R16,        0x3D
    STS     OCR1AH,     R16
    LDI     R16,        0x09
    STS     OCR1AL,     R16
    ; configurar el modo de operacion para que cuente hasta el numero que esta en OCRA
    LDI     R16,        0x1D
    STS     TCCR1B,     R16
    ; Habilitar la interrupción de comparación para el Timer1 (OCIE1A)
    LDI     R16,        0x02
    STS     TIMSK1,     R16
    ; Contador de iteraciones
    ; LDI     R25,        ITERS

reset:
    ; Inicializar
    LDI     ZL,         0

loop:
    OUT     PORTA,      ZL
    RJMP    loop

tim_isr_1:
    ; DEC     R25
    ; BRNE    reti_tim_1
    ; Los 5 son arbitrarios y parecen un buen tiempo
    ; Pero falta encontrar la ecuación para este timer y tener tiempos exactos
    ; LDI     R25,        ITERS

    ADD     ZL,         R17

reti_tim_1:
    RETI

    .org        (0x0100)
table:
    ; Tabla de datos
    .dw         0x088F, 0x2237, 0x1130, 0x0889, 0x088F
