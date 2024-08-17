    .include"m328Pdef.inc"

    .org(0x0000)
    RJMP    main

    .org(0x0002)
    RJMP    rsi_0

    .org(0x0004)
    RJMP    rsi_1

main:
    SEI                     ; Hab. Glob. Interr.

    LDI     R16,    0xFF
    LDI     R17,    0x08
    OUT     DDRB,   R16     ; Conf. Salida.

    SBI     PORTD,  2
    SBI     PORTD,  3

    OUT     SPL,    R16
    OUT     SPH,    R17

    LDI     R18,    0x03    ;   0000 0011
    OUT     EIMSK,  R18     ;   Configurar interrupciones (INT_0, INT_1)

    LDI     R19,    0xFF
    STS     EICRA,  R19     ; Configura flanco de subida

    LDI     R20,    0x01

loop:
    OUT     PORTB,  R20
    RJMP    loop

rsi_0:
    INC     R20
    RETI

rsi_1:
    DEC     R20
    RETI