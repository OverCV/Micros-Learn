    .include"m328Pdef.inc"
    .org    0x0000
    RJMP    main

    ; Detecta las entradas en el puerto B y las replica como salida en un led del puerto D.
    main:
        LDI     R16,    0x00
        OUT     DDRD,   R16

        LDI     R16,    0xFF
        OUT     DDRB,   R16

        OUT     PORTD,  R16

    loop:
        IN      R16,    PIND
        OUT     PORTB,  R16

        rjmp    loop
