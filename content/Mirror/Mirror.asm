    ; .include"m2560def.inc"
    .org(0x0000)
    RJMP    main

    ; Los pines de entrada en puerto D desde 0 -> 3
    ; Serán vistos como salidas del puerto D desde 4 -> 7
main:
    LDI     R16,    0x00    ;   Salidas 7->4, Entradas 0->3
    OUT     DDRA,   R16

    LDI     R16,    0xFF    ;   Salidas 7->4, Entradas 0->3
    OUT     DDRC,   R16

loop:
    IN      R16,    PINA
    OUT     PORTC,  R16

    RJMP    loop
