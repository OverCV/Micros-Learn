    ; .include"m2560def.inc"
    .org(0x0000)
    RJMP    main

    ; Programa donde la entrada del bit respectivo al puerto A enciende la salida del bit asociado del puerto C.
    ; Los pines de entrada en puerto D desde 0 -> 3
    ; Serán vistos como salidas del puerto D desde 4 -> 7

    ; Data directions para configurar
    ; Pines son para lectura
    ; Puertos son para escritura

main:
    ; DDRx = Data Direction Register x - Dirección de x
    LDI     R16,    0x00    ; 0000 0000
    OUT     DDRA,   R16     ; CONF DDRA, ENTRADA

    LDI     R16,    0xFF    ; 1111 1111
    OUT     DDRC,   R16     ; CONF DDRC, SALIDA

loop:
    IN      R16,    PINA    ; [0000 0000]
    OUT     PORTC,  R16     ; ESCRIBA, PORTC, R16

    ; Banderas: OVF CARR NEG .. ZER
    ; evaluación de la condición -> R16 == 0 -> Alterar la banderas
    ; BREQ main
    ; BRanch if EQual


    ; Relative JuMP
    RJMP    loop

; 0x0000 00 00 BF AF 00 00 00
; 0x0001 00 00 00 00 00 00 00
; 0x0002 00 00 00 00 00 00 00
; ...
; 0x000F 00 00 00 00 00 00 00