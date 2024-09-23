    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

; Se realizará una calculadora cual operará entradas de datos de 04 bits, el Puerto A será el encargado de recibir los datos de entrada y el Puerto C será el encargado de recibir los datos de salida.
; Los datos de entrada serán de 04 bits, los cuales serán divididos en dos nibbles, el nibble bajo será el encargado de realizar las operaciones y el nibble alto será el encargado de seleccionar la operación a realizar. Las operaciones a realizar serán: suma (00), resta (01), and (10) y or (11).
main:
    ; Setear direcciones de entrada/salida A
    LDI     R16,    0x3F    ; 0011 1111
    OUT     DDRA,   R16

    ; Setear direcciones de entrada C
    LDI     R16,    0x00
    OUT     DDRC,   R16

    OUT     PORTA,  R16

loop:
    IN      R17,    PINC    ; Data bits (4)
    IN      R19,    PINA    ; Decisors + Outputs

    MOV     R18,    R17     ; R18 <- yyyy xxxx
    SWAP    R17             ; R17 <- xxxx yyyy
    ANDI    R17,    0x0F    ; R17 <- 0000 xxxx
    ANDI    R18,    0x0F    ; R18 <- 0000 yyyy
    ANDI    R19,    0xC0

    CPI     R19,    0x00    ;    (R17 == 0)
    BREQ    addop           ; if            { addop() }

    CPI     R19,    0x40    ; (R17 == 0100)
    BREQ    subop

    CPI     R19,    0x80    ; (R17 == 1000)
    BREQ    andop

    CPI     R19,    0xC0    ; (R17 == 1100)
    BREQ    orop

addop:
    ADD     R17,    R18
    OUT     PORTA,  R17

    RJMP    loop

subop:
    SUB     R17,    R18
    OUT     PORTA,  R17

    RJMP    loop

andop:
    AND     R17,    R18
    OUT     PORTA,  R17

    RJMP    loop

orop:
    OR      R17,    R18
    OUT     PORTA,  R17

    RJMP    loop

