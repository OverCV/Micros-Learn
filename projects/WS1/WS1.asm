.include "m328Pdef.inc"

; Mostrar como salida ON=1, OFF=0: 10101010
.org 0x0000
    rjmp main

main:
    ; Seteamos la salidas (pull-up)
    LDI R16,    0xFF
    OUT DDRB,   R16

    LDI R16,    0xAA    ; 10101010
loop:
    OUT PORTB,  R16
    rjmp loop
