.include "m328Pdef.inc"

.org 0x0000
    rjmp main

main:
    ; Código aquí 🤗
    LDI R16,    0xFF
    OUT DDRB,   R16
    LDI R17,    0x01
    OUT PORTB,  R17
loop:
    rjmp loop
