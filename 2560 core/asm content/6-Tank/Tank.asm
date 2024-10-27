    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

    ; Se lee el valor del PINA
    ; Se muestra el tanque según la física clásica
main:
    ; Setear puertos de entrada A
    LDI     R16,    0x00
    OUT     DDRA,   R16

    ; Setear puertos de salida C
    LDI     R16,    0xFF
    OUT     DDRC,   R16

loop:
    LDI     R18,    0x00
    IN      R17,    PINA

    ; 0000 1000 -> Bit encendido : Saltamos. Esta apagado, no salta (ejecuta)
    SBRC    R17,    3       ; Skip if Bit Reg is Cleared=0
    RJMP    up_3

    SBRC    R17,    2       ; Si está apagado lo salta, está encendido y brinca
    RJMP    up_2

    SBRC    R17,    1
    RJMP    up_1

    SBRC    R17,    0       ; Skip Bit Regi. Clear=0
    RJMP    up_0

    RJMP    down_0          ; Si lo estamos vamos a todo_bajo

up_3:
    ORI     R18,    0x8F    ; Séptimo bit -> parpadeo bomba / máximo
    OUT     PORTC,  R18
    RCALL   delay

    ANDI    R18,    0x7F
    OUT     PORTC,  R18
    RCALL   delay

    LDI     R19,    0x00

    RJMP    loop

up_2:
    ORI     R18,    0x07
    OR      R18,    R19
    OUT     PORTC,  R18
    RJMP    loop

up_1:
    ORI     R18,    0x03
    OR      R18,    R19
    OUT     PORTC,  R18
    RJMP    loop

up_0:
    ORI     R18,    0x01
    OR      R18,    R19
    OUT     PORTC,  R18

    RJMP    loop

down_0:
    LDI     R19,    0x40
    OR      R18,    R19     ; Mínimo
    OUT     PORTC,  R18
    RJMP    loop

delay:
    ; Subrutina de Retraso (1seg)
    LDI     R25,    82
    LDI     R26,    43
    LDI     R27,    0
L1:
    DEC     R27
    BRNE    L1
    DEC     R26
    BRNE    L1
    DEC     R25
    BRNE    L1
    LPM
    NOP
    RET
