.include "m328Pdef.inc"

.org 0x0000
    rjmp main

main:
    LDI R16,    0xFF
    OUT DDRB,   R16

    LDI R17,    0x00
    OUT DDRD,   R17

    OUT PORTD,  R16
loop:
    IN  R18,    PIND
    OUT PORTB,  R18
    rjmp loop
