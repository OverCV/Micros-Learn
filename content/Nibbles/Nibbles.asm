    .include"m2560def.inc"

    .org(0x0000)
    RJMP main

; Programa que manipula los nibbles de la memoria en la posición 200H (enmascaramiento de bits).

main:
    LDI     R16,        0xA5
    STS     0x0200,     R16
    LDS     R17,        0x0200

    ; RJMP    lower_nibble
    ; RJMP    all_nibbles
    RJMP    any_nibble

lower_nibble:
    ; Hacer que el nibble bajo de la memoria posición 200H valga F
    ORI     R17,        0x0F
    STS     0x0200,     R17

    RJMP    lower_nibble

all_nibbles:
    ; Hacer que todo el nibble valga 0 (posición 200H)
    ANDI    R17,        0x00
    STS     0x0200,     R17

    RJMP    all_nibbles

any_nibble:
    ; Hacer que algún nibble se niegue (posición 200H)
    LDI     R18,        0x0F    ; Negar parte baja
    LDI     R19,        0xF0    ; Negar parte alta

    EOR     R17,        R19
    STS     0x0200,     R17

    RJMP    any_nibble

