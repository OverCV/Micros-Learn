    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

    .org        OC0Aaddr
    RJMP    isr_t0

main:
    ; Habilitar interrupciones globales
    SEI

    ; Configurar Puerto A como salida y Puerto D como entrada
    LDI     R16,            0xFF                        ; Puerto A como salida
    OUT     DDRA,           R16
    OUT     DDRC,           R16

    ; Configurar Timer0 en modo CTC (Clear Timer on Compare Match)
    LDI     R16,            0x02                        ; WGM01 = 1, modo CTC
    OUT     TCCR0A,         R16

    ; Establecer el valor máximo en OCR0A (contar hasta 250)
    LDI     R16,            125
    OUT     OCR0A,          R16                         ; Configurar el valor de comparación en OCR0A

    ; Configurar el prescaler en 1024 (CS02 = 1, CS01 = 0, CS00 = 1)
    LDI     R16,            0x05                        ; Prescaler de 1024
    OUT     TCCR0B,         R16

    ; Habilitar la interrupción de comparación para el Timer0 (OCIE0A)
    LDI     R16,            0x02                        ; Habilitar interrupción de comparación (OCIE0A)
    STS     TIMSK0,         R16

    ; Inicializar el contador de interrupciones
    LDI     R20,            0                           ; Contador de interrupciones
    LDI     R17,            125                          ; 125 interrupciones para contar 1 segundo

reset:
    ; Iniciar contador y puntero Z
    LDI     ZH,             0x00
    LDI     ZL,             0x60
    ; Multiplicamos por 2 para acceder correctamente
    LSL     ZL
    ROL     ZH

loop:
    LPM     R20,            Z+
    LPM     R21,            Z
    DEC     ZL

    ; COM     R20
    ; COM     R21

    CPI     ZL,             0xC8
    BREQ    reset

    OUT     PORTA,          R20
    OUT     PORTC,          R21

    RJMP    loop

isr_t0:
    DEC     R17                                         ; Decrementar el contador de interrupciones
    BRNE    reti_isr_t0
    LDI     R17,            125

    ; Incrementar puntero
    INC     ZL
    INC     ZL
    ; COM     R20


reti_isr_t0:
    RETI

.org(0x0060)
table:
    ; Tabla de datos
    ; .dw  'o', 'v', 'e', 'r'
    .dw  0xFFC0, 0xEECF, 0xDFC6, 0xD9CC

    ;     NM.LKJH GPFE.DCBA
    ; Z XX11.1111 1111.1111

    ; O = C0FF
    ; V = CFEE
    ; E = C6DF
    ; R = CCD9