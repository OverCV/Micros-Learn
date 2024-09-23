    .include"m328Pdef.inc"

    .org(0x0000)
    RJMP    main

    ; Diseño para configurar TIMER0 con oscilador externo, sensible a flancos de subida.
    ; Muestra por Puerto B mediante conteo los eventos ocurridos (por registro TCNT0).
main:
    LDI     R16,    0xFF    ; PU - Salida
    OUT     DDRB,   R16

    LDI     R17,    0x07    ; External clock source on T0 pin. Clock on rising edge
    OUT     TCCR0B, R17     ; TCCR0B – Timer/Counter Control Register B

    LDI     R0,    0x00
    STS     TIMSK0, R0     ; TIMSK0 -> Memoria extendida

    OUT     TCNT0,  R0     ; Inicio de contador

loop:
    IN      R1,    TCNT0
    OUT     PORTB,  R1

    ; No hay incremento puesto se actualiza con los pulsos del 555.

    RJMP    loop

    ; Cálculos Simulación ;
    ; Tiempo Estado Alto `T0 = 0.693(R1+R2)C2 = 0.98 seg`
    ; Tiempo Ciclo Completo `T2 = 0.693(R2)C2 = 0.5 seg`
    ; Tiempo Ciclo Completo `T = T1+T2 = 1.48 seg`
    ; Frecuencia `F = 1/T = 1.68 Hz`