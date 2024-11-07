; Almacene en la memoria RAM usando direccionamiento indirecto el primer nombre, luego lea los datos o bits sobrantes del puerto F, K e interprételos como un tiempo entre 0 y 3 segundos usando el dipswitch, el nombre almacenado debe mostrarse secuencialmente en el display 14 segmentos por un periood igual al tiempo leído (periodo 0 es dejar la letra quieta)
; Adicionalmente por medio de una interrupción externa se conectará un pulsador que cuando tenga un flanco de bajada hará que la letra mostrada se vea durante los siguientes tres períodos de tiempo con el nombre.

; Cada segmento del display esta conectado a los pines de forma que:
/*
a f2
b f1
C F5
D F0
E F4
F F3
G F7

H K2
I K2
J K5
K K0
L K4
M K3
N K7
*/
    .include    "m2560def.inc"


    .org        (0x0000)
    RJMP    main

    ; interrupt service routines
    .org        (INT0addr)
    RJMP    isr_0

    ; timer/counter compare match
    .org        (OC1Aaddr)
    RJMP    tim_isr_1

    ; Constantes de retardo en unidades de comparación de temporizador
    .equ        ONE_SEC = 1
    .equ        TWO_SEC = 2
    .equ        THREE_SEC = 3

main:
    ; Configurar los puertos como salida
    LDI     R16,        0xBF
    STS     DDRK,       R16
    STS     DDRF,       R16

    ; Activar interrupciones globales
    SEI

interruptions_conf:
    ; Activar INT(0) para la interrupción externa
    LDI     R16,        0x01
    OUT     EIMSK,      R16
    ; Configurar detección de flanco de bajada para INT0
    LDI     R16,        0x02
    STS     EICRA,      R16

timer_clock_conf:
    ; Configurar Timer1 en modo CTC con OCR1A como tope
    LDI     R16,        0x00
    STS     TCCR1A,     R16
    ; Establecer valor en OCR1A para el temporizador en modo CTC
    LDI     R16,        0x3D
    STS     OCR1AH,     R16
    LDI     R16,        0x09
    STS     OCR1AL,     R16
    ; Prescaler 1024, modo CTC
    LDI     R16,        0x1D
    STS     TCCR1B,     R16
    ; Habilitar la interrupción de comparación para Timer1 (OCIE1A)
    LDI     R16,        0x02
    STS     TIMSK1,     R16

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
    CPI     ZL,         LOW(table_name     *  2) + 6
    BREQ    reset

    ; Invertir y enviar valores a los puertos
    COM     R20
    COM     R21
    STS     PORTK,      R20
    STS     PORTF,      R21

    ; Leer y combinar entradas de PORTK y PORTF en bit 6
    LDS     R19,        PORTK
    ANDI    R19,        0x40
    LDS     R18,        PORTF
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
    LDI     R26,        0
    RJMP    loop
set_01:
    LDI     R26,        ONE_SEC
    RJMP    loop
set_10:
    LDI     R26,        TWO_SEC
    RJMP    loop
set_11:
    LDI     R26,        THREE_SEC
    RJMP    loop

isr_0:
    ; Al activarse esta interrupción, se configuran 3 iteraciones adicionales
    LDI     R16,        3
    MOV     R27,        R16
    RETI

tim_isr_1:
    ; Reducir el contador de tiempo `R26` en cada interrupción de temporizador
    DEC     R26
    BRNE    tim_reti                                        ; Si no ha llegado a cero, salir de la interrupción

    ; Reiniciar R26 con el tiempo original
    MOV     R26,        R17
    ; Verificar si hay iteraciones extra en R27 (de la interrupción externa)
    DEC     R27
    BRNE    tim_reti                                        ; Si hay iteraciones extra, salir

    ; Incrementar la posición de la tabla una vez completadas todas las iteraciones
    ADD     ZL,         R24

tim_reti:
    RETI

    .org        (0x0200)
table_name:
    ; Tabla de datos
    .dw         0x3F00, 0x9880, 0x9D00, 0x9E11





; ABCDEFHIJKLMNG

    ; Tabla de datos
    .dw         0x3F00, 0x9880, 0x9D00, 0x9E11



    ;   N_CEFABD
    ;   7_543210

    ;   M_HKLPGJ
    ;   7_543210


    ; O -> ABCDEF -> 0x3F00

    ;   N_CEFABD
    ;   0_111111 ->  HEX: 3F
    ;   M_HKLPGJ
    ;   0_000000 ->  HEX: 00


    ; V -> EFMH -> 0x9880

    ; LOW()
    ;   N_CEFABD
    ;   1_011000 ->  HEX: 98

    ; HIGH()
    ;   M_HKLPGJ
    ;   1_000000 ->  HEX: 80


    ; E -> AFEDN -> 0x9D00

    ;   N_CEFABD
    ;   1_011101 ->  HEX: 9D

    ;   M_HKLPGJ
    ;   0_000000 ->  HEX: 00


    ; R -> ABJNFEK -> 0x9E11

    ;   N_CEFABD

    ;   1_011110 ->  HEX: 9E

    ;   M_HKLPGJ
    ;   0_010001 ->  HEX: 11

    ; .dw 0x3F00, 0x9880, 0x9D00, 0x9E11
