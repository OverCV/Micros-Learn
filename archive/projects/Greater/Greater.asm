    .include"m328Pdef.inc"
    .org(0x0000)
    RJMP    main

    ; Comparador donde el número mayor será el mostrado, sean iguales se activará el bit 7.

    ; Primer número (Nibble bajo) -> PORT (D0 - D3)
    ; Segundo número (Nibble alto) -> PORT (D4 - D7)
    ; Número mayor -> PORT (B0 - B6)
    ; Sean iguales -> PORT (B7)

main:
    LDI     R16,                0x00
    OUT     DDRD,               R16     ; Entrada

    LDI     R16,                0xFF
    OUT     DDRB,               R16     ; Salida

    OUT     PORTD,              R16     ; PU - Activar

loop:
    IN      R16,                PIND

    MOV     R17,                R16
    SWAP    R16

    ANDI    R16,                0x0F    ; 0a
    ANDI    R17,                0x0F    ; 0b

    ; (-) a <- a-B
    ; (+) a <- A-b
    MOV     R0,                R16

    SUB     R16,                R17
    BRMI    set_lower_nibble
    BREQ    set_higher_led

    OUT     PORTB,               R0
    RJMP    loop

set_higher_led:
    LDI     R0,                0x80

    OUT     PORTB,               R0
    RJMP    loop

set_lower_nibble:
    MOV     R0,                R17

    OUT     PORTB,               R0
    RJMP    loop
