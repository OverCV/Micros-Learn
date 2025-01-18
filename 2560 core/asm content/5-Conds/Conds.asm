    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main


/*
loop -> sr1 | sr2

sr1 {a} -> input
sr1 {b, loop} <- | sr2 {a} <-

sr2 {a} -> input
sr2 {b, loop} <- | sr1 {a} <-
*/

main:
    ; Setear puertos de entrada A
    LDI     R16,        0xFC    ; 1111 1100
    OUT     DDRA,       R16

    ; Setear puertos de salida C
    LDI     R16,        0xFF
    OUT     DDRC,       R16

loop:
    LDI     R18,        0x00
    OUT     PORTC,      R18

    IN      R16,        PINA

    CPI     R16,        0x01
    BREQ    sr1

    CPI     R16,        0x02
    BREQ    sr2

    RJMP    loop

input:
    ; ONLY WITH RCALL - 0x01 - 0x02
    IN      R16,        PIND

    CP      R16,        R21
    BREQ    go_back

    CPI     R16,        0x01
    BREQ    sr1

    CPI     R16,        0x02
    BREQ    sr2

    RJMP    loop

go_back:
    RET

sr1:
    LDI     R21,        0x01

    LDI     R18,        0xAA
    OUT     PORTC,      R18

    RCALL   delay
    RCALL   input

    LDI     R18,        0x55
    OUT     PORTC,      R18

    RCALL   delay
    RCALL   input

    RJMP    loop

sr2:
    LDI     R21,        0x02

    LDI     R18,        0x99
    OUT     PORTC,      R18

    RCALL   delay
    RCALL   input

    LDI     R18,        0x66
    OUT     PORTC,      R18

    RCALL   delay
    RCALL   input

    RJMP    loop

delay:
    ; Subrutina de retraso (0.2 seg)
    LDI     R25,        41
    LDI     R26,        150
    LDI     R27,        128
L1:
    DEC     R27
    BRNE    L1
    DEC     R26
    BRNE    L1
    DEC     R25
    BRNE    L1
    RET

/*
delay:

for i=255 to 0:
    INST X
    i--
    for j=255 to 0:
        INST Y
        j--
    j=255
*/