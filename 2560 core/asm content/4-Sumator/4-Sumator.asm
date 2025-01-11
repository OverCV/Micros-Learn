    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main


main:
    ; Pines A como entrada
    LDI     R16,    0
    OUT     DDRA,   R16
    ; Puerto C como salida
    LDI     R16,    255
    OUT     DDRC,   R16

loop:
    IN      R18,    PINA

    MOV     R19,    R18
    SWAP    R19
    ANDI    R18,    0x0F
    ANDI    R19,    0x0F
    
    ADD     R18,    R19
    OUT     PORTC,  R18
    LDI     R18,    0

    RJMP    loop
