            .include    "m2560def.inc"

            .org        (0x0000)
    RJMP    main

    ; interrupt service routines
            .org        (INT0addr)
    RJMP    isr_0


    ;.org (0x0022)
    ; timer/counter compare match
            .org        (OC1Aaddr)
    RJMP    tim_isr_1

    ; Constantes de retardo en unidades de comparación de temporizador
    ;.equ ONE_SEC = 62
    ;.equ TWO_SEC = 125
    ;.equ THREE_SEC = 187
            .equ        ONE_SEC = 4
            .equ        TWO_SEC = 8
            .equ        THREE_SEC = 12

main:
    ; Configurar los puertos como salida
    LDI     R16,        0xBF
    STS     DDRK,       R16
    OUT     DDRF,       R16

    ; Activar interrupciones globales
    SEI

interruptions_conf:
    ; Activar INT(0) para la interrupcion externa
    LDI     R16,        0x01
    OUT     EIMSK,      R16
    ; Configurar detección de flanco de bajada para INT0
    LDI     R16,        0x02
    STS     EICRA,      R16

timer_clock_conf:
    ; Configurar Timer1 en modo CTC con OCR1A como tope
    LDI     R16,        0x80
    STS     TCCR1A,     R16
    ; Establecer valor en OCR1A para el temporizador en modo CTC
    LDI     R16,        0x3D
    STS     OCR1AH,     R16
    LDI     R16,        0x09
    STS     OCR1AL,     R16
    ; Prescaler 1024, modo CTC
    LDI     R16,        0x0D
    STS     TCCR1B,     R16
    ; Habilitar la interrupcion de comparación para Timer1 (OCIE1A)
    LDI     R16,        0x02
    STS     TIMSK1,     R16
    LDI     R26,        0x01    ; adicionado
    LDI     R27,        0x01    ; adicionado

reset:
    ; Inicialización del contador de iteraciones extra
    LDI     R24,        2
    ; Inicializar puntero de tabla
    LDI     ZH,         HIGH(table_name    *  2)
    LDI     ZL,         LOW(table_name     *  2)

loop:
    ; Leer el siguiente valor de la tabla
    LPM     R20,        Z+
    LPM     R21,        Z+
    SUBI    ZL,         2
    ; Restaurar ZL a la posición inicial de la tabla si se sale del rango
    CPI     ZL,         LOW(table_name     *  2) + 8
    BREQ    reset

    ; Invertir y enviar valores a los puertos
    COM     R20
    COM     R21
    STS     PORTK,      R20
    OUT     PORTF,      R21

    ; Leer y combinar entradas de PORTK y PORTF en bit 6
    ;LDS R19, PORTK
    LDS     R19,        PINK
    ANDI    R19,        0x40
    IN      R18,        PINF
    ANDI    R18,        0x40

    SWAP    R18
    OR      R18,        R19

    ; Seleccionar el tiempo de retardo según las entradas combinadas
    LDI     R24,        2
    CPI     R18,        0x00
    BREQ    set_00
    CPI     R18,        0x04
    BREQ    set_01
    CPI     R18,        0x40
    BREQ    set_10
    CPI     R18,        0x44
    BREQ    set_11

    RJMP    loop

set_00:
    LDI     R28,        0
    RJMP    loop
set_01:
    LDI     R28,        ONE_SEC
    RJMP    loop
set_10:
    LDI     R28,        TWO_SEC
    RJMP    loop
set_11:
    LDI     R28,        THREE_SEC
    RJMP    loop

isr_0:
    ; Al activarse esta interrupcion, se configuran 3 iteraciones adicionales
    LDI     R16,        3
    MOV     R27,        R16
    RETI

tim_isr_1:
    ; Reducir el contador de tiempo `R26` en cada interrupcion de temporizador
    CPI     R28,        0
    BREQ    tim_reti
    DEC     R26
    
    ; Si no ha llegado a cero, salir de la interrupcion
    BRNE    tim_reti                                        
    DEC     R27

    ; Si hay iteraciones extra, salir
    BRNE    reset_period
    LDI     R27,        0x01                                ; añadido por Nubis
    ; Incrementar la posición de la tabla una vez completadas todas las iteraciones
    ADD     ZL,         R24

reset_period:
    MOV     R26,        R28                     ; añadido por Nubis

tim_reti:
    RETI

    .org        (0x0200)
table_name:
    ; Tabla de datos
    .dw         0x3F00, 0x18A0, 0x9D00, 0x9E11


/*

R16 <- 0xFF (+1 => 0x00)
BIT: 8 7654 3210
BIN:   1111 1111 -> 255
BIN: 1 0000 0000 -> 256 = 2^8
HEX:   F    F

DEC: 255

*/