    .include"m328Pdef.inc"
    .org(0x0000)
    rjmp    main

    ; Ejecución de un semáforo
main:
    ; pull-up salidas
    LDI     R16,        0xFF
    OUT     DDRB,       R16     ; Puerto B salida

    ; Inicializar puntero de pila en 16 bits
    LDI     R17,        0x08
    OUT     SPL,        R16

    OUT     SPH,        R17

loop:
    SBI     PORTB,      2       ; Activar LED Verde
    RCALL   delay3_             ; subrutina +3 segundos
    CBI     PORTB,      2       ; Desactivar LED Verde

    RCALL   Blink
    RCALL   Blink
    RCALL   Blink

    SBI     PORTB,      1
    RCALL   delay3_
    CBI     PORTB,      1       ; Activar LED Amarillo

    SBI     PORTB,      0       ; Desactivar LED Amarillo
    RCALL   delay3_
    RCALL   delay3_
    CBI     PORTB,      0       ; Activar LED Amarillo

    rjmp    loop

blink:
    SBI     PORTB,      2
    RCALL   Delay1_5            ; RCALL 3 ciclos + RET 4 ciclos
    CBI     PORTB,      2
    RCALL   Delay1_5
    RET

delay1_5:
    ldi     r18,        3
    ldi     r19,        138
    ldi     r20,        84
L1:
    dec     r20
    brne    L1
    dec     r19
    brne    L1
    dec     r18
    brne    L1
    nop
    RET

delay3_:
    ldi     r18,        16
    ldi     r19,        57
    ldi     r20,        12
L2:
    dec     r20
    brne    L1
    dec     r19
    brne    L1
    dec     r18
    brne    L1
    nop
    RET