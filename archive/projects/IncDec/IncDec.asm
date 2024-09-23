    .include"m328Pdef.inc"

    .org(0x0000)
    RJMP    main

    ;   SEI enable interruptions

    .org(0x0002)
    RJMP    rsi_0               ; Rutine Service Interruption 0. Use RETI

    .org(0x0004)
    RJMP    rsi_1               ; Rutine Service Interruption 1. Use RETI

    ; REG | DIR
    ; EIFR  | 0x1C
    ; EIMSK | 0x1D
    ; EICRA | 0x69

main:
    SEI                         ; Habilitar interrupciones globales
    LDI     R16,        0xFF
    LDI     R17,        0x08
    OUT     DDRB,       R16     ; Configura como salida

    SBI     PORTD,      2       ; 0000 1100
    SBI     PORTD,      3       ; PU - Pines 2, 3 del

    OUT     SPL,        R16     ; 0000 1000 . 1111 1111
    OUT     SPH,        R17     ; Inicicaliza puntero de pila

    LDI     R0,        0x03    ;   0000 0011
    OUT     EIMSK,      R0     ;   Configurar interrupciones (INT_0, INT_1)

    LDI     R1,        0xFF
    STS     EICRA,      R1     ; Configura flanco de subida

loop:
    RJMP    loop


rsi_0:
    LDI     R20,        0x06    ; 1
    OUT     PORTB,      R20
    RCALL   delay0_5

    LDI     R20,        0x5B    ; 2
    OUT     PORTB,      R20
    RCALL   delay0_5

    LDI     R20,        0x4F    ; 3
    OUT     PORTB,      R20
    RCALL   delay0_5

    LDI     R20,        0x00    ; Apagar
    OUT     PORTB,      R20
    RETI

rsi_1:
    LDI     R20,        0x4F    ; 3
    OUT     PORTB,      R20
    RCALL   delay0_5

    LDI     R20,        0x5B    ; 2
    OUT     PORTB,      R20
    RCALL   delay0_5

    LDI     R20,        0x06    ; 1
    OUT     PORTB,      R20
    RCALL   delay0_5

    LDI     R20,        0x00    ; Apagar
    OUT     PORTB,      R20
    RETI

delay0_5:
    ldi     r21,        3
    ldi     r22,        138
    ldi     r23,        84
L1:
    dec     r23
    brne    L1
    dec     r22
    brne    L1
    dec     r21
    brne    L1
    nop
    RET