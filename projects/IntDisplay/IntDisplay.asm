    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

    .org(0x0006)
    RJMP    rsi_2

main:
    ; Configurar el puerto A como salida (para el display de 7 segmentos)
    LDI     R16,    0xFF
    OUT     DDRA,   R16

    ; Habilitamos interrupciones globales
    SEI

    ; Habilitar interrupciones (INT_2)
    LDI     R17,    0x04
    OUT     EIMSK,  R17

    ; Configurar con flanco de subida (bits 0, 1 activos)
    LDI     R18,    0x03
    STS     EICRA,  R18

reset:
    ; Iniciar contador y puntero Z
    LDI     ZH,     0x00
    LDI     ZL,     0x40
    ; Multiplicamos por 2 para acceder correctamente
    LSL     ZL
    ROL     ZH

loop:
    ; Mostrar el dígito
    LPM     R16,    Z
    OUT     PORTA,  R16

    ; Revisamos si recorrimos todos los dígitos (0 a 9)
    ; Si llegamos al décimo dígito, reiniciamos el contador
    SBRS    R16,    7
    RJMP    loop

    RJMP    reset

rsi_2:
    INC     R30
    RETI

table:
    ; Tabla de dígitos 0-9 en ánodo común
    .org(0x0040)
    .dw 0x7940, 0x3024, 0x1219, 0x7802, 0x1000