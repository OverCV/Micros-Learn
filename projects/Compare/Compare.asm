    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

main:
    ; Código aquí 🤗
    LDI     R16,    'a'
    STS     0x0200, R16
    LDI     R16,    'b'
    STS     0x0201, R16
    LDI     R16,    'c'
    STS     0x0202, R16
    LDI     R16,    'd'
    STS     0x0203, R16

    LDI     R17,    'A'
    STS     0x0204, R17
    LDI     R17,    'B'
    STS     0x0205, R17
    LDI     R17,    'C'
    STS     0x0206, R17
    LDI     R17,    'D'
    STS     0x0207, R17


    ; Definición de inicio
    LDI     R20,    200
    LDI     R21,    204
    ; LDI     R22,    0x00

compare:
    CP      R20,    R21
    ;si son iguales 
    BREQ  r20  

    ; Si no es igual, reiniciamos la comparación
    BRNE    compare
    ; Incrementar contadores

    ;   Comparamos si la primera palabra es 0; Si lo es validamos!
    AND     R20,    R21
    CPI     R20,    0x00
    BREQ    led_desigualdad

    ; Reiniciar contadores

    RJMP    compare

validate:
    ; Si ambas palabras son 0, son iguales.
    CPI     R21,    0x00
    BREQ    led_iguales

    ; Sin no son iguales

    RJMP    validate

led_iguales:
    ; Encender Puerto para led de igualdad
    LDI     R20,    0x00
    LDI     R21,    0x00


    RJMP    compare

led_desigualdad:
    RJMP    compare

restart:
    LDI     R20,    200
    LDI     R21,    204

    RJMP    compare


loop:
    RJMP    loop