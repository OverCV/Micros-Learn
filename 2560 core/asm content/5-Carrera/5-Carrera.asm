    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main


/*

00: Vuelta en cero
01: Vuelta en ocho
*/

.def DEL1 = R25
.def DEL2 = R26
.def DEL3 = R27

main:
    ; Definir entradas
    LDI     R16,    0xCF
    OUT     DDRA,   R16

    ; Definir salida (D7S)
    LDI     R16,    0x7F
    OUT     DDRC,   R16

    LDI     R20,    0x01
    LDI     DEL1,   255
    LDI     DEL2,   50
    LDI     DEL3,   255

reset:

    ; Inicializar

loop:
    IN      R17,    PINA    ; 1100 1111
    SWAP    R17             ; 1111 1100 
    ANDI    R17,    0x03    ; ____ __00

    ; SBRC
    ; SBRS

    CPI     R17,    0x01    ; 0000 0001
    BREQ    cero
    
    ; CPI     R17,    0x02    ; 0000 0010
    ; BREQ    ocho
    RJMP    ocho_ez

cero:
    RJMP    loop

ocho_ez:
    ; Estado del led
    ; 01, 02, 40, 20, 10, 08, 04, 40, 20
    ; A > B > G > E > D > C > G > F

    ; __GFE.DCBA
    ; __654.3210

    ; _____.___A
    ; __000.0001
    LDI     R20,    0x01
    OUT     PORTC,  R20
    RCALL   delay

    ; _____.__B_
    ; __000.0010
    LDI     R20,    0x02
    OUT     PORTC,  R20
    RCALL   delay

    ; __G__.____
    ; __100.0000
    LDI     R20,    0x40
    OUT     PORTC,  R20
    RCALL   delay

    ; ____E.____
    ; __001.0000
    LDI     R20,    0x10
    OUT     PORTC,  R20
    RCALL   delay

    LDI     R20,    0x08
    OUT     PORTC,  R20
    RCALL   delay

    LDI     R20,    0x04
    OUT     PORTC,  R20
    RCALL   delay

    LDI     R20,    0x40
    OUT     PORTC,  R20
    RCALL   delay

    LDI     R20,    0x20
    OUT     PORTC,  R20
    RCALL   delay

    RJMP    loop

delay:
    DEC     DEL1
    BRNE    delay

    DEC     DEL2
    BRNE    delay

    DEC     DEL3
    LDI     DEL2,   50
    BRNE    delay
    RET
