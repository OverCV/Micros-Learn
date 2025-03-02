    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

/*
Definición: Programa que haga un retraso en un contador binario.
*/

main:
    ; contadorcito
    LDI     R17,    1

    ; Código aquí 🤗
    LDI     R16,    0xFF   ; 1111 1111


    LDI     R20,    0
    OUT     DDRA,   R20
    ; Activar puertos de salida
    OUT     DDRC,   R16

    
loop:
    IN      R17,    PINA

    OUT     PORTC,  R17

    ; INC     R17
    
    RJMP    loop        ; Relative Jump


delay:
    