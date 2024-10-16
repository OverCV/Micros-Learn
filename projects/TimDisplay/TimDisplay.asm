    .include    "m2560def.inc"

    .org        (0x0000)
    RJMP    main

    ; Usar la etiqueta para la interrupción del Timer0 Compare Match A (OC0Aaddr)
    .org        OC0Aaddr
    RJMP    isr_t0

main:
    ; Habilitar interrupciones globales
    SEI

    ; Configurar Puerto A como salida y Puerto D como entrada
    LDI     R16,            0xFF                        ; Puerto A como salida
    OUT     DDRA,           R16

    ; Configurar Timer0 en modo CTC (Clear Timer on Compare Match)
    LDI     R16,            0x02                        ; WGM01 = 1, modo CTC
    OUT     TCCR0A,         R16

    ; Establecer el valor máximo en OCR0A (contar hasta 250)
    LDI     R16,            250
    OUT     OCR0A,          R16                         ; Configurar el valor de comparación en OCR0A

    ; Configurar el prescaler en 1024 (CS02 = 1, CS01 = 0, CS00 = 1)
    LDI     R16,            0x05                        ; Prescaler de 1024
    OUT     TCCR0B,         R16

    ; Habilitar la interrupción de comparación para el Timer0 (OCIE0A)
    LDI     R16,            0x02                        ; Habilitar interrupción de comparación (OCIE0A)
    STS     TIMSK0,         R16

    ; Inicializar el contador de interrupciones
    LDI     R20,            0                           ; Contador de interrupciones
    LDI     R17,            64                          ; 64 interrupciones para contar 1 segundo

    ; Inicializar vector de estados
    LDI     R25,            0x00

reset:
    ; Iniciar contador y puntero Z
    LDI     ZH,             0x00
    LDI     ZL,             0x60
    ; Multiplicamos por 2 para acceder correctamente
    LSL     ZL
    ROL     ZH

loop:
    ; LDI     R20,            0x00
    IN      R21,            PIND

    LPM     R20,            Z

    CPI     ZL,             0xCA
    BREQ    reset

    ANDI    R21,            0x03
    OUT     PORTA,          R20                         ; Mostrar el valor del contador en el Puerto A
    RJMP    loop                                        ; Loop infinito


    ; Rutina de interrupción para Timer0 Compare Match A
isr_t0:
    DEC     R17                                         ; Decrementar el contador de interrupciones
    BRNE    reti_isr_t0                                 ; Si no es 0 salimos de la interrupción

    ; Si se alcanzaron 64 interrupciones, incrementar contador de segundos
    LDI     R17,            64                          ; Reiniciar el contador de interrupciones

    ; LOGIC
    INC     ZL                                          ; Incrementar el contador de segundos


reti_isr_t0:
    RETI                                                ; Retornar de la interrupción


table:
    .org        (0x0060)
    ; Tabla de dígitos 0-9 en ánodo común
    .dw         0x7940, 0x3024, 0x1219, 0x7802, 0x1000