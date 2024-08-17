    .include"m328Pdef.inc"
    .org    (0x0000)
    RJMP    main

    ; Detecta las entradas en el Pin D y las emite por el Puerto D.
    main:
        LDI     R16,    0x00
        OUT     DDRD,   R16     ; Entrada

        LDI     R16,    0xFF
        OUT     DDRB,   R16     ; Salida

        OUT     PORTD,  R16     ; PU - Activar

    loop:
        IN      R16,    PIND
        OUT     PORTB,  R16

        rjmp    loop
