    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

    .org(0x0002)
    RJMP    rsi_0

    .org(0x0004)
    RJMP    rsi_1

    ; Se realizará una calculadora cual operará entradas de datos de 04 bits, el Puerto A será el encargado de recibir los datos de entrada y el Puerto C será el encargado de recibir los datos de salida.
    ; Los datos de entrada serán de 04 bits, los cuales serán divididos en dos nibbles, el nibble bajo será el encargado de realizar las operaciones y el nibble alto será el encargado de seleccionar la operación a realizar. Las operaciones a realizar serán: suma (00), resta (01), and (10) y or (11).
main:
    ; Setear direcciones de entrada A
    LDI     R16,    0x00
    OUT     DDRA,   R16

    ; Setear direcciones de entrada/salida C
    LDI     R16,    0xFF    ; 0011 1111
    OUT     DDRC,   R16

    ; Habilitamos interrupciones globales
    SEI

    ; Habilitar interrupciones (INT_0, INT_1)
    LDI     R17,    0x03    ;   (0000.0011)
    OUT     EIMSK,  R17

    ; Configurar con flanco de subida (bits 0, 1 activos)
    LDI     R18,    0x03
    STS     EICRA,  R18
    LDI     R20,    0x00

loop:
    IN      R17,    PINA    ; Data bits (4)
    ; IN      R19,    PINC    ; Decisors + Outputs
    ; Limpiamos R19 para compararlo sus 2 bits más significantes
    ANDI    R19,    0xC0

    MOV     R18,    R17     ; R18 <- yyyy xxxx
    SWAP    R17             ; R17 <- xxxx yyyy
    ANDI    R17,    0x0F    ; R17 <- 0000 xxxx
    ANDI    R18,    0x0F    ; R18 <- 0000 yyyy


    CPI     R20,    0x00    ; (R17 == 0000)
    BREQ    addop

    CPI     R20,    0x40    ; (R17 == 0100)
    BREQ    subop

    CPI     R20,    0x80    ; (R17 == 1000)
    BREQ    andop

    CPI     R20,    0xC0    ; (R17 == 1100)
    BREQ    orop

addop:
    ADD     R17,    R18
    OUT     PORTC,  R17

    RJMP    loop

subop:
    SUB     R17,    R18
    OUT     PORTC,  R17

    RJMP    loop

andop:
    AND     R17,    R18
    OUT     PORTC,  R17

    RJMP    loop

orop:
    OR      R17,    R18
    OUT     PORTC,  R17

    RJMP    loop

rsi_0:
    LDI     R19,    0x40
    EOR     R20,    R19
    RETI

rsi_1:
    LDI     R19,    0x80
    EOR     R20,    R19
    RETI