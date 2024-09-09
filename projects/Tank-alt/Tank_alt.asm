    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

; PORTC XYZW
; PORTB ABCD

main:
    ; Setear puertos de entrada C
    LDI     R16,    0x00
    OUT     DDRC,   R16

    ; Setear puertos de salida B
    LDI     R17,    0xFF
    OUT     DDRB,   R17

    ; Activar pull-up Escritura
    OUT     PORTB,  R17


loop:
    IN      R17,    PINC
    OUT     PORTB,  R17

    ; ; Si está en el mínimo
    CPI     R17,    0x01
    BREQ    minimal

    ; Si está en el máximo
    CPI     R17,    0x0F
    BREQ    maximal

    RJMP    loop

minimal:
    LDI     R18,    0x80    ; 0000 0001
    OUT     PORTB,  R18
    RJMP    loop

maximal:
    LDI     R18,    0x40
    OUT     PORTB,  R18
    RJMP    loop