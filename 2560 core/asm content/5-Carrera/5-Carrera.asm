    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main


/*

00: Vuelta en cero
01: Vuelta en ocho
*/

.def CNT1 = R25
.def CNT2 = R26
.def CNT3 = R27

main:
    ; Definir entradas
    LDI     R16,    0xCF
    OUT     DDRA,   R16

    ; Definir salida (D7S)
    LDI     R16,    0x7F
    OUT     DDRC,   R16

    LDI     R20,    0x01
    LDI     CNT1,   255
    LDI     CNT2,   50
    LDI     CNT3,   255

reset:

    ; Inicializar
    LDI     R20,    1

loop:
    IN      R17,    PINA    ; 1100 1111
    SWAP    R17             ; 1111 1100 
    ANDI    R17,    0x03    ; ____ __00

    ; SBRC
    ; SBRS

    CPI     R17,    0x01    ; 0000 0001
    BREQ    cero
    
    ; CPI     R17,    0x02    ; 0000 0010
    ; BREQ    ocho
    RJMP    ocho_ez

cero:

    ;   _000.0001
    ;   _000.0010
    ;   _000.0100
    ;   _000.1000

    ;   _10.0000
    OUT     PORTC,  R20
    RCALL   delay

    LSL     R20

    SBRS    R20,    6
    RJMP    cero

    RJMP    reset

ocho_ez:
    ; Estado del led
    ; 01, 02, 40, 20, 10, 08, 04, 40, 20
    ; A > B > G > E > D > C > G > F

    ; __GFE.DCBA
    ; __654.3210

    ; 

    ; _____.___A
    ; __000.0001
    LDI     R20,    0x01
    OUT     PORTC,  R20
    RCALL   delay

    ; _____.__B_
    ; __000.0010
    LDI     R20,    0x02
    OUT     PORTC,  R20
    RCALL   delay

    ; __G__.____
    ; __100.0000
    LDI     R20,    0x40
    OUT     PORTC,  R20
    RCALL   delay

    ; ____E.____
    ; __001.0000
    LDI     R20,    0x10
    OUT     PORTC,  R20
    RCALL   delay

    ; _____.D___
    ; __000.1000
    LDI     R20,    0x08
    OUT     PORTC,  R20
    RCALL   delay

    ; _____._C__
    ; __000.0100
    LDI     R20,    0x04
    OUT     PORTC,  R20
    RCALL   delay

    ; __G__.____
    ; __100.0000
    LDI     R20,    0x40
    OUT     PORTC,  R20
    RCALL   delay

    ; ___F_.____
    ; __010.0000
    LDI     R20,    0x20
    OUT     PORTC,  R20
    RCALL   delay

    RJMP    loop



; Registro de subrutina actual

; [0 0 0 0 . 0 0 0 0]
; subrutina_x:
;     LDI     R30     1
;     ...
;     RJMP    delay   


; subrutina_y:
;     LDI     R30     2
;     ; ...
;     RJMP    delay   

; subrutina_z:
;     LDI     R30     3
;     ...
;     RJMP    delay   


delay:
    DEC     CNT1
    BRNE    delay

    DEC     CNT2
    BRNE    delay

    DEC     CNT3
    LDI     CNT2,   50
    BRNE    delay

    ; SBRS    R30,    0
    ; RJMP    subrutina_x

    ; SBRS    R30,    0
    ; RJMP    subrutina_x

    RJMP    loop


    ; RET
; subrutina_x:
;     LDI R25,    5
;     ...
;     ; cargue R20 con FF
;     ; mostré R20 por PORTC
;     ; Subritina de retraso
;     RCALL   delay

; algo_recursivo:
;     LDI R25,    6
;     ...
;     RCALL   delay
;     algo_rec_2:
;     LDI R25,    7

;     ...

    
;     CPI     R25,    5
;     BREQ    subrutina_x

;     CPI     R25,    6
;     BREQ    algo_rec_2

;     RJMP   loop


/*
reset:
    R20 = 0000.0000
    R21 = 0000.0001
    R22 = 0001.1000

loop:
    if x => parpadeo

    ; if y => der_izq

    RJMP    loop

parpadeo:
    R20 = 0000.0000 -> 1111.1111

der_izq:
    R21 = 0000.0001 -> 0000.0010
          0000.0010 -> 0000.0100
          0000.0010 -> 0000.1000

expandir:
    R22 = 0001.1000 -> 0010.0100
    RJMP    loop
*/

