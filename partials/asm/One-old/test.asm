    .include    "m2560def.inc"

    .org        (0x0000)
    RJMP    main

    ; interrupt service routines
    .org        (INT0addr)
    RJMP    isr_0

    ; timer/counter compare match
    .org        (OC0Aaddr)
    RJMP    tim_isr_0

    .equ        ONE_SEC = 61
    .equ        TWO_SEC = 125
    .equ        THREE_SEC = 255

main:
    ; Definición entradas o salidas
    LDI     R16,            0xBF
    STS     DDRF,           R16
    STS     DDRK,           R16

    ; Vector de estado
    ; LDI     R25,            0x00

    ; Activar Interrupciones Globales
    SEI

timer_clock_conf:
    ; Configurar Timer0 en modo CTC (Clear Timer on Compare Match)
    LDI     R16,            0x02                        ; WGM01 = 1, modo CTC
    OUT     TCCR0A,         R16
    ; Establecer el valor máximo en OCR0A
    LDI     R16,            TWO_SEC
    OUT     OCR0A,          R16                         ; Valor de comparación en OCR0A
    ; Configurar el prescaler en 1024 (CS02 = 1, CS01 = 0, CS00 = 1)
    LDI     R16,            0x05
    OUT     TCCR0B,         R16
    ; Habilitar la interrupción de comparación para el Timer0 (OCIE0A)
    LDI     R16,            0x02                        ; Habilitar interrupción de comparación (OCIE0A)
    STS     TIMSK0,         R16
    ; Inicializar el contador de interrupciones
    LDI     R17,            TWO_SEC
    ; LDI     R19,            0

interruptions_conf:
    ; Activar INT(0)
    LDI     R16,            0x01
    OUT     EIMSK,          R16
    ; Configurar detección de cambio (bajada = 10)
    LDI     R16,            0x02
    STS     EICRA,          R16

reset:
    LDI     ZH,             HIGH(table_name    *  2)
    LDI     ZL,             LOW(table_name     *  2)

loop:
    ; Mantenemos estable la lectura en Z
    LPM     R20,            Z+
    LPM     R21,            Z+
    SUBI    ZL,             2

    ; comparar si se salió y consultar vector de estado
    SBRC    R21,            6
    RJMP    reset

    ; Guardado en tabla para Cátodo Común
    COM     R20
    COM     R21

    STS     PORTF,          R20
    STS     PORTK,          R21

    RJMP    loop

isr_0:
    ; LDI     R16,            0x01
    ; EOR     R25,            R16
    RETI

tim_isr_0:
    DEC     R17
    BRNE    tisr_reti
    LDI     R17,            TWO_SEC

    LDI     R16,            2

    ; SBRC    R25,            0
    ; SUB     ZL,             R16

    ; SBRS    R25,            0
    ADD     ZL,             R16

tisr_reti:
    RETI

.org(0x0100)
table_name:
    ; Tabla de datos
    .dw 0x3F00, 0x9880, 0x9D00, 0x9E11