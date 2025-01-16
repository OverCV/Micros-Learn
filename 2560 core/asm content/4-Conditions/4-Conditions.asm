    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

/**
    Programa que recibe dos datos como entrada por el puerto A y el puerto B decide si se realiza una suma, resta, producto o división entre dos.
*/
main:
    LDI     R16,    0x00
    OUT     DDRB,   R16
    OUT     DDRA,   R16
    
    COM     R16
    OUT     DDRC,   R16

    MOV     R20,    R16

loop:
    OUT     PORTC,  R17
    IN      R17,    PINA
    IN      R19,    PINB

    ; Registro salida
    MOV     R20,    R17

    ; separación de datos
    MOV     R18,    R17
    SWAP    R18
    ANDI    R17,    0x0F     
    ANDI    R18,    0x0F

    CPI     R19,    0       ; 0 = 0b0000.0001 = 0x01
    BREQ    sum

    CPI     R19,    1       ; 2 = 0b0000.0002 = 0x02
    BREQ    subs

    CPI     R19,    2       ; 3 = 0b0000.0003 = 0x03
    BREQ    prod

    RJMP    half

sum:
    ADD     R17,    R18
    RJMP    loop

subs:
    SUB     R17,    R18
    RJMP    loop

prod:
    ; ADD     R17,    R17
    ; SUBI    R18,    1

    ; BREQ    loop
    ; RJMP    prod
    MUL     R17,    R18
    RJMP    loop

half:
    LSR     R20
    MOV     R17,    R20
    RJMP    loop

