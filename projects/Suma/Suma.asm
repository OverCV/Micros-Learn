    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

main:
    LDI     R16,    0x01
    LDI     R17,    0x01    ; Leer los datos

    STS     0x0100,     R16
    STS     0x0101,     R17     ; Almacenar en memoria RAM

    LDS     R18,     0x0100
    LDS     R19,     0x0101     ; Leer los registros

    ADD     R18,     R19      ; Suma de los valores en memoria

    LDI     R19,     0xFF
    OUT     DDRF,    R19      ; Activar las salidas

loop:
    OUT     PORTF,  R18      ; Mostrar la salida

    RJMP    loop
