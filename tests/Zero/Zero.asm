    .include    "m2560def.inc"

    .org    (0x0000)
    RJMP    main

    ; interrupt service routines
    .org    (INT0addr)
    RJMP    isr_0

    .org    (INT1addr)
    RJMP    isr_1

    ; timer/counter compare match
    .org    (OC0Aaddr)
    RJMP    tisr_0

    ; Diseñar un programa que combinando dos bits:
    ; b0 = 0: Ascendente.  b0 = 1: Descendente.
    ; b1 = 0: Leer cédula. b1 = 1: Leer nombre.

    ; El tiempo debe estar cronometrado en medio segundo con el timer en modo reloj.
    ; La entrada debe ser activada por interrupción.
    ; Deben almacenarsen los valores en la memora del programa.

    .equ    PROG_MEM_DIR = 0x0100
    .equ    ONE_SEC      = 125

main:
    ; Definición entradas o salidas
    LDI     R16,    0xFF
    OUT     DDRA,   R16
    OUT     DDRC,   R16

timer_clock_conf:
    ; Configurar Timer0 en modo CTC (Clear Timer on Compare Match)
    LDI     R16,    0x02                ; WGM01 = 1, modo CTC
    OUT     TCCR0A, R16
    ; Establecer el valor máximo en OCR0A
    LDI     R16,    ONE_SEC
    OUT     OCR0A,  R16                 ; Valor de comparación en OCR0A
    ; Configurar el prescaler en 1024 (CS02 = 1, CS01 = 0, CS00 = 1)
    LDI     R16,    0x05
    OUT     TCCR0B, R16
    ; Habilitar la interrupción de comparación para el Timer0 (OCIE0A)
    LDI     R16,    0x02                ; Habilitar interrupción de comparación (OCIE0A)
    STS     TIMSK0, R16
    ; Inicializar el contador de interrupciones
    LDI     R17,    ONE_SEC

interruptions_conf:


reset:
    ; Inicializar

loop:
    RJMP    loop

isr_0:
    RETI

isr_1:
    RETI

tisr_0:
    DEC     R17                                         ; Decrementar el contador de interrupciones
    BRNE    tisr_reti
    LDI     R17,            62

tisr_reti:
    RETI

    .org        (PROG_MEM_DIR)
table_name:
    .dw 0x003F, 0x1130, 0x2239, 0x2633

table_numbers:
    .dw 0x220f, 0x0006
    ; .dw 0x003f, 0x221b, 0x220f, 0x2227, 0x223d
