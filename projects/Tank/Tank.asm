    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

    ; Se lee el valor del PINC
    ; Se muestra el tanque según la física clásica
main:
    ; Setear puertos de salida B
    LDI     R16,    0xFF
    OUT     DDRB,   R16

    ; Setear puertos de entrada C
    LDI     R16,    0x00
    OUT     DDRC,   R16

    OUT     PORTB,  R16

loop:
    LDI     R18,    0x00
    IN      R17,    PINC

    ; 0000 1000 -> Bit encendido : Saltamos. Esta apagado, no salta (ejecuta)
    SBRC    R17,    3   ; Skip if Bit Reg is Cleared=0
    RJMP    up_3

    SBRC    R17,    2   ; Si está apagado lo salta, está encendido y brinca
    RJMP    up_2

    SBRC    R17,    1
    RJMP    up_1

    SBRC    R17,    0   ; Skip Bit Regi. Clear=0
    RJMP    up_0

    RJMP    down_0          ; Si lo estamos vamos a todo_bajo

up_3:
    ORI     R18,    0x8F    ; Séptimo bit -> parpadeo bomba / máximo
    ; ORI     R18,    R19
    OUT     PORTB,  R18

    ANDI    R18,    0x7F
    OUT     PORTB,  R18

    LDI     R19,    0x00

    RJMP    loop

up_2:
    ORI     R18,    0x07
    OR      R18,    R19
    OUT     PORTB,  R18
    RJMP    loop

up_1:
    ORI     R18,    0x03
    OR      R18,    R19
    OUT     PORTB,  R18
    RJMP    loop

up_0:
    ORI     R18,    0x01
    OR      R18,    R19
    OUT     PORTB,  R18

    RJMP    loop

down_0:
    LDI     R19,    0x40
    OR      R18,    R19    ; Mínimo
    OUT     PORTB,  R18
    RJMP    loop
