    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

; Se realizará un contador del 0 al 9 (con loop) en un display de 7 segmentos ánodo común, los dígitos serán escritos en memoria Flash, leídos y mostrados por el Puerto A.

main:
    ; Configurar el puerto A como salida (para el display de 7 segmentos)
    LDI     R16,    0xFF
    OUT     DDRA,   R16

reset:
    ; Iniciar contador y puntero Z
    LDI     R17,    0
    LDI     ZH,     0x01
    LDI     ZL,     0x00
    ; Multiplicamos por 2 para acceder correctamente
    LSL     ZL
    ROL     ZH

loop:
    ; Mostrar el dígito
    LPM     R16,    Z+
    OUT     PORTA,  R16
    RJMP    delay

loop_delay:
    ; Revisamos si recorrimos todos los dígitos (0 a 9)
    INC     R17
    CPI     R17,    10
    BRNE    loop

    RJMP    reset

delay:
    ; Subrutina de retraso (1 seg)
    LDI     R20,    82
    LDI     R21,    43
    LDI     R22,    255
L1:
    DEC     R22
    BRNE    L1
    DEC     R21
    BRNE    L1
    DEC     R20
    BRNE    L1
    RJMP    loop_delay

table:
    ; Tabla de dígitos en ánodo común
    .org(0x100)
    .dw 0xF9C0, 0xB0A4, 0x9299, 0xF882, 0x9080