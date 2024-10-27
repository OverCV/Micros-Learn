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
    LDI     R17,            62                          ; 125 interrupciones para contar 1 segundo

reset:
    ; Iniciar contador y puntero Z
    LDI     ZH,             0x00
    LDI     ZL,             0x60
    ; Multiplicamos por 2 para acceder correctamente
    LSL     ZL
    ROL     ZH

    LDI     R16,            52
    MOV     R25,            ZL
    ADD     R25,            R16

loop:
    LPM     R20,            Z+
    LPM     R21,            Z
    DEC     ZL

    COM     R20
    COM     R21

    CP      ZL,             R25
    BREQ    reset

    OUT     PORTA,          R20
    OUT     PORTC,          R21

    RJMP    loop

isr_t0:
    DEC     R17                                         ; Decrementar el contador de interrupciones
    BRNE    reti_isr_t0
    LDI     R17,            62

    ; Incrementar puntero
    INC     ZL
    INC     ZL

reti_isr_t0:
    RETI

.org(0x0060)
table:
    ; Tabla de datos

; A: 0x10001000110111
; B: 0x00101010001111
; C: 0x00000000111001
; D: 0x00100010001111
; E: 0x10001000111001
; F: 0x10001000110001
; G: 0x00001000111101
    .dw 0x2237, 0x0A8F, 0x0039, 0x088F, 0x2239, 0x2231, 0x023D

; H: 0010001000110110
; I: 0000100010001001
; J: 0000000000011110
; K: 0010010100110000
; L: 0000000000111000
; M: 0000000101110110
; N: 0000010001110110
    .dw 0x2236, 0x0889, 0x001e, 0x2530, 0x0038, 0x0176, 0x0476

; O: 0x00000000111111
; P: 0x10001000110011
; Q: 0x00010000111111
; R: 0x10011000110011
; S: 0x00001001001101
; T: 0x00100010000001
; U: 0x00000000111110
    .dw 0x003F, 0x2233, 0x043F, 0x2633, 0x024D, 0x0881, 0x003E

; V: 0x01000100110000
; W: 0x01010000110110
; X: 0x01010101000000
; Y: 0x00100101000000
; Z: 0x01000100001001
    .dw 0x1130, 0x1436, 0x1540, 0x0940, 0x1109




