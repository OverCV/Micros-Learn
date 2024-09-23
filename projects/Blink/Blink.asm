    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

main:
    ; Configurar puerto de salida
    LDI     R16,    0xFF
    OUT     DDRC,   R16

loop:
    ; Encender los leds
    LDI     R16,    0xFF
    OUT     PORTC,  R16

    RCALL   delay

    ; Apagar los leds
    LDI     R16,    0x00
    OUT     PORTC,  R16

    RCALL   delay

    RJMP    loop

/*
Subrutina de retraso
decidimos decrementar un contador hasta que llegue a cero,de forma que cuando llegue a cero se decremente y se vaa resetear el contador (puesto no puede ser inferior a 0).
Aún seguimos calculando los valores por asignar a los contadores de cada ciclo.
*/
delay:
    LDI     R20,    255
L1:
    DEC     R20
    BRNE    L1
    RET
