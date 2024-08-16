    .include"m328Pdef.inc"
    .org(0x0000)
    RJMP    main

; Comparador donde el número mayor será el mostrado, sean iguales se activará el bit 7.

; Primer número (Nibble bajo) -> PORT (D0 - D3)
; Segundo número (Nibble alto) -> PORT (D4 - D7)
; Número mayor -> PORT (B0 - B6)
; Sean iguales -> PORT (B7)

main:
    ; Código aquí 🤗

loop:
    RJMP    loop
