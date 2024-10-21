    .include    "m2560def.inc"

    .org        (0x0000)
    RJMP    main

    ; Etiqueta de interrupción del Timer0 Compare Match A (OC0Aaddr)
    .org        (OC0Aaddr)
    RJMP    isr_t0

main:
    ; Habilitar interrupciones globales
    SEI

    ; Configurar Puerto A como salida y Puerto D como entrada
    LDI     R16,        0xFF    ; Puerto A como salida
    OUT     DDRA,       R16
    LDI     R16,        0xFC    ; Configurar solo PD0 y PD1 como entradas
    OUT     DDRD,       R16

    ; Configurar Timer0 en modo CTC (Clear Timer on Compare Match)
    LDI     R16,        0x02    ; WGM01 = 1, modo CTC
    OUT     TCCR0A,     R16

    ; Establecer el valor máximo en OCR0A (contar hasta 250)
    LDI     R16,        250
    OUT     OCR0A,      R16     ; Configurar el valor de comparación en OCR0A

    ; Configurar el prescaler en 1024 (CS02 = 1, CS01 = 0, CS00 = 1)
    LDI     R16,        0x05    ; Prescaler de 1024
    OUT     TCCR0B,     R16

    ; Habilitar la interrupción de comparación para el Timer0 (OCIE0A)
    LDI     R16,        0x02    ; Habilitar interrupción de comparación (OCIE0A)
    STS     TIMSK0,     R16

    ; Inicializar el contador de interrupciones
    LDI     R20,        0       ; Contador de interrupciones
    LDI     R17,        64      ; 64 interrupciones para contar 1 segundo

    ; Inicializar vector de estados
    LDI     R25,        0x00


/*
; R25: 0000 0000

; b0: Blink
; b0.0 = on, b0.1 = off

; b1,2: Left <-> Right
; b1.0 = ->, b1.1 = <-
; b1.0 = mantain, b1.1 = change
*/


loop:
    IN      R21,        PIND
    ANDI    R21,        0x03

    CPI     R21,        0x01

    CPI     R21,        0x02
    CPI     R21,        0x03
    INC     R20

    OUT     PORTA,      R20     ; Mostrar el valor del contador en el Puerto A
    RJMP    loop


    ; Rutina de interrupción para Timer0 Compare Match A
isr_t0:
    DEC     R17                 ; Decrementar el contador de interrupciones
    BRNE    reti_t0             ; Si no es 0 salimos de la interrupción

    ; Si se alcanzaron 64 interrupciones, incrementar contador de segundos
    LDI     R17,        64      ; Reiniciar el contador de interrupciones

    ; LOGIC
    LDI     R19,        0xFF
    EOR     R25,        R19


    ; Else count

reti_t0:
    RETI                        ; Retornar de la interrupción

blink:
    LDI     R20,        0xFF    ; Reseteo de la pantalla
    SBRC    R25,        0       ; ..0 -> on
    COM     R20

    ; XOR b0R23
    LDI     R19,        0x01
    EOR     R25,        R19

    RJMP    reti_t0


left:

    RJMP    reti_t0


right:
    RJMP    reti_t0


collapse:
    RJMP    reti_t0


expand:
    RJMP    reti_t0



