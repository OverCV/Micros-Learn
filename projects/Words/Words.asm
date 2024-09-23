/*
Integrantes:
    Juan Carlos Saldarriaga Urrea
    Over Haider Castrillón Valencia
*/

    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

main:
    ; Cargar las palabras en memoria
    LDI     R16,    'a'
    STS     0x0200, R16
    LDI     R16,    'B'
    STS     0x0201, R16
    LDI     R16,    'c'
    STS     0x0202, R16
    LDI     R16,    'D'
    STS     0x0203, R16

    LDI     R17,    'a'
    STS     0x0210, R17
    LDI     R17,    'b'
    STS     0x0211, R17
    LDI     R17,    'C'
    STS     0x0212, R17
    LDI     R17,    'd'
    STS     0x0213, R17

    ; Setear puerto B como salida
    LDI     R25,    0xFF
    OUT     DDRB,   R25

reset:
    ; Cargamos X como 200H
    LDI     R27,    0x02
    LDI     R26,    0x00

    ; Cargamos Y como 210H
    LDI     R29,    0x02
    LDI     R28,    0x10

loop:
	; Cargamos los datos
    LD      R20,    X+
    LD      R21,    Y+

    MOV     R22,    R20
    OR      R22,    R21

	STS		0x0229,	R20
	STS		0x022A,	R21

    ; Si ambos son cero entonces hemos terminado de recorrer exitósamente
    CPI     R22,    0x00
    BREQ    leds_on

    ; Si R21 * R20 = 0 es porque alguno es 0. Son palabras diferentes
    MOV     R22,    R20
    AND     R22,    R21

    CPI     R22,    0x00
    BREQ    reset

    ; Hacemos la diferencia
	MOV		R22,	R20
    SUB     R22,    R21
    STS     0x0228, R22

    ; Comparar si son la misma en Upper o Lower (+/-20)
    CPI     R22,    0x20
    BREQ    loop

    CPI     R22,    0xe0
    BREQ    loop

    ; Comparar si es la misma letra (0)
    CPI     R22,    0x00
    BREQ    loop

    RJMP    reset

leds_on:
    LDI     R25,    0xFF
    OUT     PORTB,  R25
    LDI     R25,    0x00
    OUT     PORTB,  R25
    RJMP    reset