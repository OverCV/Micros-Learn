    ; ATMega2560 Light Sequence Control Program
    ; Este programa controla secuencias de luces basadas en la entrada del usuario

    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

main:
    ; Configuración de puertos
    LDI     R17,                    0x00
    OUT     DDRA,                   R17     ; Puerto A como entrada
    LDI     R17,                    0xFF
    OUT     DDRC,                   R17     ; Puerto C como salida

    ; Inicialización de variables
    CLR     R18
    LDI     R19,                    0x08
    LDI     R20,                    0x10

loop:
    ; Lectura de entrada y selección de secuencia
    IN      R16,                    PINA
    CPI     R16,                    0x01
    BREQ    blink_sequence
    CPI     R16,                    0x02
    BREQ    reset_inout
    CPI     R16,                    0x03
    BREQ    shift_sequence
    RJMP    loop

    ; Secuencia de parpadeo
blink_sequence:
    CLR     R18                             ; Resetear estado
    OUT     PORTC,                  R18
    RCALL   delay
    COM     R18                             ; Complementar para parpadeo
    OUT     PORTC,                  R18
    RCALL   delay
    RJMP    loop

    ; Secuencia de expansión desde el centro
reset_inout:
    LDI     R19,                    0x08    ; Resetear a posición central
    LDI     R20,                    0x10

in_out:
    MOV     R18,                    R19
    OR      R18,                    R20
    OUT     PORTC,                  R18
    RCALL   delay
    IN      R16,                    PINA    ; Verificar cambio de entrada
    CPI     R16,                    0x02
    BRNE    loop
    CPI     R18,                    0x81    ; Verificar si llegó a los extremos
    BREQ    out_in
    LSR     R19                             ; Expandir hacia afuera
    LSL     R20
    RJMP    in_out

out_in:
    MOV     R18,                    R19
    OR      R18,                    R20
    OUT     PORTC,                  R18
    RCALL   delay
    IN      R16,                    PINA    ; Verificar cambio de entrada
    CPI     R16,                    0x02
    BRNE    loop
    CPI     R18,                    0x18    ; Verificar si llegó al centro
    BREQ    reset_inout
    LSL     R19                             ; Contraer hacia el centro
    LSR     R20
    RJMP    out_in

    ; Secuencia de desplazamiento
shift_sequence:
    LDI     R18,                    0x80    ; Iniciar en el extremo derecho
shift_right:
    OUT     PORTC,                  R18
    RCALL   delay
    IN      R16,                    PINA    ; Verificar cambio de entrada
    CPI     R16,                    0x03
    BRNE    loop
    LSR     R18
    BRCC    shift_right                     ; Continuar si no llegó al extremo izquierdo
shift_left:
    LDI     R18,                    0x01    ; Reiniciar en el extremo izquierdo
shift_left_continue:
    OUT     PORTC,                  R18
    RCALL   delay
    IN      R16,                    PINA    ; Verificar cambio de entrada
    CPI     R16,                    0x03
    BRNE    loop
    LSL     R18
    BRCC    shift_left_continue             ; Continuar si no llegó al extremo derecho
    RJMP    shift_sequence

delay:
    ; Subrutina de Retraso (1seg)
    LDI     R25,                    82
    LDI     R26,                    43
    LDI     R27,                    0
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
