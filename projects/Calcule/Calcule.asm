    .include"m328Pdef.inc"
    .org(0x0000)
    RJMP    main

; Programa calculadora. Los bits de parte alta deciden el cálculo.
; 00: Selector. 01: Suma. 10: Resta. 11: LOr. 100; LAnd.

main:
    ; Código aquí 🤗
    LDI R16, 0x00
    OUT DDRD, R16   ; pd -> Entradas

    LDI R16, 0x00
    OUT DDRB, R16   ; pd -> Entradas

    LDI R16,    0xFF
    OUT DDRC,  R16 ; pu -> Salidas

    OUT PORTD,  R16 ; Escritura
    OUT PORTB,  R16 ; Escritura

loop:
    IN R16, PIND
    IN R17, PINB

    MOV R18, R16
    MOV R19, R17

    ANDI R18, 0x80  ; 1000 0000
    SWAP R18        ; 0000 1000
    ANDI R19, 0x80  ; 1000 0000
    ADD R18, R19

    CPI R18,    0x00    ; 0000 0000
    BREQ add_op

    CPI R18,    0x08    ; 0000 1000
    BREQ sub_op

    CPI R18,    0x80    ; 1000 0000
    BREQ or_op

    CPI R18,    0x88    ; 1000 1000
    BREQ and_op

    RJMP    loop


add_op:
    ADD R16, R17
    OUT PORTC, R16

    RJMP loop

sub_op:
    SUB R16, R17
    OUT PORTC, R16

    RJMP loop

or_op:
    OR R16, R17
    OUT PORTC, R16

    RJMP loop

and_op:
    AND R16, R17
    OUT PORTC, R16

    RJMP loop

