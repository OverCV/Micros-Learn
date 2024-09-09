    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main


; Modificar el nible menos significativo de la
; posición de memoria 200H a 1111

main:
    LDI     R16,        0xAB    ; Setear un valor al azar

    STS     0x0200,     R16     ; Acción de interfáz
    LDS     R17,        0x200   ; Sólo traer el valor

    ORI     R17,        0x0F    ; Con una OR Inmediata cambiamos el valor del nibble bajo a F
    STS     0x200,      R17     ; Almacenar el valor nuevamente en memoria externa

    RJMP main