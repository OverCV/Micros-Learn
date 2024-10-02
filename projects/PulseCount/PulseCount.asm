    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

    .org(0x0002)
    RJMP    rsi_0


main:
    ; Configurar el puerto A como salida (para el display de 7 segmentos)
    LDI     R16,        0xFF
    OUT     DDRA,       R16

    ; Habilitamos interrupciones globales
    SEI

    ; Habilitar interrupciones (INT_0)
    LDI     R17,         0x01
    OUT     EIMSK,      R17

    ; Configurar con flanco de subida (Sólo los 3 bits)
    LDI     R18,        0x03
    STS     EICRA,      R18

reset:
    ; Iniciar contador y puntero Z
    LDI     R17,        0
    LDI     ZH,         0x01
    LDI     ZL,         0x00
    ; Multiplicamos por 2 para acceder correctamente
    LSL     ZL
    ROL     ZH

loop:
    ; Mostrar el dígito
    LPM     R16,        Z ; esta es no necesaria
    OUT     PORTA,      R16

    ; Revisamos si recorrimos todos los dígitos (0 a 9)
    CPI     R17,        10
    BRNE    loop

    RJMP    reset

rsi_0:
    INC     R30
    RETI

table:
    ; Tabla de dígitos 0-9 en ánodo común
    .org(0x100)
    .dw 0xF9C0, 0xB0A4, 0x9299, 0xF882, 0x9080