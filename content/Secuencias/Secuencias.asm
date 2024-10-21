    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

    ; Programa que hace que según la selección del usuario por el Puerto A con (01) se encienda y apague un led, con (02) se haga un blink, con (03) se haga que un led se mueva de izquierda a derecha y viceversa.

main:
    ; Setear puertos de entrada A
    LDI     R16,        0x00
    OUT     DDRA,       R16

    ; Setear puertos de salida C
    LDI     R17,        0xFF
    OUT     DDRC,       R17

loop:
    ; Lectura de entrada por PIN A.
    IN      R16,        PINA

    ; Comparamos si entró 01Hex
    CPI     R16,        0x01
    BREQ    blink               ; Branch If Equal

    ; Comparamos si entró 02H
    LDI     R19,        0x08
    LDI     R20,        0x10
    CPI     R16,        0x02
    BREQ    go_out

    ; Comparamos si entró 03H
    LDI     R18,        0x80
    CPI     R16,        0x03
    BREQ    go_right

    RJMP    loop

blink:
    LDI     R18,        0x00
    OUT     PORTC,      R18
    RCALL   delay

    COM     R18
    OUT     PORTC,      R18
    RCALL   delay

    RJMP    loop

go_out:
    CLR     R18                 ; Limpiar
    OR      R18,        R19
    OR      R18,        R20

    OUT     PORTC,      R18
    RCALL   delay

    CPI     R18,        0x81    ; Extremos
    BREQ    go_in

    LSR     R19
    LSL     R20

    RJMP    go_out

go_in:
    CLR     R18                 ; Limpiar
    OR      R18,        R19
    OR      R18,        R20

    OUT     PORTC,      R18
    RCALL   delay

    CPI     R18,        0x18    ; Extremos
    BREQ    loop

    LSR     R20
    LSL     R19

    RJMP    go_in

go_right:
    OUT     PORTC,      R18
    LSR     R18

    RCALL   delay

    CPI     R18,        0x01
    BREQ    go_left

    RJMP    go_right

go_left:
    OUT     PORTC,      R18
    RCALL   delay



    LSL     R18
    CPI     R18,        0x80
    BREQ    loop

    RJMP    go_left

delay:
    ; Subrutina de retraso (0.2 seg)
    LDI     R25,        17
    LDI     R26,        60
    LDI     R27,        201
L1:
    DEC     R27
    BRNE    L1
    DEC     R26
    BRNE    L1
    DEC     R25
    BRNE    L1
    RET