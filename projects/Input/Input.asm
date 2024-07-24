.include "m328Pdef.inc"

.org 0x0000
    rjmp main

    main:
        ; Código aquí 🤗
        LDI R16,    0x00
        OUT DDRD,   R16

        LDI R16,    0xFF
        OUT DDRB,   R16

        OUT PORTD,  R16
    loop:
        IN  R16,    PIND
        OUT PORTB,  R16

        rjmp loop


;   En DDRx set { 0: out, 1: inp }
;       if (DDRx==0) => {
;           PORTx { 0: H, 1: L }
;       } else {
;           PORTx { 0: L, 1: H }
;       }
