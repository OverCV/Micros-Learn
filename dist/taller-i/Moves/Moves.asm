    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

main:
    ; Setear puertos de salida B
    LDI     R17,    0xFF
    OUT     DDRB,   R17

    ; Setear puertos de entrada C
    LDI     R16,    0x00
    OUT     DDRC,   R16

    OUT     PORTB,  R16

loop:
    IN      R16,    PINC        ; Entrada.

    ; Comparamos si entró 01
    CPI     R16,    0x01
    BREQ    blink               ; Branch If Equal

    ; Comparamos si entró 02
    CPI     R16,    0x02
    LDI     R19,    0x08
    LDI     R20,    0x10
    BREQ    go_out

    ; Comparamos si entró 03
    CPI     R16,    0x03
    LDI     R18,    0x80
    BREQ    go_right

    RJMP    loop

blink:
    LDI     R18,    0x00
    OUT     PORTB,  R18

    COM     R18
    OUT     PORTB,  R18

    RJMP    loop

go_out:
    CLR     R18             ; Limpiar
    OR      R18,     R19
    OR      R18,     R20

    OUT     PORTB,  R18

    CPI     R18,     0x81   ; Extremos
    BREQ    go_in

    LSR     R19
    LSL     R20

    RJMP    go_out

go_in:
    CLR     R18             ; Limpiar
    OR      R18,     R19
    OR      R18,     R20

    OUT     PORTB,  R18

    CPI     R18,     0x18   ; Extremos
    BREQ    loop

    LSR     R20
    LSL     R19

    RJMP    go_in

go_right:
    OUT     PORTB,  R18
    LSR     R18

    CPI     R18,    0x01
    BREQ    go_left

    RJMP    go_right

go_left:
    OUT     PORTB,  R18
    LSL     R18

    CPI     R18,    0x80
    BREQ    loop

    RJMP    go_left

