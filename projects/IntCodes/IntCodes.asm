    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

    .org(0x0002)
    RJMP    rsi_0

    .org(0x0004)
    RJMP    rsi_1

main:
    ; Salida: Puerto A  (Display 7 Segmentos)
    LDI     R16,    0xFF    ; (1111 1111)
    OUT     DDRA,   R16

    ;! Habilitamos interrupciones globales
    SEI
    ; Habilitar INT_0, INT1
    LDI     R17,    0x01
    OUT     EIMSK,  R17
    ; Configurar con cambio de nivel (conf=00)
    LDI     R18,    0x00
    STS     EICRA,  R18

    RJMP    reset

loop:
    LPM     R16,    Z+

    OUT     PORTA,  R16
    RCALL   delay

    SBRC    R16,    7
    RJMP    reset

    RJMP    loop

set_seq:
    ; read ptr X | Y

set_dir:
    ; (ASC inc | DESC dec) R30
    ; RJMP    loop

reset:
    ; Iniciar puntero Z (contador)
    LDI     ZH,     0x00
    LDI     ZL,     0x80
    ; Multiplicamos por 2 para acceder correctamente
    LSL     ZL
    ROL     ZH
    RJMP    loop

delay:
    ; Subrutina de retraso (1 seg)
    LDI     R20,    82
    LDI     R21,    43
    LDI     R22,    50
L1:
    DEC     R22
    BRNE    L1
    DEC     R21
    BRNE    L1
    DEC     R20
    BRNE    L1
    ; RJMP    loop
    RET

rsi_0:
    RETI

rsi_1:
    RETI

table_cod:
    .org(0x0080)


table_cc:
    .org(0x0100)
    .dw 0x4079, 0x2440, 0x1902, 0x4030, 0x2479