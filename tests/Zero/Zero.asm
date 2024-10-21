    .include"m2560def.inc"

    .org(0x0000)
    RJMP    main

main:
    ; Código aquí 🤗

reset:
    ; Inicializar

loop:
    RJMP    loop

.org(0x0100)
table:
    ; Tabla de datos
    ; .dw
