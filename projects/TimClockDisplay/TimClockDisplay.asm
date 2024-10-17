    .include    "m2560def.inc"

    .org        (0x0000)
    RJMP    main

    ; Etiqueta para interrupción del Timer0 Compare Match A (0x002A)
    .org        OC0Aaddr
    RJMP    isr_t0

/*
Se pone el Timer Control Register 0A (TCCR0A) en 0x10 para activar el WGM01, ya activado, el OC0A (Compare Match 0A) (permite asignar un 250) que ante la comparación con este tope reiniciará el contador (+generando la interrupción OC0Aaddr).
Luego miramos el TCCR0B, el que permite el prescaling porque tiene los bits del CS00 CS01 CS02 que en valor de 0x05 dan un prescaling de 1024, luego pues activamos el OC0A con el TIMSK0 porque el bit asociado es el 1 (activado con OCIE0A: On Compare Interrupt Enable 0A) y luego podríamos habilitar las interrupciones globales con SEI pero pues ya lo hicimos al inicio.

revisar en `assets\atmel-2549-8-bit-avr-microcontroller-atmega640-1280-1281-2560-2561_datasheet.pdf` para más información.
*/

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
    LDI     R17,            125                          ; 64 interrupciones para contar 1 segundo

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
    LPM     R20,            Z

    CPI     ZL,             0xCA
    BREQ    reset

    OUT     PORTA,          R20

    RJMP    loop

; decremtar el registro y la comparacion es por fuera
    ; Rutina de interrupción para Timer0 Compare Match A
isr_t0:
    DEC     R17                                         ; Decrementar el contador de interrupciones
    BRNE    reti_isr_t0                                 ; Si no es 0 salimos de la interrupción

    ; Si se alcanzaron 125 interrupciones, incrementar contador de segundos
    LDI     R17,            125                          ; Reiniciar el contador de interrupciones

    ; Incrementar el contador de segundos
    INC     ZL

reti_isr_t0:
    RETI

table:
    ; Tabla de dígitos 0-9 en ánodo común
    .org        (0x0060)
    .dw         (0x7940), 0x3024, 0x1219, 0x7802, 0x1000