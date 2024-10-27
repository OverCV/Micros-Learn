    .include"m328Pdef.inc"
    .org(0x0000)
    RJMP    main

    ; Contador hasta el 255 en intervalos de 1 seg.
main:
    ; Activar salidas Puerto B (pull-up)
    LDI     R16,        0xFF
    OUT     DDRB,       R16

    ; Inicialzar contador
    LDI     R16,        0x00

loop:
    ; Visualizar valor
    OUT     PORTB,      R16
    INC     R16

    ; Delay 1 segundo
    RCALL   delay1_

    RJMP    loop

delay1_:
    ldi     r20,        130
    ldi     r21,        220
L1:
    dec     r21
    brne    L1
    dec     r20
    brne    L1
    RET

reset_sum:
    LDI     R16,        0x00
    RET