    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

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

    RJMP    loop

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

