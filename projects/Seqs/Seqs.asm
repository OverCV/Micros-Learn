.include"m328Pdef.inc"
    .org(0x0000)
    RJMP    main

    ; Programa que consiste de 04 secuencias cíclicas.
    ; Si `D0, D1 = 0, 0` No se actúa.
    ; Si `D0, D1 = 1, 0` se alternan bits pares e impares encendidos y apagados
    ; Si `D0, D1 = 0, 1` se activa el patrón de centro a extremos

main:
    LDI     R16,                0x00
    OUT     DDRD,               R16         ; PORTD como entrada

    LDI     R16,                0xFF
    OUT     DDRB,               R16         ; PORTB como salida
    OUT     PORTD,              R16         ; Activar pull-ups en PORTD

    ; Inicializar patrones para center_to_edges
    LDI     R20,                0b00011000
    LDI     R21,                0b00011000

loop:
    IN      R16,                PIND
    ANDI    R16,                0x03        ; Nos interesa solo D0 y D1

    CPI     R16,                0x01
    BREQ    ten_five

    CPI     R16,                0x02
    BREQ    center_to_edges

    RJMP    loop                ; Si no es ninguno de los anteriores, volver al inicio

center_to_edges:
    MOV     R22,                R20         ; Copiar patrón actual a R22
    OR      R22,                R21         ; Combinar con el patrón espejo
    OUT     PORTB,              R22         ; Mostrar en los LEDs
    RCALL   delay0_5

    LSR     R20                             ; Rotar derecha la parte alta
    LSL     R21                             ; Rotar izquierda la parte baja

    MOV     R23,                R20
    OR      R23,                R21
    CPI     R23,                0b10000001  ; Comprobar si llegamos al final
    BRNE    center_to_edges

    ; Reiniciar el patrón
    LDI     R20,                0b00011000
    LDI     R21,                0b00011000
    RJMP    loop                ; Volver al loop principal después de completar la secuencia

ten_five:
    LDI     R16,                0xAA
    OUT     PORTB,              R16
    RCALL   delay0_5

    LDI     R16,                0x55
    OUT     PORTB,              R16
    RCALL   delay0_5

    RJMP    loop

delay0_5:
    ldi     r25,                3
    ldi     r26,                138
    ldi     r27,                84
L1:
    dec     r27
    brne    L1
    dec     r26
    brne    L1
    dec     r25
    brne    L1
    nop
    RET