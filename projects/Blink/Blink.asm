    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

main:
    ; Código aquí 🤗
    LDI     R16,    0x00
    OUT     DDRA,   R16     ; PD - Entradas

    LDI     R16,    0xFF
    OUT     DDRF,   R16     ; PU - Salidas

    OUT     PORTA,  R16     ; Activar entradas

loop:
    IN      R16,    PINA
    OUT     PORTF,  R16

    RJMP    loop

; Tomar dos datos de la memoria de datos, sumalos y almacenar el resultado en la memoria de datos