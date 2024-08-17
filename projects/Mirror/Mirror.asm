    .include"m328Pdef.inc"
    .org(0x0000)
    RJMP    main

    ; Los pines de entrada en puerto D desde 0 -> 3
    ; Serán vistos como salidas del puerto D desde 4 -> 7
main:
    LDI     R16,    0xF0    ;   Salidas 7->4, Entradas 0->3
    OUT     DDRD,   R16

    LDI     R16,    0x0F
    OUT     PORTD,  R16     ;   PU - Sólo a entradas

loop:
    IN      R16,    PIND
    SWAP    R16

    OUT     PORTD,  R16

    RJMP    loop
