    .include    "m2560def.inc"

    .org        (0x0000)
    RJMP    main

    ; interrupt service routines
    .org        (INT0addr)
    RJMP    isr_0

    .org        (INT1addr)
    RJMP    isr_1

    ; timer/counter compare match
    .org        (OC0Aaddr)
    RJMP    tisr_0

    ; Diseñar un programa que combinando dos bits:
    ; b0 = 0: Ascendente.  b0 = 1: Descendente.
    ; b1 = 0: Leer cédula. b1 = 1: Leer nombre.

    ; El tiempo debe estar cronometrado en medio segundo con el timer en modo reloj.
    ; La entrada debe ser activada por interrupción.
    ; Deben almacenarsen los valores en la memora del programa.

    .equ        ONE_SEC = 70

main:
    ; Definición entradas o salidas
    LDI     R16,            0xFF
    OUT     DDRA,           R16
    OUT     DDRC,           R16

    ; Vector de estado
    LDI     R25,            0x00

    ; Activar Interrupciones Globales
    SEI

timer_clock_conf:
    ; Configurar Timer0 en modo CTC (Clear Timer on Compare Match)
    LDI     R16,            0x02                        ; WGM01 = 1, modo CTC
    OUT     TCCR0A,         R16
    ; Establecer el valor máximo en OCR0A
    LDI     R16,            ONE_SEC
    OUT     OCR0A,          R16                         ; Valor de comparación en OCR0A
    ; Configurar el prescaler en 1024 (CS02 = 1, CS01 = 0, CS00 = 1)
    LDI     R16,            0x05
    OUT     TCCR0B,         R16
    ; Habilitar la interrupción de comparación para el Timer0 (OCIE0A)
    LDI     R16,            0x02                        ; Habilitar interrupción de comparación (OCIE0A)
    STS     TIMSK0,         R16
    ; Inicializar el contador de interrupciones
    LDI     R17,            ONE_SEC
    LDI     R19,            0

interruptions_conf:
    ; Activar INT(0, 1)
    LDI     R16,            0x03
    OUT     EIMSK,          R16
    ; Configurar detección de cambio
    LDI     R16,            0x0F
    STS     EICRA,          R16

reset_table:
    SBRC    R25,            1
    RJMP    reset_y

reset_x:
    LDI     ZH,             HIGH(table_name    *  2)

    SBRC    R25,            0
    LDI     ZL,             LOW(table_name     *  2) + 6
    SBRS    R25,            0
    LDI     ZL,             LOW(table_name     *  2)

    RJMP    loop

reset_y:
    LDI     ZH,             HIGH(table_nums    *  2)

    SBRC    R25,            0
    LDI     ZL,             LOW(table_nums     *  2) + 8
    SBRS    R25,            0
    LDI     ZL,             LOW(table_nums     *  2)

    RJMP    loop

loop:
    ; Mantenemos estable la lectura en Z
    LPM     R20,            Z+
    LPM     R21,            Z+
    SUBI    ZL,             2

    ; comparar si se salió y consultar vector de estado
    SBRC    R21,            7
    RJMP    reset_table

    ; Guardado en tabla para Cátodo Común
    COM     R20
    COM     R21

    OUT     PORTA,          R20
    OUT     PORTC,          R21

    RJMP    loop

isr_0:
    LDI     R16,            0x01
    EOR     R25,            R16
    RETI

isr_1:
    LDI     R16,            0x02
    EOR     R25,            R16
    RETI

tisr_0:
    DEC     R17
    BRNE    tisr_reti
    LDI     R17,            ONE_SEC

    LDI     R16,            2

    SBRC    R25,            0
    SUB     ZL,             R16

    SBRS    R25,            0
    ADD     ZL,             R16

tisr_reti:
    RETI

    .org        (0x0100)
table_name:
    ; Nombre = OVER
    .dw         0x003F, 0x1130, 0x2239, 0x2633

    .org        (0x0200)
table_nums:
    ; Número = 20396
    .dw         0x221B, 0x003f, 0x220f, 0x2227, 0x223D