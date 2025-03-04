    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

main:
    ; Código aquí 🤗
    EOR     R16,    R16
    COM     R16
    OUT     DDRC,   R16


reset:
    LDI     R20,    0x80
    CLR     R19
    ; Inicializar

loop:
    RJMP    lazo_r


lazo_r:
    OUT     PORTC,  R20
    RCALL   delay
    LSR     R20

    CPI     R20,    0
    BREQ    pre_lazo_l

    RJMP    lazo_r

pre_lazo_l:
    LDI     R20,    0
    OUT     PORTC,  R20
    RCALL   delay
    LDI     R20,    1
    RJMP    lazo_l

lazo_l:
    OUT     PORTC,  R20
    RCALL   delay
    LSL     R20

    CPI     R20,    0x80
    BREQ    loop

    RJMP    lazo_l

delay:
    ldi     r27,    20
    ldi     r28,    150
    ldi     r29,    125
L1:
    dec     r29
    brne    L1
    dec     r28
    brne    L1
    dec     r27
    brne    L1
    rjmp    PC+1
	ret